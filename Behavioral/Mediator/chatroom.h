#pragma once

struct ChatRoom
{
  vector<Person*> people; // assume append-only

  void join(Person* p);
  void broadcast(const string& origin, const string& message);
  void message(const string& origin, const string& who, const string& message);
};
