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
  // Simulation parameters
  const int dt = 60;                           // one minute time step
  const int day_count = 1;//365 * 1;               // sim for x years
  const int runtime = day_count * 86400 / dt;  // runtime in seconds
  const int output_frequency = 120;//120;            // output state every 120 minutes
  const int num_bodies = 820; //25;             // total number of bodies
  const int bodies_per_process = 410; //5           // number of bodies each process will update

  mpi::environment env(argc, argv);
  mpi::communicator world;
  assert(world.size() == num_bodies / bodies_per_process);


  // Load initial state
  NBodyPhysics *physics = new NBodyPhysics();
  std::vector<Body> state;
  std::vector<Body> new_state;
  state.reserve(num_bodies);
  new_state.reserve(num_bodies);

  const int my_rank       = world.rank();
  const int my_body_begin = my_rank * bodies_per_process;

  std::vector<Body> saved_states;
  if (my_rank == 0){
    loadStateFromFile(state, num_bodies);
    saved_states.reserve(output_frequency / runtime);
  }

  // Broadcast initial state from root process
  broadcast(world, &(state[0]), num_bodies, 0);

  // Start timer and go.
  boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();

  // Main Loop
  for (int t = 0; t < runtime; ++t){
    physics->updateState(state, my_body_begin, bodies_per_process, dt); // update my portion
    mpi::all_gather(world, &(state[my_body_begin]), bodies_per_process, new_state); // all_gather

    state = new_state;

    // Save state and output progress if root
    if (my_rank == 0){
      if (t % output_frequency == 0){
        cout << (float)t / runtime * 100 << endl;
        saved_states.insert(saved_states.end(), state.begin(), state.end());
      }
    }
  }

  // Done. Write state if root.
  boost::chrono::duration<double> sec = boost::chrono::system_clock::now() - start;
  if (my_rank == 0){
    std::cout << "took " << sec.count() << " seconds\n";
    //saveSimToFile(saved_states, num_bodies);
  }

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
  fin.open("galaxy.tab");
  //fin.open("solar_system.csv");

  // for big galaxy file
  const float scale_factor = 1.5f;
  const float vel_factor = 8.0f;
  const float mass_factor = 120000.0f;
  const int KM = 1000;

  for (int i = 0; i < num_bodies; ++i){
    double pos_x, pos_y, pos_z;
    double dx, dy, dz;
    double mass;

    fin >> mass >> pos_x >> pos_y >> pos_z >> dx >> dy >> dz;
    Body body( Vector(pos_x *scale_factor, pos_y*scale_factor, pos_z*scale_factor),
               Vector(dx*vel_factor, dy*vel_factor, dz*vel_factor), mass*mass_factor);

    //fin >> pos_x >> pos_y >> pos_z >> dx >> dy >> dz >> mass;
    //Body body( Vector(pos_x *KM, pos_y*KM, pos_z*KM), Vector(dx*KM, dy*KM, dz*KM), mass);

    initial_state.push_back(body);
  }

  fin.close();
}
