#include "headers.hpp"

struct BankAccount
{
  int balance{0};
  int overdraft_limit{-500};

  void deposit(int amount)
  {
    balance += amount;
    cout << "deposited " << amount
         << ", balance is now " << balance << endl;
  }

  bool withdraw(int amount)
  {
    if (balance - amount >= overdraft_limit)
    {
      balance -= amount;
      cout << "withdrew " << amount
           << ", balance is now " << balance << endl;
      return true;
    }
    return false;
  }

  friend ostream &operator<<(ostream &os, const BankAccount &account) {
    os << "balance: " << account.balance;
    return os;
  }
};

struct Command
{
  bool succeeded;
  virtual void call() = 0;
  virtual void undo() = 0;
};

struct BankAccountCommand : Command
{
  BankAccount& account;
  enum Action { deposit, withdraw } action;
  int amount;

  BankAccountCommand(BankAccount &account, Action action, int amount) : account(account), action(action),
                                                                        amount(amount) {
    succeeded = false;
  }

  void call() override {
    switch (action)
    {
      case deposit:
        account.deposit(amount);
        succeeded = true;
        break;
      case withdraw:
        succeeded = account.withdraw(amount);
        break;
    }
  }

  void undo() override {
    if (!succeeded) return;

    switch (action)
    {
      case deposit:
        account.withdraw(amount);
        break;
      case withdraw:
        account.deposit(amount);
        break;
    }
  }


};

int main()
{
  BankAccount ba;

  vector<BankAccountCommand> commands
    {
      BankAccountCommand{ba, BankAccountCommand::deposit, 100},
      BankAccountCommand{ba, BankAccountCommand::withdraw, 200}
    };

  cout << ba << endl;

  for (auto& cmd : commands)
    cmd.call();

  for (auto it = commands.rbegin(); it != commands.rend(); ++it)
  {
    it->undo();
  }

  cout << ba << endl;

  return 0;
}