#define BOOST_CHRONO_HEADER_ONLY

#include <boost/chrono.hpp>
#include <png++/png.hpp>
#include "n-body-physics.h"

inline void setPixel(int x, int y, int color, png::image< png::rgb_pixel > *image);
void writeImage(int image_number, std::vector<Body>& saved_states,
                std::vector<Body>::iterator start,
                std::vector<Body>::iterator end);

int imageCoordsX(double initial_x);
int imageCoordsY(double initial_y);

const int IMAGE_HEIGHT = 100;
const int IMAGE_WIDTH  = 100;
const int MIN_X = -10;
const int MAX_X = 10;
const int MAX_Y = -10;
const int MIN_Y = -10;

int main()
{
  // Setup simulation
  NBodyPhysics *physics = new NBodyPhysics();

  Body b1( Point(0, 0), 1.0);
  Body b2( Point(1, 1), 1.0);

  physics->addBody(b1);
  physics->addBody(b2);

  std::vector<Body> saved_states;

  // Run simulation
  double dt = 0.01;
  double tmax = 1;
  int num_steps = tmax / dt;

  for (int t = 0; t < num_steps; ++t){
    physics->updateState(dt);
    physics->saveState(saved_states);
  }

  // Done. Write images
  const int num_bodies = physics->getNumBodies();
  assert( saved_states.size() % num_bodies == 0);

  int image_number = 0;
  typedef std::vector<Body>::iterator vec_iter;

  for (vec_iter it = saved_states.begin(); it != saved_states.end(); it += num_bodies){
    writeImage(image_number, saved_states, it, it + num_bodies);
    image_number ++;
  }

  return 0;
}

inline void setPixel(int x, int y, int color, png::image< png::rgb_pixel > *image)
{
  (*image)[y][x] = png::rgb_pixel(color, color, color);
}

void writeImage(int image_number, std::vector<Body>& saved_states,
                std::vector<Body>::iterator start,
                std::vector<Body>::iterator end)
{
  // Create new png
  png::image< png::rgb_pixel > image(IMAGE_WIDTH, IMAGE_HEIGHT);

  // Set each body as a white pixel
  std::vector<Body>::iterator it = start;
  while (it != end){
    double x = start->position.x;
    double y = start->position.y;
    double mass = start->mass;

    setPixel( imageCoordsX(x), imageCoordsY(y), 255, &image);
    it++;
  }

  // Create filename
  std::string s;
  std::stringstream out;
  out << image_number;
  s = out.str();

  std::string filename = "images/" + s + ".png";

  // Save image
  image.write(filename);
}

int imageCoordsX(double initial_x)
{
  return static_cast<int> (initial_x);
}

int imageCoordsY(double initial_y)
{
  return static_cast<int> (initial_y);
}
