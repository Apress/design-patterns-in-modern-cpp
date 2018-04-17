#pragma once
#include <memory>
#include "HotDrink.h"
#include "HotDrinkFactory.h"

struct HotDrink;

struct CoffeeFactory : HotDrinkFactory
{
  unique_ptr<HotDrink> make() const override
  {
    return make_unique<Coffee>();
  }
};