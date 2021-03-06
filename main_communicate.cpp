#include <iostream>
#include <string>
#include <chrono>  // for time calculations
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <csignal> // create interupt signal
#include "include/ocp.h"
#include "mqtt/async_client.h"

const std::string SERVER_ADDRESS{"tcp://broker.hivemq.com:1883"};
const std::string CLIENT_ID{"mpcController"}; // not mandatory, can be empty
const std::string PUBLISH_TOPIC{"CONTROL/FORCE"};
const std::string SUBSCRIBE_TOPIC{"PENDULUM/STATES"};


// How many to buffer while off-line
const int MAX_BUFFERED_MESSAGES = 1200;

// The QoS for sending data
const int QOS = 1;

// How often to sample the "data"
const auto SAMPLE_PERIOD = std::chrono::milliseconds(5);

const auto TIMEOUT = std::chrono::seconds(10);

std::string timestamp()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
	return ss.str();
}

void signalHandler(int signum)
{
	std::cout << "Interrupt signal (" << signum << ") received.\n";
	// cleanup and close up stuff here
	// terminate program
	exit(signum);
}

std::array<double,4U> stringToArray(std::string &str_arr){
    
     //Test if string begins with [ and ends with ].
    if (str_arr.front() != '[' || str_arr.back() != ']')
    {
        std::cout << "String does not begin with '[' and/or end with ']'\n";
    }
    //Replace [] with whitespace.
    str_arr.front() = str_arr.back() = ' ';
    
    //Create stringstream from string.
    auto my_stream = std::istringstream(str_arr);
    double n;
    std::array<double,4U> arr{};
    int i = 0;
    while (my_stream) {
        // Streaming until space is encountered
        my_stream >> n;
 
        // If my_stream is not empty
        if (my_stream) {
           arr.at(i) = n;
        }
        i++;
    }
	for (auto x:arr){
		std::cout<<x<<" ";
	}
	std::cout<<"\n";
return arr;
}

std::array<double, 4U> get_state_measurement()
{
	return {0, 0.1, 0, 0};
}

// How much the "data" needs to change before we publish a new value.
// const int DELTA_MS = 100;

// How many to buffer while off-line
// const int MAX_BUFFERED_MESSAGES = 1200;
int main(int argc, char *argv[])
{
	std::cout << "Initializing for server '" << SERVER_ADDRESS << "'..." << std::endl;
	// auto createOpts = mqtt::create_options_builder()
	// 					  .send_while_disconnected(true, true)
	// 					  .max_buffered_messages(MAX_BUFFERED_MESSAGES)
	// 					  .delete_oldest_messages()
	// 					  .finalize();

	mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);
	// mqtt::async_client client2(SERVER_ADDRESS, "");

	// Set callbacks for when connected and connection lost.

	client.set_connected_handler([&client](const std::string &)
								 { std::cout << "*** Connected ("
											 << timestamp() << ") ***" << std::endl; });

	client.set_connection_lost_handler([&client](const std::string &)
									   { std::cout << "*** Connection Lost ("
												   << timestamp() << ") ***" << std::endl; });

	// auto willMsg = mqtt::message(PUBLISH_TOPIC, "Controller offline", QOS);

	auto connOpts = mqtt::connect_options_builder()
						.clean_session(false)
						.automatic_reconnect(true)
						.finalize();

	try
	{
		// Start consumer before connecting to make sure to not miss messages

		client.start_consuming();

		// Note that we start the connection, but don't wait for completion.
		// We configured to allow publishing before a successful connection.
		std::cout << "Initiating connection..." << std::endl;
		mqtt::token_ptr conntok = client.connect(connOpts);
		std::cout << "Waiting for the connection..." << std::endl;
		conntok->wait();

		// we need to subscribe
		client.subscribe(SUBSCRIBE_TOPIC, QOS)->wait();
		std::cout << "Waiting for messages on topic: '" << SUBSCRIBE_TOPIC << "'" << std::endl;

		// register signal SIGINT and signal handler
		signal(SIGINT, signalHandler);

		// Get the latest state measurement and compute control input
		std::unique_ptr<ocp::OCP> mpcController(new ocp::OCP());

		while (true)
		{
			auto start = std::chrono::high_resolution_clock::now();

			// Get pedulum states
			auto msg = client.consume_message();
			std::string system_state = msg->to_string();
			std::cout << msg->get_topic() << ": " << system_state << std::endl;


			std::array<double, 4U> new_measurement{stringToArray(system_state)};
			double control_input = mpcController->step_ocp(new_measurement);
			auto stop = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

			// To get the value of duration use the count()
			// member function on the duration object
			std::cout << "input: " << control_input << ", computation time: " << duration.count() / 1000.0F << " (milliseconds)" << std::endl;

			// Publish the computed control input.
			auto topic = mqtt::topic(client, PUBLISH_TOPIC, QOS);
			std::cout << "Publishing data..." << std::endl;
			topic.publish(std::to_string(control_input));
		}

		// Disconnect
		std::cout << "\nDisconnecting..." << std::endl;
		client.disconnect()->wait();
		std::cout << "  ...OK" << std::endl;
	}
	catch (const mqtt::exception &exc)
	{
		std::cerr << exc.what() << std::endl;
		return 1;
	}
}