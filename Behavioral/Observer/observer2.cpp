#include <string>
#include <vector>
#include <iostream>
#include <mutex>
using namespace std;
#include <boost/any.hpp>
using namespace boost;

namespace {

struct Person;

struct PersonListener
{
  virtual ~PersonListener() = default;
  virtual void person_changed(Person& p, const string& property_name, const any new_value) = 0;
};

static mutex mtx;

struct Person
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

    auto old_c_v = get_can_vote();

    this->age = age;
    notify("age", this->age);

    auto new_c_v = get_can_vote();
    if (old_c_v != new_c_v)
    {
      notify("can_vote", new_c_v);
    }
  }

  bool get_can_vote() const
  {
    return age >= 16;
  }

  void subscribe(PersonListener* pl)
  {
    lock_guard<mutex> guard{ mtx };

    // prevent existing references?
    if (find(begin(listeners), end(listeners), pl) == end(listeners))
      listeners.push_back(pl);
  }
  void ubsubscribe(PersonListener* pl)
  {
    lock_guard<mutex> guard{ mtx };
    if (listeners.empty()) return;
    // multiple identical listeners?
    // erase-remove idiom?
    for (auto it = listeners.begin(); it != listeners.end(); ++it)
    {
      if (*it == pl)
      {
        *it = nullptr; // just mark as nullptr
      }
    }
  }
private:
  int age;
  vector<PersonListener*> listeners;

  void notify(const string& property_name, const any new_value)
  {
    lock_guard<mutex> guard{ mtx };
    for (const auto listener : listeners)
      if (listener)
        listener->person_changed(*this, property_name, new_value);

    // erase-remove 
    listeners.erase(
      remove(listeners.begin(), listeners.end(), nullptr),
      listeners.end()
    );
  }

  // std::list (easier to remove elements)
  // concurrent_vector? yes, but no easy erase-remove
};

struct ConsoleListener : PersonListener
{
  void person_changed(Person& p, const string& property_name, 
    const any new_value) override
  {
    cout << "person's " << property_name << " has been changed to ";
    if (property_name == "age")
    {
      cout << any_cast<int>(new_value);
    }
    else if (property_name == "can_vote")
    {
      cout << any_cast<bool>(new_value);
    }
    cout << "\n";
  }
};

int main__x_()
{
  Person p{ 14 };
  ConsoleListener cl;
  p.subscribe(&cl);
  p.subscribe(&cl); // ignored
  p.set_age(15);
  p.set_age(16);
  p.ubsubscribe(&cl);
  p.set_age(17);


  getchar();
  return 0;
}

}