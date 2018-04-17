#pragma once
#include "HotDrink.h"

struct HotDrinkFactory
{
  virtual unique_ptr<HotDrink> make() const = 0;
};