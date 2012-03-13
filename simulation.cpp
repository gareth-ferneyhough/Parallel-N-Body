#define BOOST_CHRONO_HEADER_ONLY

#include <iomanip>
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

const int IMAGE_HEIGHT = 1000;
const int IMAGE_WIDTH  = 1000;
const int MIN_X = -6E8;
const int MAX_X =  6E8;
const int MAX_Y =  6E8;
const int MIN_Y = -6E8;

int main()
{
  // Setup simulation
  NBodyPhysics *physics = new NBodyPhysics();

  Body b1( Vector(0, 0), 6E14); //6E24
  Body b2( Vector(406700000, 0), 7.4E12); //7.4E22
  b2.velocity = Vector(0, -5000000);

  physics->addBody(b1);
  physics->addBody(b2);

  std::vector<Body> saved_states;

  // Run simulation
  double dt = 1;
  double tmax = 1E4;
  int num_steps = tmax / dt;

  for (int t = 0; t < num_steps; ++t){
    physics->printState();
    physics->updateState(dt);
    physics->saveState(saved_states);
  }

  // Done. Write images
  const int num_bodies = physics->getNumBodies();
  assert( saved_states.size() % num_bodies == 0);

  int image_number = 0;
  typedef std::vector<Body>::iterator vec_iter;

  for (vec_iter it = saved_states.begin(); it < saved_states.end(); it += (num_bodies)*10){
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
  for(int x = center_x - radius; x <= center_x + radius; x++)
    for(int y = center_y - radius; y <= center_y + radius; y++){
      if (( (x - center_x) * (x - center_x) + (y - center_y) * (y - center_y)) < (radius * radius)) 
           setPixel(x, y, 255, image);
    }
}
