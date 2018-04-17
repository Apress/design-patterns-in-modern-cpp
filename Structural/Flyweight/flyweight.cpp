#include <iostream>
#include <string>
#include <cstdint>
using namespace std;

#include <boost/bimap.hpp>
#include <boost/flyweight.hpp>
#include <boost/flyweight/key_value.hpp>
using namespace boost;
using namespace flyweights;

// coloring in the console by-letter vs using ranges

// boost.flyweight  

// naive
typedef uint32_t key;

// mmorpg
struct User
{
  User(const string& first_name, const string& last_name)
    : first_name{add(first_name)}, last_name{add(last_name)}
  {
  }

  const string& get_first_name() const
  {
    return names.left.find(last_name)->second;
  }

  const string& get_last_name() const
  {
    return names.left.find(last_name)->second;
  }

  static void info()
  {
    for (auto entry : names.left)
    {
      cout << "Key: " << entry.first << ", Value: " << entry.second << endl;
    }
  }

  friend ostream& operator<<(ostream& os, const User& obj)
  {
    return os
      << "first_name: " << obj.first_name << " " << obj.get_first_name()
      << " last_name: " << obj.last_name << " " << obj.get_last_name();
  }

protected:
  static bimap<key, string> names;
  static int seed;

  static key add(const string& s)
  {
    auto it = names.right.find(s);
    if (it == names.right.end())
    {
      // add it
      key id = ++seed;
      names.insert({seed, s});
      return id;
    }
    return it->second;
  }
  key first_name, last_name;
};

key User::seed = 0;
bimap<key, string> User::names{};

void naive_flyweight()
{
  User john_doe{ "John", "Doe" };
  User jane_doe{ "Jane", "Doe" };

  cout << "John " << john_doe << endl;
  cout << "Jane " << jane_doe << endl;

  User::info();
}

struct User2
{
  flyweight<string> first_name, last_name;

  User2(const string &first_name, const string &last_name)
    : first_name(first_name),
      last_name(last_name) {}
};

void boost_flyweight()
{
  User2 user1{"John", "Smith"};
  User2 user2{"Jane", "Smith"};

  cout << user1.first_name << endl;

  cout << boolalpha
       << (&user1.first_name.get() == &user2.first_name.get()) << endl;
  cout << boolalpha
       << (&user1.last_name.get() == &user2.last_name.get()) << endl;
}

int main_()
{
  naive_flyweight();
  boost_flyweight();

  getchar();
  return 0;
}
