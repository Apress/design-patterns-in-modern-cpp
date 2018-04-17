#pragma once
#include <string>

struct Person
{
  std::string name;

  class PersonImpl;
  PersonImpl *impl; // bridge - not necessarily inner class, can vary

  Person();
  ~Person();

  void greet();
};

// pimpl
// binary interfaces are fragile; this removes most of the internals to a separate class
// prevents recompilation of sources reliant on the header