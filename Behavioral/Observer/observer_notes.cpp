#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <mutex>
class Foo;
using namespace std;

// function pointer

// observer pattern in quantlib

// changes in a collection vs its members

// boost::signal
// proeprty change notification
// rxcpp

recursive_mutex mtx;
// but even so, this can deadlock

struct Listener
{
  virtual void fooChanged(Foo* foo); // not const
  virtual ~Listener() = default;
};


class Foo
{
  string s;
  vector<Listener*> listeners; // set<>?
  // also, list<> instead of vector lets you remove while iterating
public:

  virtual string GetS() const
  {
    return s;
  }

  virtual void SetS(const string cs)
  {
    // lock here too?
    if (s != cs)
    {
      s = cs;
      notifyListeners();
    }
  }

  void addListener(Listener* l)
  {
    listeners.push_back(l);

    // add might reallocate the vector, so recursive add fails
  }

  bool removeListener(Listener* l)
  {
    // doesn't remove things
    //remove(listeners.begin(), listeners.end(), l);



    listeners.erase(
      remove(listeners.begin(), listeners.end(), l),
      listeners.end()
    );

    // alternatively, find and remove
    // this just removes 1st occurrence
    listeners.erase(
      find(listeners.begin(), listeners.end(), l)
    );

    // but what happens if item is NOT in the list?
    // this crashes because find() returns end() :(


    auto it = find(listeners.begin(), listeners.end(), l);
    if (it != listeners.end())
    {
      listeners.erase(it);

      // return true?
    }
    // return false

    // can also begin rbegin()/rend()
    // no, erase() does not take a reverse_iterator

    // whose alice are we removing? should we be
    // returning a memento?

    // point is, this is all crap for ordering and such

    // how about NOT removing but just nulling
    for (auto it = listeners.begin(); it != listeners.end(); ++it)
    {
      if (*it == l)
      {
        *it = nullptr; // only the first one
        // but we can only handle the case of removing ourselves
        // not other people
        return true;
      }
    }

    // and BTW, null listeners will be nuked! maybe a check?

    return false;
  }
private:
  void notifyListeners()
  {
    lock_guard<recursive_mutex> lg(mtx); // non-recursive

    if (listeners.empty()) return;

    // implicit ++ here that will crash on recusive
    // array indexing?
    int size = listeners.size();
    for (int i = 0; i < size; ++i)
      if (listeners[i])
        listeners[i]->fooChanged(this); // not safe
    // remove any nulls
    listeners.erase(
      remove(listeners.begin(), listeners.end(), nullptr),
      listeners.end()
    );

    // better version at 31:12
  }
};

void Listener::fooChanged(Foo* foo)
{
  cout << "foo has been changed to " << foo << "\n";
}

struct ScopeListener : Listener
{
  Foo& foo;

  explicit ScopeListener(Foo& foo)
    : foo(foo)
  {
    startListening();
  }

  ~ScopeListener()
  {
    stopListening();
  }

  void startListening() { foo.addListener(this); }
  void stopListening() { foo.removeListener(this); }


  void fooChanged(Foo* foo) override;
};

void ScopeListener::fooChanged(Foo* foo)
{
  cout << foo->GetS();
  this->stopListening();
}

void cppnow_talk()
{
  // alice and bob - listeners are ordered (priority?)

  // add same listener twice - should it be a set?
  // if I remove one, I remove 2 of them
  // locally code worries about itself, but then,
  // how do I know an item is already there
  // either usage_count or set<>

  Foo foo;
  ScopeListener sl(foo);

  foo.SetS("abc"); // will crash
                   // removing a listener inside a listener
                   // changing an iterated collection!

  // and then an observer has a pointer to a different observer
}

int main__()
{
  cppnow_talk();

  getchar();
  return 0;
}
