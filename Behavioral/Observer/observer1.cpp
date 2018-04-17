#include <string>
#include <vector>
#include <iostream>
using namespace std;
#include <boost/any.hpp>
using namespace boost;

namespace
{

  struct Person;

  struct PersonListener
  {
    virtual ~PersonListener() = default;
    virtual void person_changed(Person& p, const string& property_name, const any new_value) = 0;
  };

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

    void set_age(const int age);

    bool get_can_vote() const
    {
      return age >= 16;
    }

    void subscribe(PersonListener* pl);

    void notify(const string& property_name, const any new_value);
  private:
    int age;
    vector<PersonListener*> listeners;
  };

  void Person::set_age(const int age)
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

  void Person::subscribe(PersonListener* pl)
  {
    listeners.push_back(pl);
  }

  void Person::notify(const string& property_name, const any new_value)
  {
    for (const auto listener : listeners)
      listener->person_changed(*this, property_name, new_value);
  }

  struct ConsoleListener : PersonListener
  {
    void person_changed(Person& p, const string& property_name, const any new_value) override
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

  int main()
  {
    Person p{ 14 };
    ConsoleListener cl;
    p.subscribe(&cl);
    p.set_age(15);
    p.set_age(16);


    getchar();
    return 0;
  }

}