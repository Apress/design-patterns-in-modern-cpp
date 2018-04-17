#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct BankAccount
{ 
  int balance = 0;
  int overdraft_limit = -500;

  void deposit(int amount)
  {
    balance += amount;
    cout << "deposited " << amount << ", balance now " << 
      balance << "\n";
  }

  void withdraw(int amount)
  {
    if (balance - amount >= overdraft_limit)
    {
      balance -= amount;
      cout << "withdrew " << amount << ", balance now " << 
        balance << "\n";
    }
  }
};

struct Command
{
  virtual ~Command() = default;
  virtual void call() const = 0;
  virtual void undo() const = 0;
};

// should really be BankAccountCommand
struct BankAccountCommand : Command
{
  BankAccount& account;
  enum Action { deposit, withdraw } action;
  int amount;

  BankAccountCommand(BankAccount& account, 
    const Action action, const int amount)
    : account(account), action(action), amount(amount) {}

  void call() const override
  {
    switch (action)
    {
    case deposit:
      account.deposit(amount);
      break;
    case withdraw: 
      account.withdraw(amount);
      break;
    default: break;
    }
  }

  void undo() const override
  {
    switch (action)
    {
    case withdraw:
      account.deposit(amount);
      break;
    case deposit:
      account.withdraw(amount);
      break;
    default: break;
    }
  }
};

// vector doesn't have virtual dtor, but who cares?
struct CompositeBankAccountCommand 
  : vector<BankAccountCommand>, Command
{
  CompositeBankAccountCommand(const initializer_list<value_type>& items)
    : vector<BankAccountCommand>(items) {}

  void call() const override
  {
    for (auto& cmd : *this)
      cmd.call();
  }

  void undo() const override
  {
    for (auto& cmd : *this)
      cmd.undo();
  }
};

int main_987()
{
  BankAccount ba;
  /*vector<BankAccountCommand> commands{*/
  CompositeBankAccountCommand commands {
    BankAccountCommand{ba, BankAccountCommand::deposit, 100},
    BankAccountCommand{ba, BankAccountCommand::withdraw, 200}
  };

  cout << ba.balance << endl;

  // apply all the commands
  /*for (auto& cmd : commands)
  {
    cmd.call();
  }*/
  commands.call();

  cout << ba.balance << endl;

  /*for_each(commands.rbegin(), commands.rend(),
    [](const BankAccountCommand& cmd) { cmd.undo(); });*/
  commands.undo();

  cout << ba.balance << endl;

  getchar();
  return 0;
}
