#include <iostream>
#include <string>
#include <sstream>
using namespace std;

// cyclic visitor: based on function overloading
//                 works only on a stable hierarchy
// acyclic visitor: based on RTTI
//                  no hierarchy limitations, but slower

template <typename Visitable>
struct Visitor
{
  virtual void visit(Visitable& obj) = 0;
};

struct VisitorBase // marker interface
{
  virtual ~VisitorBase() = default;
};

struct Expression
{
  virtual ~Expression() = default;

  virtual void accept(VisitorBase& obj)
  {
    using EV = Visitor<Expression>;
    if (auto ev = dynamic_cast<EV*>(&obj))
      ev->visit(*this);
  }
};

struct DoubleExpression : Expression{
  double value;

  DoubleExpression(double value) : value(value) {}

  virtual void accept(VisitorBase& obj)
  {
    using DEV = Visitor<DoubleExpression>;
    if (auto ev = dynamic_cast<DEV*>(&obj))
      ev->visit(*this);
  }
};

struct AdditionExpression : Expression
{
  Expression *left, *right;

  AdditionExpression(Expression *left, Expression *right) : left(left), right(right) {}

  ~AdditionExpression()
  {
    delete left;
    delete right;
  }

  virtual void accept(VisitorBase& obj)
  {
    using AEV = Visitor<AdditionExpression>;
    if (auto ev = dynamic_cast<AEV*>(&obj))
      ev->visit(*this);
  }
};

struct ExpressionPrinter : VisitorBase,
                           Visitor<Expression>,
                           //Visitor<DoubleExpression>,
                           Visitor<AdditionExpression>
{
  void visit(Expression &obj) override
  {
    // fallback?
  }

  // can remove double visitor without failure
//  void visit(DoubleExpression &obj) override
//  {
//    oss << obj.value;
//  }

  void visit(AdditionExpression &obj) override
  {
    oss << "(";
    obj.left->accept(*this);
    oss << "+";
    obj.right->accept(*this);
    oss << ")";
  }

  string str() const { return oss.str(); }
private:
  ostringstream oss;
};

int main()
{
  auto e = new AdditionExpression{
    new DoubleExpression{1},
    new AdditionExpression{
      new DoubleExpression{2},
      new DoubleExpression{3}
    }
  };

  ExpressionPrinter ep;
  ep.visit(*e);
  cout << ep.str() << "\n";

	//getchar();
	return 0;
}
