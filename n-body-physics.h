#include <vector>
#include <iostream>
#include <ostream>
#include <cmath>

using std::cout;
using std::endl;

class Vector{
 public:
  Vector(){
    x = -9999;
    y = -9999;
    z = -9999;
  }

  Vector(double x_component, double y_component, double z_component){
    x = x_component;
    y = y_component;
    z = z_component;
  }

  Vector(const Vector& p){
    x = p.x;
    y = p.y;
    z = p.z;
  }

  Vector& operator=(const Vector& v){
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
  }

  double magnitude(){
    return sqrt(x*x + y*y + z*z);
  }

  Vector operator-(const Vector& rhs){
    return Vector(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
  }

  Vector operator+(const Vector& rhs){
    return Vector(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);
  }

  Vector operator+=(const Vector& rhs){
    return Vector(this->x += rhs.x, this->y += rhs.y, this->z += rhs.z);
  }

  Vector operator*(const double& d){
    return Vector(this->x *d, this->y *d, this->z *d);
  }

  Vector operator/(const double& d){
    return Vector(this->x / d, this->y / d, this->z / d);
  }

  double x;
  double y;
  double z;
  friend std::ostream& operator<<(std::ostream& os, const Vector& v);
};

class Body{
 public:
  explicit Body(Vector position, Vector velocity, double mass){
    this->position = position, this->velocity = velocity, this->mass = mass;
    total_force = Vector(0, 0, 0);
  }
  Vector position; // Meters
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
