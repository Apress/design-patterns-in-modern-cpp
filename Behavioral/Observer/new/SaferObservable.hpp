#pragma once

#include <string>
#include <vector>
#include <mutex>


template <typename T> struct Observer;

template <typename T> struct SaferObservable
{
  std::vector<Observer<T>*> observers;
  //std::mutex mtx;
  typedef std::recursive_mutex mutex_t;
  mutex_t mtx; // not recommended
public:
  // informs all observers when a change is made
  void notify(T& source, const std::string& field_name)
  {
    std::scoped_lock<mutex_t> lock{mtx};
    for (auto observer : observers)
      if (observer) // added for reentrancy
        observer->field_changed(source, field_name);
  }

  void subscribe(Observer<T>& observer)
  {
    std::scoped_lock<mutex_t> lock{mtx};
    observers.push_back(&observer);
  }

  void unsubscribe(Observer<T>& observer)
  {
//    std::scoped_lock<mutex_t> lock{mtx};
//    observers.erase(
//      remove(observers.begin(), observers.end(), &observer),
//      observers.end()
//    );

    // avoid taking a lock here
    // avoid removal of the object
    auto it = std::find(begin(observers), end(observers), &observer);
    if (it != end(observers))
      *it = nullptr; // not viable for an std::set
  }
};