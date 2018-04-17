#include "Headers.hpp"

struct AddressPrinter
{
  void operator()(const string& house_name) const {
    cout << "A house called " << house_name << "\n";
  }

  void operator()(const int house_number) const {
    cout << "House number " << house_number << "\n";
  }
};

int main(int ac, char* av[])
{
  variant<string, int> house;
  //house = "Montefiore Castle";
  house = 221;

  AddressPrinter ap;
  std::visit(ap, house);

  // what if you want to create a visitor in-place?
  std::visit([](auto& arg) {
    using T = decay_t<decltype(arg)>;

    if constexpr (is_same_v<T, string>)
    {
      cout << "A house called " << arg.c_str() << "\n";
    }
    else
    {
      cout << "House number " << arg << "\n";
    }
  }, house);

  return 0;
}