#define BOOST_CHRONO_HEADER_ONLY

#include <fstream>
#include <boost/chrono.hpp>
#include <boost/mpi.hpp>
#include "n-body-physics.h"

namespace mpi = boost::mpi;

void writePosition(std::vector<Body>& saved_states,
                   std::vector<Body>::iterator start,
                   std::vector<Body>::iterator end,
                   std::ostream& fout);

int main(int argc, char* argv[])
{
  mpi::environment env(argc, argv);
  mpi::communicator world;

  if (world.rank() == 0) {
    Body b( Vector(1, 2, 3), Vector(4, 5, 6), 10);
    world.send(1, 0, b);
  }

  else {
    Body b2;
    world.recv(0, 0, b2);

    cout << b2.position << b2.velocity << b2.total_force << b2.mass << endl;
  }

  return 0;
}

// int main()
// {
//   // Setup simulation
//   NBodyPhysics *physics = new NBodyPhysics();

//   // Read state file
//   std::ifstream fin;
//   fin.open("solar_system.csv");

//   for (int i = 0; i < 25; ++i){
//     double pos_x, pos_y, pos_z;
//     double dx, dy, dz;
//     double mass;

//     fin >> pos_x >> pos_y >> pos_z >> dx >> dy >> dz >> mass;
//     const int KM = 1000;
//     Body b1( Vector(pos_x *KM, pos_y*KM, pos_z*KM), Vector(dx*KM, dy*KM, dz*KM), mass);

//     physics->addBody(b1);
//   }

//   fin.close();
//   physics->printState();

//   std::vector<Body> saved_states;

//   // // Run simulation
//   const int dt = 60;                // one minute time step
//   const int day_count = 365 * 2;    // sim for 1 year
//   const int runtime = day_count * 86400 / dt;
//   const int output_frequency = 1 * 86400 /dt; // output images once per day

//   for (int t = 0; t < runtime; ++t){
//     physics->updateState(dt);

//     if (t % output_frequency == 0){
//       cout << (float)t / runtime * 100 << endl;
//       physics->saveState(saved_states);
//     }
//   }

//   // Done. Write images at specified interval
//   const int num_bodies = physics->getNumBodies();
//   assert( saved_states.size() % num_bodies == 0);

//   std::ofstream fout;
//   fout.open("data.bin", std::ios::out | std::ios::binary);

//   typedef std::vector<Body>::iterator vec_iter;

//   for (vec_iter it = saved_states.begin(); it < saved_states.end(); it += (num_bodies)*output_frequency){
//     writePosition(saved_states, it, it + num_bodies, fout);
//   }

//   fout.close();
//   return 0;
// }

// void writePosition(std::vector<Body>& saved_states,
//                    std::vector<Body>::iterator start,
//                    std::vector<Body>::iterator end,
//                    std::ostream& fout)
// {
//   std::vector<Body>::iterator it = start;
//   while (it != end){
//     double x = it->position.x;
//     double y = it->position.y;
//     double z = it->position.z;

//     fout.write(reinterpret_cast<char*>(&x), sizeof(x));
//     fout.write(reinterpret_cast<char*>(&y), sizeof(y));
//     fout.write(reinterpret_cast<char*>(&z), sizeof(z));

//     it++;
//   }
// }
