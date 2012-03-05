#include "n-body-physics.h"

int main()
{
  // Setup simulation
  NBodyPhysics *physics = new NBodyPhysics();

  Body b1( Point(0, 0), 1.0);
  Body b2( Point(1, 1), 1.0);

  physics->addBody(b1);
  physics->addBody(b2);
  
  // Run simulation
  double dt = 0.01;
  double t_max = 10;
  int num_steps = tmax / dt;

  for (int t = 0; t < num_steps; ++t){
    physics->updateState(dt);
    physics->printState();
  }

  return 0;
}
