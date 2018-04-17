#include <iostream>
#include <string>
#include <memory>
#include <sstream>
using namespace std;
#include <boost/lexical_cast.hpp>
#include <boost/functional.hpp>
using namespace boost;

struct Literal;
struct Addition;

struct ExpressionVisitor
{
  virtual void visit(const Literal& lit) = 0;
  virtual void visit(const Addition& add) = 0;
};

struct PrintVisitor : ExpressionVisitor
{
  void visit(const Literal& lit) override;
  void visit(const Addition& addition) override;

  string str() const
  {
    return oss.str();
  }
private:
  ostringstream oss;
};

struct Expression
{
  virtual ~Expression()
  {
  }

  virtual void print(ostringstream& oss) = 0;
  virtual void accept(PrintVisitor& pv) = 0;
};

struct Literal : Expression
{
  void print(ostringstream& oss) override
  {
    oss << lexical_cast<string>(value);
  }

  explicit Literal(double value)
    : value{value}
  {
  }

  void accept(PrintVisitor& pv) override
  {
    pv.visit(*this);
  }

  double value;
};

struct Addition : Expression
{
  std::shared_ptr<Expression> left, right;


  Addition(const std::shared_ptr<Expression>& left, const std::shared_ptr<Expression>& right)
    : left{left},
      right{right}
  {
  }

  void accept(PrintVisitor& pv) override
  {
    pv.visit(*this);
  }

  void print(ostringstream& oss) override
  {
    oss << "(";
    left->print(oss);
    oss << " + ";
    right->print(oss);
    oss << ")";
  }
};

void PrintVisitor::visit(const Literal& literal)
{
  this->oss << literal.value;
}

void PrintVisitor::visit(const Addition& addition)
{
  this->oss << "(";
  addition.left->accept(*this);
  this->oss << " + ";
  addition.right->accept(*this);
  this->oss << ")";
}

// dispatch - single, double, multiple

// acyclic visitors

// loki multimethods and https://ideone.com/8VxALs

// https://github.com/Flast/multimethod
// also one of the answers here http://stackoverflow.com/questions/23336586/did-the-loki-multimethods-make-it-into-c11

std::shared_ptr<Literal> lit(double value)
{
  return make_shared<Literal>(value);
}

std::shared_ptr<Addition> add(
  const std::shared_ptr<Expression>& left, 
  const std::shared_ptr<Expression>& right)
{
  return make_shared<Addition>(left, right);
}

void double_dispatch()
{
  PrintVisitor pv;
  auto expr = add(add(lit(1), lit(2)), lit(3));
  pv.visit(*expr);
  cout << pv.str() << endl;
}

void dynamic_visitor()
{
  
}

void static_visitor()
{
  ostringstream oss; // visitor
  add(add(lit(1), lit(2)), lit(3))->print(oss);
  cout << oss.str() << endl;
}

int main_()
{
  static_visitor();
  double_dispatch();

  getchar();
  return 0;
}
