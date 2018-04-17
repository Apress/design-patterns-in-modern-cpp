#include <iostream>
#include <string>
#include <memory>
using namespace std;
//#include <boost/lexical_cast.hpp>
//using namespace boost;

struct Logger
{
  virtual ~Logger() = default;
  virtual void info(const string& s) = 0;
  virtual void warn(const string& s) = 0;
};

struct OptionalLogger : Logger
{
  shared_ptr<Logger> impl;
  OptionalLogger(const shared_ptr<Logger>& logger)
    : impl(logger)
  {
  }

  void info(const string& s) override
  {
    if (impl) impl->info(s);
  }

  void warn(const string& s) override
  {
    if (impl) impl->warn(s);
  }
};

struct BankAccount
{
  shared_ptr<OptionalLogger> log;
  string name;
  int balance = 0;

  static shared_ptr<Logger> no_logging;

  BankAccount(const string& name, int balance, const shared_ptr<Logger>& logger = no_logging)
    : log{make_shared<OptionalLogger>(logger)},
      name{name},
      balance{balance}
  {
  }

  void deposit(int amount)
  {
    balance += amount;
    log->info("Deposited $" /*+ lexical_cast<string>(amount) 
      + " to " + name + ", balance is now $" + lexical_cast<string>(balance)*/);
  }

  void withdraw(int amount)
  {
    /*if (balance >= amount)
    {
      balance -= amount;
      log->info("Withdrew $" + lexical_cast<string>(amount) 
        + " from " + name + ", $" + lexical_cast<string>(balance) + " left");
    } 
    else
    {
      log->warn("Tried to withdraw $" + lexical_cast<string>(amount) +
        " from " + name + " but couldn't due to low balance");
    }*/
  }
};

shared_ptr<Logger> BankAccount::no_logging{};

struct ConsoleLogger : Logger
{
  void info(const string& s) override
  {
    cout << "INFO: " << s << endl;
  }

  void warn(const string& s) override
  {
    cout << "WARNING!!! " << s << endl;
  }
};

// similar idea
struct EmailLogger : Logger
{
  void info(const string& s) override;
  void warn(const string& s) override;
};

int main()
{
  //auto logger = make_shared<ConsoleLogger>();

  struct NullLogger : Logger
  {
    void info(const string& s) override {}
    void warn(const string& s) override {}
  };
  
  // this will crash
  shared_ptr<Logger> empty_logger;
  auto logger = make_shared<NullLogger>();
  BankAccount account{ "primary account", 1000 };
  
  account.deposit(2000);
  account.withdraw(2500);
  account.withdraw(1000);

  getchar();
  return 0;
}
