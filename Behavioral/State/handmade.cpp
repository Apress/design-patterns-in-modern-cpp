#include <iostream>
#include <string>
#include <map>
#include <vector>
using namespace std;

enum class State
{
  off_hook,
  connecting,
  connected,
  on_hold,
  on_hook
};

inline ostream& operator<<(ostream& os, const State& s)
{
  switch (s)
  {
    case State::off_hook:
      os << "off the hook";
      break;
    case State::connecting:
      os << "connecting";
      break;
    case State::connected:
      os << "connected";
      break;
    case State::on_hold:
      os << "on hold";
      break;
    case State::on_hook:
      os << "on the hook";
      break;
  }
  return os;
}

enum class Trigger
{
  call_dialed,
  hung_up,
  call_connected,
  placed_on_hold,
  taken_off_hold,
  left_message,
  stop_using_phone
};

inline ostream& operator<<(ostream& os, const Trigger& t)
{
  switch (t)
  {
    case Trigger::call_dialed:
      os << "call dialed";
      break;
    case Trigger::hung_up:
      os << "hung up";
      break;
    case Trigger::call_connected:
      os << "call connected";
      break;
    case Trigger::placed_on_hold:
      os << "placed on hold";
      break;
    case Trigger::taken_off_hold:
      os << "taken off hold";
      break;
    case Trigger::left_message:
      os << "left message";
      break;
    case Trigger::stop_using_phone:
      os << "putting phone on hook";
      break;
    default: break;
  }
  return os;
}

int main_f(char* argv[])
{
  map<State, vector<pair<Trigger, State>>> rules;

  rules[State::off_hook] = {
    {Trigger::call_dialed, State::connecting},
    {Trigger::stop_using_phone, State::on_hook}
  };

  rules[State::connecting] = {
    {Trigger::hung_up, State::off_hook},
    {Trigger::call_connected, State::connected}
  };

  rules[State::connected] = {
    {Trigger::left_message, State::off_hook},
    {Trigger::hung_up, State::off_hook},
    {Trigger::placed_on_hold, State::on_hold}
  };

  rules[State::on_hold] = {
    {Trigger::taken_off_hold, State::connected},
    {Trigger::hung_up, State::off_hook}
  };

  State currentState{ State::off_hook },
    exitState{ State::on_hook };

  while (true)
  {
    cout << "The phone is currently " << currentState << endl;
    select_trigger:
    cout << "Select a trigger:" << "\n";
 
    int i = 0;
    for (auto item : rules[currentState])
    {
      cout << i++ << ". " << item.first << "\n";
    }

    int input;
    cin >> input;
    if (input < 0 || (input+1) > rules[currentState].size())
    {
      cout << "Incorrect option. Please try again." << "\n";
      goto select_trigger;
    }

    currentState = rules[currentState][input].second;
    if (currentState == exitState) break;
  }

  cout << "We are done using the phone" << "\n";

  getchar();
  getchar();
  return 0;
}
