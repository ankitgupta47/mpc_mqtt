#ifndef PENDULUM_MPC_TEST_H_
#define PENDULUM_MPC_TEST_H_
#include "include/ocp.h"
#include  "gtest/gtest.h"


namespace ocp
{
namespace test
{
class MPCTest: public ::testing::Test {


 protected:
  OCP ocp_;
 // void SetUp();
//void TearDown();
//  void initialize_state(std::array<double,4U> &);
}; 

}//test
}//ocp
#endif

