// visitor examples for design patterns c++ book
#include <sstream>
#include <string>
#include <iostream>
using namespace std;

struct SubtractionExpression;
struct DoubleExpression;
struct AdditionExpression;

struct ExpressionVisitor
{
  virtual void visit(DoubleExpression* de) = 0;
  virtual void visit(AdditionExpression* ae) = 0;
  virtual void visit(SubtractionExpression* se) = 0;
};

struct ExpressionPrinter : ExpressionVisitor
{
  ostringstream oss;
  string str() const { return oss.str(); }
  void visit(DoubleExpression* de) override;
  void visit(AdditionExpression* ae) override;
  void visit(SubtractionExpression* se) override;
};

struct ExpressionEvaluator : ExpressionVisitor
{
  double result;
  void visit(DoubleExpression* de) override;
  void visit(AdditionExpression* ae) override;
  void visit(SubtractionExpression* se) override;
};

struct Expression
{
  virtual void accept(ExpressionVisitor* visitor) = 0;
};

struct DoubleExpression : Expression
{
  double value;
  explicit DoubleExpression(const double value)
    : value{ value } {}

  void accept(ExpressionVisitor* visitor) override
  {
    visitor->visit(this);
  }
};

struct AdditionExpression : Expression
{
  Expression *left, *right;

  AdditionExpression(Expression* const left, Expression* const right)
    : left{ left }, right{ right } {}

  ~AdditionExpression()
  {
    delete left;
    delete right;
  }

  void accept(ExpressionVisitor* visitor) override
  {
    visitor->visit(this);
  }
};

struct SubtractionExpression : Expression
{
  Expression *left, *right;

  SubtractionExpression(Expression* const left, Expression* const right)
    : left{ left }, right{ right } {}

  ~SubtractionExpression()
  {
    delete left;
    delete right;
  }

  void accept(ExpressionVisitor* visitor) override
  {
    visitor->visit(this);
  }
};

void ExpressionPrinter::visit(DoubleExpression* de)
{
  oss << de->value;
}

void ExpressionPrinter::visit(AdditionExpression* e)
{
  bool need_braces = dynamic_cast<SubtractionExpression*>(e->right);
  e->left->accept(this);
  oss << "-";
  if (need_braces) oss << "(";
  e->right->accept(this);
  if (need_braces) oss << ")";
}

void ExpressionPrinter::visit(SubtractionExpression* se)
{
  bool need_braces = dynamic_cast<SubtractionExpression*>(se->right);
  if (need_braces) oss << "(";
  se->left->accept(this);
  oss << "-";
  se->right->accept(this);
  if (need_braces) oss << ")";
}

void ExpressionEvaluator::visit(DoubleExpression* de)
{
  result = de->value;
}

void ExpressionEvaluator::visit(AdditionExpression* ae)
{
  ae->left->accept(this);
  auto temp = result;
  ae->right->accept(this);
  result += temp;
}

void ExpressionEvaluator::visit(SubtractionExpression* se)
{
  se->left->accept(this);
  auto temp = result;
  se->right->accept(this);
  result = temp - result;
}

void main()
{
  auto e = new AdditionExpression{
    new DoubleExpression{ 1 },
    new SubtractionExpression {
      new DoubleExpression{ 2 },
      new DoubleExpression{ 3 }
    }
  };
  ostringstream oss;
  ExpressionPrinter printer;
  ExpressionEvaluator evaluator;
  printer.visit(e);
  evaluator.visit(e);
  cout << printer.str() << " = " << evaluator.result << endl;
  getchar();
}
