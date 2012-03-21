#define BOOST_CHRONO_HEADER_ONLY

#include <fstream>
#include <boost/chrono.hpp>
#include "n-body-physics.h"

void writePosition(std::vector<Body>& saved_states,
                   std::vector<Body>::iterator start,
                   std::vector<Body>::iterator end,
                   std::ostream& fout);

int main()
{
  // Setup simulation
  NBodyPhysics *physics = new NBodyPhysics();

  // Read state file
  std::ifstream fin;
  fin.open("solar_system.csv");

  for (int i = 0; i < 25; ++i){
    double pos_x, pos_y, pos_z;
    double dx, dy, dz;
    double mass;

    fin >> pos_x >> pos_y >> pos_z >> dx >> dy >> dz >> mass;
    const int KM = 1000;
    Body b1( Vector(pos_x *KM, pos_y*KM, pos_z*KM), Vector(dx*KM, dy*KM, dz*KM), mass);

    physics->addBody(b1);
  }
  fin.close();

  // Run simulation
  const int dt = 60;                            // one minute time step
  const int day_count = 365 * 1;                // run for x years
  const int runtime = day_count * 86400 / dt;   // runtime in seconds
  const int output_frequency = 120;             // save positions every two minutes

  std::vector<Body> saved_states;
  saved_states.reserve(output_frequency / runtime);

  // Main loop
  for (int t = 0; t < runtime; ++t){
    physics->updateState(dt);

    if (t % output_frequency == 0){
      cout << (float)t / runtime * 100 << endl;
      physics->saveState(saved_states);
    }
  }

  // Done. Write positions.
  const int num_bodies = physics->getNumBodies();
  assert( saved_states.size() % num_bodies == 0);

  std::ofstream fout;
  fout.open("data.bin", std::ios::out | std::ios::binary);

  typedef std::vector<Body>::iterator vec_iter;

  for (vec_iter it = saved_states.begin(); it < saved_states.end(); it += num_bodies){
    writePosition(saved_states, it, it + num_bodies, fout);
  }

  fout.close();
  return 0;
}

void writePosition(std::vector<Body>& saved_states,
                   std::vector<Body>::iterator start,
                   std::vector<Body>::iterator end,
                   std::ostream& fout)
{
  std::vector<Body>::iterator it = start;
  while (it != end){
    double x = it->position.x;
    double y = it->position.y;
    double z = it->position.z;

    fout.write(reinterpret_cast<char*>(&x), sizeof(x));
    fout.write(reinterpret_cast<char*>(&y), sizeof(y));
    fout.write(reinterpret_cast<char*>(&z), sizeof(z));

    it++;
  }
}
