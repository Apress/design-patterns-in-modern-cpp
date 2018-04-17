#include "Person.h"
#include "PersonBuilder.h"

PersonBuilder Person::create()
{
  return PersonBuilder{};
}