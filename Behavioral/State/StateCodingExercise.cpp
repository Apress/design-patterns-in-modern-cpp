#include <iostream>
#include <vector>
#include <string>
using namespace std;

class CombinationLock
{
    vector<int> combination;
    int digits_entered{0};
    bool failed{false};

    void reset()
    {
      status = "LOCKED";
      digits_entered = 0;
      failed = false;
    }
public:
    string status;

    CombinationLock(const vector<int> &combination) : combination(combination) {
      reset();
    }

    void enter_digit(int digit)
    {
      if (status == "LOCKED") status = "";
      status +=  to_string(digit);
      if (combination[digits_entered] != digit)
      {
        failed = true;
      }
      digits_entered++;

      if (digits_entered == combination.size())
        status = failed ? "ERROR" : "OPEN";
    }
};

#include "gtest/gtest.h"

//#include "helpers/iohelper.h"

//#include "exercise.cpp"

namespace {

    class Evaluate : public ::testing::Test {};

    TEST_F(Evaluate, TestSuccess)
    {
      CombinationLock cl({1,2,3});
      ASSERT_EQ("LOCKED", cl.status);

      cl.enter_digit(1);
      ASSERT_EQ("1", cl.status);

      cl.enter_digit(2);
      ASSERT_EQ("12", cl.status);

      cl.enter_digit(3);
      ASSERT_EQ("OPEN", cl.status);
    }

    TEST_F(Evaluate, TestFailure)
    {
      CombinationLock cl({1,2,3});
      ASSERT_EQ("LOCKED", cl.status);

      cl.enter_digit(1);
      ASSERT_EQ("1", cl.status);

      cl.enter_digit(2);
      ASSERT_EQ("12", cl.status);

      cl.enter_digit(5);
      ASSERT_EQ("ERROR", cl.status);
    }

}  // namespace

int main(int ac, char* av[])
{
  //::testing::GTEST_FLAG(catch_exceptions) = false;
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}