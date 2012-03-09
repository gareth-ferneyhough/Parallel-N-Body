#include <vector>
#include <iostream>
using std::cout;
using std::endl;

class Vector{
 public:
  Vector(){
    x = -9999;
    y = -9999;
  }

  Vector(double x_component, double y_component){
    x = x_component;
    y = y_component;
  }

  Vector(const Vector& p){
    x = p.x;
    y = p.y;
  }

  Vector& operator=(const Vector& v){
    x = v.x;
    y = v.y;
    return *this;
  }

  double x;
  double y;
};

class Point{
 public:
  Point(){
    cout << "c\n";
    x = -9999;
    y = -9999;
  }

  Point(double x, double y){
    cout << "d\n";
    this->x = x;
    this->y = y;
  }

  Point(const Point& p){
    cout << "cpy\n";
    x = p.x;
    y = p.y;
  }

  Point& operator=(const Point& p){
    cout << "ass\n";
    x = p.x;
    y = p.y;
    return *this;
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
  Vector total_force; // Newtons
  Vector velocity;
};

class NBodyPhysics{
 public:
  void addBody(Body b);
  void updateState(double dt);
  void saveState(std::vector<Body>&) const;
  void printState() const;
  int getNumBodies() const;

 private:
  static const double G = 6.674e-11; // check this notation!
  std::vector<Body> bodies;

  void updateForces();
  Vector calculateForce(Body &b1, Body &b2);
};
