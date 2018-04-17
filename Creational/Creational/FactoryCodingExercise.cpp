#include <vector>
#include <string>
#include <gtest/gtest.h>
using namespace std;

namespace FactoryExercise {

struct Person
{
  int id;
  string name;
};

class PersonFactory
{
  int id{ 0 };
public:
  Person create_person(const string& name)
  {
    return { id++, name };
  }
};

#include "gtest/gtest.h"

//#include "helpers/iohelper.h"

//#include "exercise.cpp"


namespace
{
  class Evaluate : public testing::Test
  {
  };

  TEST_F(Evaluate, SimplePersonTest)
  {
    PersonFactory pf;

    auto p1 = pf.create_person("Chris");
    ASSERT_EQ("Chris", p1.name);

    auto p2 = pf.create_person("Sarah");
    ASSERT_EQ(1, p2.id) << "Expected the second created person's id to be = 1";
  }
} // namespace

}

int main(int ac, char* av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}