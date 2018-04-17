#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <mutex>
#include <set>
using namespace std;

#include <concurrent_vector.h>
using namespace Concurrency;

#include <boost/thread/recursive_mutex.hpp>

template<typename T> struct Observer;

template <typename T>
struct Observable
{
  typedef mutex mutex_t;

  void notify(T& source, const string& name)
  {
    vector<Observer<T>*> observers_copy;
    {
      lock_guard<mutex_t> lock{ mtx };
      observers_copy = observers;
    }
    for (auto obs : observers_copy)
      if (obs)
        obs->field_changed(source, name);

    observers.erase(
      remove(observers.begin(), observers.end(), nullptr),
      observers.end());
  }
  /*void notify(T& source, const string& name)
  {
	  lock_guard<mutex_t> lock{ mtx };
	  for (auto obs : observers)
		  if (obs)
			  obs->field_changed(source, name);

	  observers.erase(
		  remove(observers.begin(), observers.end(), nullptr),
		  observers.end());
  }*/
	void subscribe(Observer<T>* f)
	{
		lock_guard<mutex_t> lock{ mtx };
		observers.push_back(f);
	}
	void unsubscribe(Observer<T>* o)
	{
		lock_guard<mutex_t> lock{ mtx };
		auto it = find(observers.begin(), observers.end(), o);
		if (it != observers.end())
			*it = nullptr; // cannot do this for a set
	}
private:
	vector<Observer<T>*> observers;
	mutex_t mtx;
};

template<typename T> struct Observer
{
	virtual void field_changed(T& source, const string& field_name) = 0;
};

struct Person : Observable<Person>
{
	explicit Person(const int age)
		: age(age)
	{
	}

	int get_age() const
	{
		return age;
	}

	void set_age(const int age)
	{
		if (this->age == age) return;

		auto old_can_vote = can_vote();
		this->age = age;

		notify(*this, "age");

		if (old_can_vote != can_vote())
			notify(*this, "can_vote");
	}

	bool can_vote() const
	{
		return age >= 16;
	}

private:
	int age;
};

struct Creature
{
  
};

struct ConsolePersonObserver : Observer<Person>
{
	void field_changed(Person& source, const string& field_name) override
	{
		cout << "Person's " << field_name << " has changed to "
	       << source.get_age() << ".\n";
	}
};

struct TrafficAdministration : Observer<Person>
{
  void TrafficAdministration::field_changed(
    Person& source, const string& field_name) override
  {
    if (field_name == "age")
    {
      if (source.get_age() < 17)
        cout << "Whoa there, you are not old enough to drive!\n";
      else
      {
        // oh, ok, they are old enough, let's not monitor them anymore
        cout << "We no longer care!\n";
        source.unsubscribe(this);
      }
    }
  }
};



int main()
{
	cout << "observer book" << endl;

  /*Person p{ 20 };
  ConsolePersonObserver cpo;
  p.subscribe(&cpo);
  p.set_age(21);
  p.set_age(22);*/

	Person p{ 10 };
	TrafficAdministration o;
	p.subscribe(&o);
	p.set_age(16);
	p.set_age(17);
	p.set_age(21);

  getchar();
	return 0;
}
