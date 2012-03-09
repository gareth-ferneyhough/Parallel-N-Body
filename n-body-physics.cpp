#include <algorithm>
#include <iostream>
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

    bodies[i].velocity.x += (dt * bodies[i].total_force.x) / bodies[i].mass;
    bodies[i].velocity.y += (dt * bodies[i].total_force.y) / bodies[i].mass;

    bodies[i].position.x += (dt * last_velocity.x);
    bodies[i].position.y += (dt * last_velocity.y);
  }
}

void NBodyPhysics::saveState(std::vector<Body>& state) const
{
  std::copy(bodies.begin(), bodies.end(), std::back_inserter (state));
}

void NBodyPhysics::printState() const
{
  for(unsigned int i = 0; i < bodies.size(); ++i){
    std::cout << "Body " << i << " Position: " << bodies[i].position.x << "," << bodies[i].position.y << " \nVelocity: " << bodies[i].velocity.x << "," << bodies[i].velocity.y << std::endl << std::endl;
  }
}

int NBodyPhysics::getNumBodies() const
{
  return bodies.size();
}

void NBodyPhysics::updateForces()
{
  for(unsigned int i = 0; i < bodies.size(); ++i){
    bodies[i].total_force.x = 0;
    bodies[i].total_force.y = 0;

    for(int unsigned j = 0; j < bodies.size(); j++){
      Vector force = calculateForce(bodies[i], bodies[j]);
      bodies[i].total_force.x += force.x;
      bodies[i].total_force.y += force.y;
    }
  }
}

Vector NBodyPhysics::calculateForce(Body& b1, Body& b2)
{
  Vector force;
  double top_of_equation = G * b1.mass * b2.mass;
  double x_distance = b2.position.x - b1.position.x; // these may be negative, but we square it below
  double y_distance = b2.position.y - b1.position.y;

  if (x_distance == 0)
    force.x = 0;
  else force.x = top_of_equation / (x_distance * x_distance);

  if (y_distance == 0)
    force.y = 0;
  else force.y = top_of_equation / (y_distance * y_distance);

  // Calculate force in the direction that b1 is experiencing
  //       |
  // -1, 1 | 1, 1
  // ------------
  // -1,-1 | 1,-1
  //       |

  if (b1.position.x > b2.position.x) //b1 is to the right of b2, so x-component should be negative
    force.x *= -1;

  if (b1.position.y > b2.position.y)
    force.y *= -1;

  return force;
}
