#include "di.hpp"
#include "headers.hpp"


struct IFoo
{
  virtual string name() = 0;
};

struct Foo : IFoo
{
  static int id;
  Foo() { ++id; }
  string name() override
  {
    return "foo "s + lexical_cast<string>(id);
  }
};

int Foo::id = 0;

struct Bar
{
  std::shared_ptr<IFoo> foo;
};

int main()
{
  auto injector = di::make_injector(
    di::bind<IFoo>().to<Foo>().in(di::singleton)
  );
  auto bar1 = injector.create<std::shared_ptr<Bar>>();
  auto bar2 = injector.create<std::shared_ptr<Bar>>();

  cout << bar1->foo->name() << endl;
  cout << bar2->foo->name() << endl;

  cout << boolalpha 
       << (bar1->foo.get() == bar2->foo.get()) 
       << endl;
}