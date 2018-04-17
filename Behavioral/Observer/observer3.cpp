#include <iostream>
#include <string>
using namespace std;
#include <boost/signals2.hpp>
using namespace boost;
using namespace signals2;

namespace {

template <typename T>
struct Observable
{
  virtual ~Observable() = default;
  signal<void(T&, const string&)> property_changed;
};

struct Person : Observable<Person>
{
  explicit Person(int age)
    : age(age)
  {
  }

  int get_age() const
  {
    return age;
  }

  void set_age(const int age)
  {
    if (this->age == age) return;

    this->age = age;
    property_changed(*this, "age");
  }

private:
  int age;
};

int main_()
{
  Person p{123};
  p.property_changed.connect([](Person&, const string& prop_name)
  {
    cout << prop_name << " has been changed" << endl;
  });
  p.set_age(20);

  getchar();
  return 0;
}
}