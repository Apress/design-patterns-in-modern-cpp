// visitor examples for design patterns c++ book
#include <sstream>
#include <string>
#include <iostream>
using namespace std;

struct Expression
{
  virtual void print(ostringstream& oss) = 0;
};

struct DoubleExpression : Expression
{
  double value;
  explicit DoubleExpression(const double value)
    : value{value} {}

  void print(ostringstream& oss) override
  {
    oss << value;
  }
};

struct AdditionExpression : Expression
{
  Expression *left, *right;

  AdditionExpression(Expression* const left, Expression* const right)
    : left{left}, right{right} {}

  ~AdditionExpression()
  {
    delete left;
    delete right;
  }

  void print(ostringstream& oss) override
  {
    oss << "(";
    left->print(oss);
    oss << "+";
    right->print(oss);
    oss << ")";
  }
};

void main_2_()
{
  auto e = new AdditionExpression{
    new DoubleExpression{1},
    new AdditionExpression{
      new DoubleExpression{2},
      new DoubleExpression{3}
    }
  };
  ostringstream oss;
  e->print(oss);
  cout << oss.str() << endl;
}