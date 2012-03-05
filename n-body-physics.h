#include <vector>

class Vector{
 public:
  explicit Vector(double x_component, double y_component){
    x = x_component;
    y = y_component;
  }
  double x;
  double y;
};

class Point{
 public:
  explicit Point(double x, double y){
    this->x = x;
    this->y = y;
  }
  double x;
  double y;
};

class Body{
 public:
  explicit Body(Point position, double mass){
    this->position = position, this->mass = mass;
    total_force = Vector(0, 0);
    velocity = Vector(0, 0);
  }
  Point position; // Meters
  double mass;    // Kg
  //std::vector<Vector> forces;
  Vector total_force; // Newtons
  Vector velocity;
};

class NBodyPhysics{
 public:
  void addBody(Body b);
  void updateState(double dt);
  void printState();

 private:
  static const double G = 6.674e-11; // check this notation!
  std::vector<Body> bodies;

  void updateForces();
  Vector calculateForce(Body b1, Body b2);
};
