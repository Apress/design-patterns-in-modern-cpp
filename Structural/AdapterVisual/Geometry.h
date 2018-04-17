#pragma once
#include <vector>
#include <boost/functional/hash.hpp>

struct Point
{
  int x, y;


  friend std::size_t hash_value(const Point& obj)
  {
    std::size_t seed = 0x725C686F;
    boost::hash_combine(seed, obj.x);
    boost::hash_combine(seed, obj.y);
    return seed;
  }
};

struct Line
{
  Point start, end;


  friend std::size_t hash_value(const Line& obj)
  {
    std::size_t seed = 0x719E6B16;
    boost::hash_combine(seed, obj.start);
    boost::hash_combine(seed, obj.end);
    return seed;
  }
};

struct VectorObject
{
  virtual std::vector<Line>::iterator begin() = 0;
  virtual std::vector<Line>::iterator end() = 0;
};

struct VectorRectangle : VectorObject
{
  VectorRectangle(int x, int y, int width, int height)
  {
    lines.emplace_back(Line{ Point{x,y}, Point{x + width,y} });
    lines.emplace_back(Line{ Point{x + width,y}, Point{x+width, y+height} });
    lines.emplace_back(Line{ Point{x,y}, Point{x,y+height} });
    lines.emplace_back(Line{ Point{ x,y + height }, Point{ x + width, y + height } });
  }

  std::vector<Line>::iterator begin() override {
    return lines.begin();
  }
  std::vector<Line>::iterator end() override {
    return lines.end();
  }
private:
  std::vector<Line> lines;
};