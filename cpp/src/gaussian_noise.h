#pragma one

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <math.h>

// constexpr size_t GN_BUFFER_SIZE = 1000;

typedef struct {
  float x,y,z;
} sample_t;

typedef struct {
  float x, y;
} sample_2d_t;

class GaussianNoise {
protected:
  std::mt19937 _generator;
  // std::uniform_real_distribution<float> _distribution;
  std::normal_distribution<float> _distribution;

public:
  GaussianNoise(float stddev = 0.001, uint32_t seed = 0)
        : _generator(seed)
        , _distribution(0.0, stddev) {}
};