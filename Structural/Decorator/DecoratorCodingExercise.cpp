#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <sstream>
using namespace std;

struct Flower
{
  virtual string str() = 0;
};

struct Rose : Flower
{
  string str() override {
    return "A rose";
  }
};

struct RedFlower : Flower
{
  Flower& flower;

  RedFlower(Flower &flower) : flower(flower) {}

  string str() override
  {
    string s = flower.str();
    if (s.find("red") != string::npos) return s;
    else if (s.find("blue") != string::npos)
    {
      return s + " and red";
    }
    else return s + " that is red";
  }
};

struct BlueFlower : Flower
{
  Flower& flower;

  BlueFlower(Flower &flower) : flower(flower) {}

  string str() override
  {
    string s = flower.str();
    if (s.find("blue") != string::npos) return s;
    else if (s.find("red") != string::npos)
    {
      return s + " and blue";
    }
    else return s + " that is blue";
  }
};

#include "gtest/gtest.h"

//#include "helpers/iohelper.h"

//#include "exercise.cpp"

namespace
{
  class Evaluate : public testing::Test
  {
  public:
    Rose rose;
    BlueFlower blue_rose{rose};
    RedFlower red_rose{rose};
  };

  TEST_F(Evaluate, BaselineTest)
  {
    ASSERT_EQ("A rose", Rose{}.str());
  }

  TEST_F(Evaluate, SingleColorTests)
  {
    ASSERT_EQ("A rose that is blue", blue_rose.str());
    ASSERT_EQ("A rose that is red", red_rose.str());
  }

  TEST_F(Evaluate, RepetitionTest)
  {
    ASSERT_EQ("A rose that is red", RedFlower{red_rose}.str())
            << "This rose was made red twice, but you should print once.";
  }

  TEST_F(Evaluate, MultiColorTests)
  {
    ASSERT_EQ("A rose that is red and blue",
      BlueFlower{red_rose}.str());
    ASSERT_EQ("A rose that is blue and red",
      RedFlower{blue_rose}.str());
  }

  TEST_F(Evaluate, NestedRepetitionTest)
  {
    BlueFlower b_r_rose{red_rose};
    RedFlower r_b_r_rose{b_r_rose};
    ASSERT_EQ("A rose that is red and blue",
      r_b_r_rose.str())
      << "This is a complicated case. I'm expecting that "
      << "a rose defined as Red{Blue{Red{Rose{}}}} is printed "
      << "as 'red and blue'.";
  }

} // namespace

int main(int ac, char* av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}