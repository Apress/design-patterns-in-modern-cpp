#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <map>
#include <algorithm>
using namespace std;

class Person
{
  friend class ResponsiblePerson;
  int age;
public:
  Person(int age) : age(age) {}

  int get_age() const { return age; }
  void set_age(int age) { this->age=age; }

  string drink() const { return "drinking"; }
  string drive() const { return "driving"; }
  string drink_and_drive() const { return "driving while drunk"; }
};

struct ResponsiblePerson
{
  ResponsiblePerson(const Person &person) : person(person) {}

  int get_age() const { return person.age; }
  void set_age(int value) { person.age = value; }

  string drink() const {
    if (get_age() >= 18)
      return person.drink();
    else return "too young";
  }

  string drive() const {
    if (get_age() >= 16)
      return person.drive();
    else return "too young";
  }

  string drink_and_drive() const
  {
    return "dead";
  }

private:
  Person person;
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

  TEST_F(Evaluate, BaselineTest)
  {
    Person p{10};
    ResponsiblePerson rp{p};

    ASSERT_EQ("too young", rp.drive());
    ASSERT_EQ("too young", rp.drink());
    ASSERT_EQ("dead", rp.drink_and_drive());

    rp.set_age(20);

    ASSERT_EQ("driving", rp.drive());
    ASSERT_EQ("drinking", rp.drink());
    ASSERT_EQ("dead", rp.drink_and_drive());
  }

} // namespace

int main(int ac, char* av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}