#include "include/pendulum_mpc_test.hpp"
#include <gtest/gtest.h>

namespace ocp
{
namespace test
{
//void MPCTest::SetUp(){}
//void MPCTest::TearDown(){}
//void MPCTest::initialize_state(std::array<double,4U> &x){
//  x{0,0,0,0};
//}

TEST_F(MPCTest,TestStepFunction)
{
  std::array<double,4U> x{0,0,0,0};
  //initialize_state(x);
  double u = ocp_.step_ocp(x); 
  EXPECT_EQ(u, 0);


}
}
}
