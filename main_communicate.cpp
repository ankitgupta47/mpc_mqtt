#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>	// For sleep
#include <atomic>
#include <chrono>
#include <cstring>
#include "include/ocp.h"
#include "mqtt/async_client.h"

const std::string SERVER_ADDRESS { "tcp://broker.hivemq.com:1883" };
const std::string CLIENT_ID				{ "mpcController" };
const std::string TOPIC {"CONTROL/FORCE"};


// The QoS for sending data
const int QOS = 1;

// How often to sample the "data"
const auto SAMPLE_PERIOD = std::chrono::milliseconds(5);

const auto TIMEOUT = std::chrono::seconds(10);

uint64_t timestamp()
{
	auto now = std::chrono::system_clock::now();
	auto tse = now.time_since_epoch();
	auto msTm = std::chrono::duration_cast<std::chrono::milliseconds>(tse);
	return uint64_t(msTm.count());
}

std::array<double, 4U> get_state_measurement()
{
    return {0,0.1,0,0};
}

// How much the "data" needs to change before we publish a new value.
// const int DELTA_MS = 100;

// How many to buffer while off-line
// const int MAX_BUFFERED_MESSAGES = 1200;
int main(int argc, char* argv[])
{
	std::cout << "Initializing for server '" << SERVER_ADDRESS << "'..." << std::endl;
    mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);
    
    // Set callbacks for when connected and connection lost.

	client.set_connected_handler([&client](const std::string&) {
		std::cout << "*** Connected ("
			<< timestamp() << ") ***" << std::endl;
	});

	client.set_connection_lost_handler([&client](const std::string&) {
		std::cout << "*** Connection Lost ("
			<< timestamp() << ") ***" << std::endl;
	});

    std::array<double, 4U> new_measurement{get_state_measurement()};

    std::unique_ptr<ocp::OCP> mpcController(new ocp::OCP());

    double control_input = mpcController->step_ocp(new_measurement);

    printf("input=%f\n",control_input);

    std::string PAYLOAD = std::to_string(control_input); 

    auto willMsg = mqtt::message(TOPIC, &PAYLOAD, QOS);

    auto connOpts = mqtt::connect_options_builder()
		.clean_session(true)
		.will(willMsg)
        .automatic_reconnect(true)
		.finalize();



}