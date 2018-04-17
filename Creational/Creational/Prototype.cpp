#include <string>
#include <iostream>
#include <memory>
#include <functional>
#include <sstream>
#include "Person.h"
using namespace std;
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

struct Address
{
  string street;
  string city;
  int suite;


  Address(const string& street, const string& city, const int suite)
    : street{street},
      city{city},
      suite{suite}
  {
  }

  /*Address(const Address& other)
    : street{other.street},
      city{other.city},
      suite{other.suite}
  {
  }*/

  friend ostream& operator<<(ostream& os, const Address& obj)
  {
    return os
      << "street: " << obj.street
      << " city: " << obj.city
      << " suite: " << obj.suite;
  }
};


struct Contact
{
  string name;
  Address* address;

  Contact& operator=(const Contact& other)
  {
    if (this == &other)
      return *this;
    name = other.name;
    address = other.address;
    return *this;
  }

  Contact() : name(nullptr), address(nullptr)
  {} // required for serialization

  Contact(string name, Address* address)
    : name{name}, address{address}
  {
    //this->address = new Address{ *address };
  }

  Contact(const Contact& other)
    : name{other.name}
    //, address{ new Address{*other.address} }
  {
    address = new Address(
      other.address->street, 
      other.address->city, 
      other.address->suite
    );
  }


private:
  friend class boost::serialization::access;

  template <class archive>
  void save(archive& ar, const unsigned version) const
  {
    ar << name;
    ar << address;
  }

  template <class archive>
  void load(archive& ar, const unsigned version)
  {
    ar >> name;
    ar >> address;
  }

  BOOST_SERIALIZATION_SPLIT_MEMBER()

public:
  ~Contact()
  {
    delete address;
  }


  friend ostream& operator<<(ostream& os, const Contact& obj)
  {
    return os
      << "name: " << obj.name
      << " works at " << *obj.address; // note the star here
  }
};

struct EmployeeFactory
{
  static Contact main;
  static Contact aux;

  static unique_ptr<Contact> NewMainOfficeEmployee(string name, int suite)
  {
    //static Contact p{ "", new Address{ "123 East Dr", "London", 0 } };
    return NewEmployee(name, suite, main);
  }

  static unique_ptr<Contact> NewAuxOfficeEmployee(string name, int suite)
  {
    return NewEmployee(name, suite, aux);
  }

private:
  static unique_ptr<Contact> NewEmployee(string name, int suite, Contact& proto)
  {
    auto result = make_unique<Contact>(proto);
    result->name = name;
    result->address->suite = suite;
    return result;
  }
};

//Contact EmployeeFactory::main{ "", new Address{ "123 East Dr", "London", 0 } };
//Contact EmployeeFactory::aux{ "", new Address{ "123B East Dr", "London", 0 } };

int main_3423()
{
  // this is tedious
  // Contact john{ "John Doe", new Address{"123 East Dr", "London"} };
  // Contact jane{ "Jane Doe", new Address{"123 East Dr", "London"} };

  auto addr = new Address{ "123 East Dr", "London", 0 /* ? */ };

  //Contact john{ "John Doe", addr };
  //john.address->suite = 123;
  //Contact jane{ "Jane Doe", addr };
  //jane.address->suite = 124;

  //Contact jane2{ jane }; // shallow copy
  //jane2.address->suite = 555;

  

  //
  //std::cout << jane2 << std::endl;

  // whenever an address is needed, make a copy
  /*Contact john{ "John Doe", new Address{*addr} };
  john.address->suite = 123;

  Contact jane{ "Jane Doe", new Address{*addr} };
  jane.address->suite = 125;

  cout << john << "\n" << jane << endl;*/

  // much better. let's list employees
  //Contact employee{ "Unknown", new Address{"628 Happy St", "Joy", 0} };

  //// we can use this prototype to create john and jane
  //Contact john{ employee };
  //john.name = "John Doe";
  //john.address->suite = 123;

  //Contact jane{ employee };
  //jane.name = "Jane Doe";
  //jane.address->suite = 125;

  //cout << john << "\n" << jane << "\n";

  //delete addr;

  // 4. Boost Serialization

  auto clone = [](const Contact& c)
  {
    // 1. Serialize the contact
    ostringstream oss;
    boost::archive::text_oarchive oa(oss);
    oa << c;
    string s = oss.str();

    // 2. Deserialize the contact
    istringstream iss(oss.str());
    boost::archive::text_iarchive ia(iss);
    Contact result;
    ia >> result;
    return result;
  };
  
  // contact john = ...
  // contact jane = clone(john)

  //auto john = EmployeeFactory::NewAuxOfficeEmployee("John Doe", 123);
  //auto jane = EmployeeFactory::NewMainOfficeEmployee("Jane Doe", 125);

  //cout << *john << "\n" << *jane << "\n"; // note the stars here

  delete addr;

  getchar();
  return 0;
}