#include <vector>
#include <iostream>
#include <ostream>
#include <cmath>

#include <boost/serialization/serialization.hpp>
using std::cout;
using std::endl;

class Vector{
 public:
  Vector(){
    x = 0;
    y = 0;
    z = 0;
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

 private:
  friend std::ostream& operator<<(std::ostream& os, const Vector& v);
  friend class boost::serialization::access;

  template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      ar & x;
      ar & y;
      ar & z;
    }
};

class Body{
 public:
  Body() {};
  explicit Body(Vector position, Vector velocity, double mass){
    this->position = position, this->velocity = velocity, this->mass = mass;
    total_force = Vector(0, 0, 0);
  }
  Vector position;      // Meters
  Vector velocity;      // Meters / second
  Vector total_force;   // Newtons
  double mass;          // Kg

 private:
  friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & position;
        ar & mass;
        ar & velocity;
        //ar & total_force; // dont send force. It is pointless.
    }
};

class NBodyPhysics{
 public:
  void addBody(Body b);
  void updateState(std::vector<Body>& state, const int index_begin, const int num_to_update, const double dt);
  void updateState(double dt);
  void saveState(std::vector<Body>&) const;
  void printState() const;
  int getNumBodies() const;
  void setNewState(std::vector<Body>& state);


 private:
  static const double G = 6.674e-11;
  std::vector<Body> bodies;

  void updateForces();
  void updateForces(std::vector<Body>& state, const int index_begin, const int num_to_update);
  Vector calculateForce(Body &b1, Body &b2);
};
