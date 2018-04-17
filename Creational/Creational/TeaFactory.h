#pragma once
#include "HotDrinkFactory.h"

struct TeaFactory : HotDrinkFactory
{
  unique_ptr<HotDrink> make() const override {
    return make_unique<Tea>();
  }
};