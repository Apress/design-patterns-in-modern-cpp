#include "person.h"
#include "chatroom.h"

Person::Person(const string& name) : name(name)
{
}

void Person::receive(const string& origin, const string& message)
{
  string s{ origin + ": \"" + message + "\"" };
  cout << "[" << name << "'s chat session] " << s << "\n";
  chat_log.emplace_back(s);
}

void Person::say(const string& message) const
{
  room->broadcast(name, message);
}

void Person::pm(const string& who, const string& message) const
{
  room->message(name, who, message);
}
