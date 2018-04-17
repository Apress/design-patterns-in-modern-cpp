#include <gtest/gtest.h>
#include "../../../../../gtest/include/gtest/gtest.h"
#include "../../../../../gtest/include/gtest/internal/gtest-internal.h"
#include "../../../../../gtest/include/gtest/gtest_pred_impl.h"

struct Square
{
  int side{ 0 };


  explicit Square(const int side)
    : side(side)
  {
  }
};

struct Rectangle
{
  virtual int width() const = 0;
  virtual int height() const = 0;

  int area() const
  {
    return width() * height();
  }
};

struct SquareToRectangleAdapter : Rectangle
{
  SquareToRectangleAdapter(const Square& square)
    : square(square) {}
  
  int width() const override
  {
    return square.side;
  }

  int height() const override
  {
    return square.side;
  }

  const Square& square;
};

#include "gtest/gtest.h"

//#include "helpers/iohelper.h"

//#include "exercise.cpp"


namespace
{
  class Evaluate : public testing::Test
  {
  };

  TEST_F(Evaluate, SimpleTest)
  {
    Square sq{ 11 };
    SquareToRectangleAdapter adapter{ sq };
    ASSERT_EQ(121, adapter.area());
  }
}

int main(int ac, char* av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}