#pragma once
#include <string>

template <typename T> struct Observer
{
  virtual void field_changed(
    T& source,               // reference to the object that changed
    const std::string& field_name // name of field (property) that changed
  ) = 0;
};


