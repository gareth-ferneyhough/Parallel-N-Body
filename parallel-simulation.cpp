#define BOOST_CHRONO_HEADER_ONLY

#include <fstream>
#include <boost/chrono.hpp>
#include <boost/mpi.hpp>
#include <boost/mpi/collectives.hpp>
#include "n-body-physics.h"

namespace mpi = boost::mpi;

void saveSimToFile(const std::vector<Body>& saved_states, const int num_bodies);
void loadStateFromFile(std::vector<Body>& initial_state, const int num_bodies);
void writePosition(const std::vector<Body>& saved_states,
                   std::vector<Body>::const_iterator start,
                   std::vector<Body>::const_iterator end,
                   std::ostream& fout);

int main(int argc, char* argv[])
{
  mpi::environment env(argc, argv);
  mpi::communicator world;


  // Simulation parameters
  const int dt = 60;                           // one minute time step
  const int day_count = 365 * 2;               // sim for x years
  const int runtime = day_count * 86400 / dt;  // runtime in seconds
  const int output_frequency = 120;            // output state every two minutes
  const int bodies_per_process = 2;            // number of bodies each process will update
  const int num_bodies = 24;                   // total number of bodies

  std::vector<Body> saved_states;
  saved_states.reserve(output_frequency / runtime);

  // Load initial state
  NBodyPhysics *physics = new NBodyPhysics();
  std::vector<Body> state;
  std::vector<Body> new_state;
  state.reserve(num_bodies);
  new_state.reserve(num_bodies);

  const int my_rank       = world.rank();
  const int my_body_begin = my_rank * bodies_per_process;
 
  if (my_rank == 0) loadStateFromFile(state, num_bodies);

  // Broadcast initial state from root process
  broadcast(world, &(state[0]), num_bodies, 0);
  
  // Main Loop
  for (int t = 0; t < runtime; ++t){
    physics->updateState(state, my_body_begin, bodies_per_process, dt);             // update the portion I am responsible for
    mpi::all_gather(world, &(state[my_body_begin]), bodies_per_process, new_state); // all_gather
    state = new_state;

    // Save state and output progress if root
    if (my_rank == 0){
      if (t % output_frequency == 0){
        cout << (float)t / runtime * 100 << endl;
        physics->saveState(saved_states);
      }
    }
  }

  // Done. Write state if root.
  if (my_rank == 0) saveSimToFile(saved_states, num_bodies);

  return 0;
}

void writePosition(const std::vector<Body>& saved_states,
                   std::vector<Body>::const_iterator start,
                   std::vector<Body>::const_iterator end,
                   std::ostream& fout)
{
  std::vector<Body>::const_iterator it = start;
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

void saveSimToFile(const std::vector<Body>& saved_states, const int num_bodies)
{
  // Done. Write state at specified interval
  assert( saved_states.size() % num_bodies == 0);

  std::ofstream fout;
  fout.open("data.bin", std::ios::out | std::ios::binary);

  typedef std::vector<Body>::const_iterator vec_iter;
  for (vec_iter it = saved_states.begin(); it < saved_states.end(); ++it){
    writePosition(saved_states, it, it + num_bodies, fout);
  }

  fout.close();
}

void loadStateFromFile(std::vector<Body>& initial_state, const int num_bodies)
{
  // Read state file
  std::ifstream fin;
  fin.open("solar_system.csv");

  for (int i = 0; i < num_bodies; ++i){
    double pos_x, pos_y, pos_z;
    double dx, dy, dz;
    double mass;

    fin >> pos_x >> pos_y >> pos_z >> dx >> dy >> dz >> mass;
    const int KM = 1000;
    Body body( Vector(pos_x *KM, pos_y*KM, pos_z*KM), Vector(dx*KM, dy*KM, dz*KM), mass);

    initial_state.push_back(body);
  }

  fin.close();
}



// int main(int argc, char* argv[])
// {
//   mpi::environment env(argc, argv);
//   mpi::communicator world;

//   if (world.rank() == 0) {
//     std::vector<Body> bodies;
//     std::vector<Body> bodies_new;
//     Body b1( Vector(1.4, 2.4, 3.4), Vector(4, 5, 6), 10);
//     Body b2( Vector(1.2, 2.2, 3.2), Vector(4.2, 5.2, 6.2), 10.2);

//     Body bb[2];
//     bb[0] = b1;
//     bb[1] = b2;

//     bodies.push_back(b1);
//     bodies.push_back(b2);

//     mpi::all_gather(world, &(bodies[0]), 2, bodies_new);
//   }

//   else {
//     std::vector<Body> bodies;
//     std::vector<Body> bodies_new;
//     Body b1( Vector(1.9, 2.9, 3.9), Vector(4, 5, 6), 10);
//     Body b2( Vector(1.23, 2.23, 3.23), Vector(4.23, 5.23, 6.23), 10.23);

//     Body bb[2];
//     bb[0] = b1;
//     bb[1] = b2;

//     bodies.push_back(b1);
//     bodies.push_back(b2);

//     mpi::all_gather(world, &(bodies[0]), 2, bodies_new);

//     std::vector<Body>::iterator it;
//     for (it = bodies_new.begin(); it != bodies_new.end(); ++it){
//       cout << "\nbody: ";
//       cout << it->position << it->velocity << it->total_force << it->mass << endl;
//     }
//   }

//   return 0;
// }
