struct Point
{
  int x{ 0 }, y{ 0 };

  Point(){}
  
  Point(const int x, const int y) : x{x}, y{y} {}
};

struct Line
{
  Point *start, *end;
  
  Line(Point* const start, Point* const end)
    : start(start), end(end)
  {
  }

  ~Line()
  {
    delete start;
    delete end;
  }

  Line deep_copy() const
  {
    return Line{
      new Point(start->x, start->y),
      new Point(end->x, end->y)
    };
  }
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
    Line line1{
      new Point{3,3},
      new Point{10,10}
    };

    auto line2 = line1.deep_copy();
    line1.start->x = line1.start->y =
      line1.end->y = line1.end->y = 0;

    ASSERT_EQ(3, line2.start->x);
    ASSERT_EQ(3, line2.start->y);
    ASSERT_EQ(10, line2.end->x);
    ASSERT_EQ(10, line2.end->y);
  }
} // namespace

int main(int ac, char* av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}