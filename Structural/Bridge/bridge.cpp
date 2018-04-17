#include <iostream>
#include <string>
#include <vector>
#include "Person.h"
using namespace std;

// two classes of objects

// Renderers - determine how an object is drawn
// Shapes - determine what to draw

struct Renderer
{
  virtual void render_circle(float x, float y, float radius) = 0;
};

struct VectorRenderer : Renderer
{
  void render_circle(float x, float y, float radius) override
  {
    cout << "Rasterizing circle of radius " << radius << endl;
  }
};

struct RasterRenderer : Renderer
{
  void render_circle(float x, float y, float radius) override
  {
    cout << "Drawing a vector circle of radius " << radius << endl;
  }
};

struct Shape
{
protected:
  Renderer& renderer;
  Shape(Renderer& renderer) : renderer{ renderer } {}
public:
  virtual void draw() = 0; // implementation specific
  virtual void resize(float factor) = 0; // abstraction specific
};

struct Circle : Shape
{
  float x, y, radius;

  void draw() override
  {
    renderer.render_circle(x, y, radius);
  }

  void resize(float factor) override
  {
    radius *= factor;
  }

  Circle(Renderer& renderer, float x, float y, float radius)
    : Shape{renderer},
      x{x},
      y{y},
      radius{radius}
  {
  }
};

void bridge()
{
  RasterRenderer rr;
  Circle raster_circle{ rr, 5,5,5 };
  raster_circle.draw();
  raster_circle.resize(2);
  raster_circle.draw();
}

int main()
{
  

  Person p;
  p.greet();

  getchar();
  return 0;
}
