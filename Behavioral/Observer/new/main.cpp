#include "Headers.hpp"
#include "Observer.hpp"
#include "Observable.hpp"
#include "SaferObservable.hpp"

class Person : public SaferObservable<Person>
{
  int age;
public:
  Person(int age) : age(age) {}

  int get_age() const
  {
    return age;
  }

  // no dependencies
//  void set_age(int age)
//  {
//    if (this->age == age) return;
//    this->age = age;
//    notify(*this, "age");
//  }

  void set_age(int age)
  {
    if (this->age == age) return;

    auto old_can_vote = get_can_vote();
    this->age = age;
    notify(*this, "age");

    // determine if voting status changed
    if (old_can_vote != get_can_vote())
      notify(*this, "can_vote");
  }

  bool get_can_vote() const
  {
    return age >= 16;
  }
};

// we could define a Person-specific listener
struct PersonListener
{
  virtual void person_changed(Person& p,
    const string property_name) = 0;
}; // changes can occur on other objects, so


struct ConsolePersonObserver
  : public Observer<Person> // , Observer<Creature>
{
  void field_changed(Person &source, const string &field_name) override
  {
    cout << "Person's " << field_name << " has changed to ";
    if (field_name == "age") cout << source.get_age();
    if (field_name == "can_vote") cout << boolalpha << source.get_can_vote();
    cout << ".\n";
  }
};

// keeps monitoring a person until they're old enough to drive
// when they're old enough, stops monitoring them
struct TrafficAdministration : Observer<Person>
{
  void field_changed(Person &source, const std::string &field_name) override
  {
    if (field_name == "age")
    {
      if (source.get_age() < 17)
        cout << "Whoa there, you're not old enough to drive!\n";
      else
      {
        // let's not monitor them anymore
        cout << "Oh, ok, we no longer care!\n";
        source.unsubscribe(*this);
      }
    }
  }
};

// observable with Boost.Signals2
template <typename T> struct Observable2
{
  signal<void(T&, const string&)> field_changed;
};

class Person2 : public Observable2<Person2>
{
  int age;
public:
  int get_age() const
  {
    return age;
  }

  void set_age(int age)
  {
    if (this->age == age) return;
    this->age = age;
    field_changed(*this, "age");
  }
};

int main(int ac, char* av[])
{
  // observer
  Person person{10};
  ConsolePersonObserver cpo;
  person.subscribe(cpo);

  person.set_age(11);
  person.set_age(12);

  //person.unsubscribe(&cpo);
  person.set_age(13);

  // boost signals observer
  Person2 p2;
  auto conn = p2.field_changed.connect([](Person2& p, const string& field_name)
  {
    cout << field_name << " has changed";
  });
  p2.set_age(20);
  conn.disconnect();

  // dependency problems
  cout << "Reaching adulthood...\n";
  person.set_age(20);

  // thread safety and reentrancy
  person.unsubscribe(cpo);
  TrafficAdministration ta;
  person.subscribe(ta);

  person.set_age(16);

  try
  {
    person.set_age(17);
    // notify() --> field_changed() --> ubsubscribe()
  }
  catch (const std::exception& e)
  {
    cout << "Oops, " << e.what() << "\n";
  }

  return 0;
}