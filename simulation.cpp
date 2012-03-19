#define BOOST_CHRONO_HEADER_ONLY

#include <iomanip>
#include <fstream>
#include <sstream>
#include <boost/chrono.hpp>
#include <png++/png.hpp>
#include "n-body-physics.h"

inline void setPixel(int x, int y, int color, png::image< png::rgb_pixel > *image);
void writeImage(int image_number, std::vector<Body>& saved_states,
                std::vector<Body>::iterator start,
                std::vector<Body>::iterator end);

int imageCoordsX(double initial_x);
int imageCoordsY(double initial_y);
void drawCircle(int center_x, int center_y, int radius, int color, png::image< png::rgb_pixel > *image);

const int IMAGE_HEIGHT = 10000;
const int IMAGE_WIDTH  = 10000;
const double MIN_X = -3E12;
const double MAX_X =  3E12;
const double MIN_Y = -3E12;
const double MAX_Y =  3E12;

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
  physics->printState();

  std::vector<Body> saved_states;

  // // Run simulation
  int dt = 60;                // one minute time step
  int day_count = 365 * 0.5;    // sim for 1 year
  int runtime = day_count * 86400 / dt;

  for (int t = 0; t < 1; ++t){
    if (t % 1000 == 0)
      cout << (float)t / runtime * 100 << endl;

    physics->updateState(dt);
    physics->saveState(saved_states);
    physics->printState();
  }

  // Done. Write images at specified interval
  const int output_frequency = 1;//1 * 86400 /dt; // output images once per day
  const int num_bodies = physics->getNumBodies();
  assert( saved_states.size() % num_bodies == 0);

  int image_number = 0;
  typedef std::vector<Body>::iterator vec_iter;

  for (vec_iter it = saved_states.begin(); it < saved_states.end(); it += (num_bodies)*output_frequency){
    writeImage(image_number, saved_states, it, it + num_bodies);
    image_number ++;
  }

  return 0;
}

inline void setPixel(int x, int y, int color, png::image< png::rgb_pixel > *image)
{
  if ((x >= 0 && x < IMAGE_WIDTH) && (y >= 0 && y < IMAGE_HEIGHT)){
    (*image)[y][x] = png::rgb_pixel(color, color, color);
  }
}

void writeImage(int image_number, std::vector<Body>& saved_states,
                std::vector<Body>::iterator start,
                std::vector<Body>::iterator end)
{
  // Create new png
  png::image< png::rgb_pixel > image(IMAGE_WIDTH, IMAGE_HEIGHT);

  // Set each body as a white circle
  std::vector<Body>::iterator it = start;
  while (it != end){
    double x = it->position.x;
    double y = it->position.y;
    double mass = start->mass;

    drawCircle( imageCoordsX(x), imageCoordsY(y), 10, 255, &image);
    it++;
  }

  // Create filename
  std::string s;
  std::stringstream out;
  out << std::setfill('0') << std::setw(4) << image_number;
  s = out.str();

  cout << "saving image\n";
  std::string filename = "images/" + s + ".png";

  // Save image
  image.write(filename);
}

int imageCoordsX(double initial_x)
{
  const double SCALE_X = double(MAX_X - MIN_X) / IMAGE_WIDTH;
  return static_cast<int> (initial_x - MIN_X) / SCALE_X;
}

int imageCoordsY(double initial_y)
{
  const double SCALE_Y = double(MAX_Y - MIN_Y) / IMAGE_HEIGHT;
  return static_cast<int> (initial_y - MIN_Y) / SCALE_Y;
}

void drawCircle(int center_x, int center_y, int radius, int color, png::image< png::rgb_pixel > *image)
{
  std::cout << "center: " << center_x << "," << center_y << endl;

  for(int x = center_x - radius; x <= center_x + radius; x++)
    for(int y = center_y - radius; y <= center_y + radius; y++){
      if (( (x - center_x) * (x - center_x) + (y - center_y) * (y - center_y)) < (radius * radius))
        setPixel(x, y, 255, image);
    }
}
