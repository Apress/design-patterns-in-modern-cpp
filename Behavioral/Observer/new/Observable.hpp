#pragma once

#include <string>
#include <vector>

template <typename T> struct Observer;

template <typename T> struct Observable
{
  // should this be an std::set?
  std::vector<Observer<T>*> observers;
public:
  // informs all observers when a change is made
  void notify(T& source, const std::string& field_name)
  {
    for (auto observer : observers)
      observer->field_changed(source, field_name);
  }

  void subscribe(Observer<T>& observer)
  {
    observers.push_back(&observer);
  }

  void unsubscribe(Observer<T>& observer)
  {
    observers.erase(
      remove(observers.begin(), observers.end(), &observer),
      observers.end()
    );
  }
};