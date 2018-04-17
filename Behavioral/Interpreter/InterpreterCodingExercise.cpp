#include <map>
#include <string>
#include <vector>
#include <regex>
#include <iostream>
using namespace std;


inline vector<string> split(const string& stringToSplit)
{
  vector<string> result;
  size_t pos = 0, lastPos = 0;
  while ((pos = stringToSplit.find_first_of("+-", lastPos)) != string::npos)
  {
    result.push_back(stringToSplit.substr(lastPos, pos-lastPos+1));
    lastPos = pos+1;
  }
  result.push_back(stringToSplit.substr(lastPos));
  return result;
}

struct ExpressionProcessor
{
  map<char,int> variables;

  enum NextOp
  {
    nothing,
    plus,
    minus
  };

  int calculate(const string& expression)
  {
    int current;
    auto next_op = nothing;

    auto parts = split(expression);

    cout << "parts (" << parts.size() << "): ";
    for (auto& part : parts)
      cout << "`" << part << "` ";
    cout << endl;

    for (auto& part : parts)
    {
      auto no_op = split(part);
      auto first = no_op[0];
      int value, z;

      try
      {
        value = stoi(first);
      }
      catch (const invalid_argument&)
      {
        if (first.length() == 1 &&
            variables.find(first[0]) != variables.end())
        {
          value = variables[first[0]];
        }
        else return 0;
      }

      switch (next_op)
      {
        case nothing:
          current = value;
          break;
        case plus:
          current += value;
          break;
        case minus:
          current -= value;
          break;
      }

      if (*part.rbegin() == '+') next_op = plus;
      else if (*part.rbegin() == '-') next_op = minus;
    }

    return current;
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
  };

  TEST_F(Evaluate, LotsOfAccountTests)
  {
    ExpressionProcessor ep;
    ep.variables['x'] = 5;

    ASSERT_EQ(1, ep.calculate("1"));
    ASSERT_EQ(3, ep.calculate("1+2"));
    ASSERT_EQ(6, ep.calculate("1+x"));
    ASSERT_EQ(0, ep.calculate("1+xy"));
  }
} // namespace

int main(int ac, char* av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}