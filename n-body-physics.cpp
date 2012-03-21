#include <algorithm>
#include <iostream>
#include <ostream>
#include "n-body-physics.h"

void NBodyPhysics::addBody(Body b)
{
  bodies.push_back(b);
}

void NBodyPhysics::updateState(double dt)
{
  updateForces();

  // Update position and velocity
  for(unsigned int i = 0; i < bodies.size(); ++i){
    Vector last_velocity = bodies[i].velocity;

    bodies[i].velocity += (bodies[i].total_force * dt) / bodies[i].mass;
    bodies[i].position += (last_velocity * dt);
  }
}

void NBodyPhysics::updateState(std::vector<Body>& state, const int my_index_begin, const int num_to_update, const double dt)
{
  updateForces(state, my_index_begin, num_to_update);

  // Update position and velocity
  for(unsigned int i = my_index_begin; i < my_index_begin + num_to_update; ++i){
    Vector last_velocity = bodies[i].velocity;

    state[i].velocity += (state[i].total_force * dt) / state[i].mass;
    state[i].position += (last_velocity * dt);
  }  
}

void NBodyPhysics::saveState(std::vector<Body>& state) const
{
  std::copy(bodies.begin(), bodies.end(), std::back_inserter (state));
}

void NBodyPhysics::printState() const
{
  for(unsigned int i = 0; i < bodies.size(); ++i){
    std::cout << "Body " << i << " Position: " << bodies[i].position.x << ", " << bodies[i].position.y << ", " << bodies[i].position.z << " \nVelocity: " << bodies[i].velocity.x << ", " << bodies[i].velocity.y << ", " << bodies[i].velocity.z << std::endl << std::endl;
  }
}

int NBodyPhysics::getNumBodies() const
{
  return bodies.size();
}

void NBodyPhysics::updateForces()
{
  for(unsigned int i = 0; i < bodies.size(); ++i){
    bodies[i].total_force = Vector(0., 0., 0.);

    for(int unsigned j = 0; j < bodies.size(); j++){
      // maybe remove this if statement //
      if (i != j){
        Vector force = calculateForce(bodies[i], bodies[j]);
        bodies[i].total_force += force;
      }
    }
  }
}

void NBodyPhysics::updateForces(std::vector<Body>& state, const int index_begin, const int num_to_update)
{
  for(unsigned int i = index_begin; i < index_begin + num_to_update; ++i){
    state[i].total_force = Vector(0., 0., 0.);

    for(int unsigned j = 0; j < state.size(); j++){
      // maybe remove this if statement //
      if (i != j){
        Vector force = calculateForce(state[i], state[j]);
        state[i].total_force += force;
      }
    }
  }
}

Vector NBodyPhysics::calculateForce(Body& b1, Body& b2)
{
  double top_of_equation = G * b1.mass * b2.mass;

  double magnitude = Vector(b2.position - b1.position).magnitude();
  if (magnitude == 0.)
    return Vector(0., 0., 0.);

  Vector direction = Vector(b2.position - b1.position) / magnitude;
  return Vector((direction * top_of_equation) / (magnitude*magnitude));
}

std::ostream& operator<<(std::ostream& os, const Vector& v)
{
  os << "[" << v.x << "," << v.y << "," << v.z << "]" << endl;
  return os;
}
