#pragma once
#include <string>
#include <iostream>
#include <vector>
using namespace std;

struct ChatRoom;

struct Person
{
  string name;
  ChatRoom* room = nullptr;

  Person(const string& name);
  void receive(const string& origin, const string& message);

  void say(const string& message) const;
  vector<string> chat_log;

  void pm(const string& who, const string& message) const;

  // generated in IDE
  friend bool operator==(const Person& lhs, const Person& rhs)
  {
    return lhs.name == rhs.name;
  }

  friend bool operator!=(const Person& lhs, const Person& rhs)
  {
    return !(lhs == rhs);
  }
};
