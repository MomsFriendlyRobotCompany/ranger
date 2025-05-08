#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generateGaussianArray(float *array, int size, float stddev) {
  // Seed the random number generator
  srand((unsigned int)time(NULL));

  for (int i = 0; i < size; i += 2) {
    // Generate two uniform random numbers between 0 and 1
    float u1 = (float)rand() / RAND_MAX;
    float u2 = (float)rand() / RAND_MAX;

    // Box-Muller transform
    float z0 = sqrt(-2.0f * log(u1)) * cos(2.0f * M_PI * u2);
    float z1 = sqrt(-2.0f * log(u1)) * sin(2.0f * M_PI * u2);

    // Scale by standard deviation
    array[i] = z0 * stddev;

    // Store second value if within array bounds
    if (i + 1 < size) {
      array[i + 1] = z1 * stddev;
    }
  }
}

int main() {
  int size     = 1000;
  float stddev = 1.0f; // Set your desired standard deviation
  float array[1000];

  generateGaussianArray(array, size, stddev);

  // Optional: Print first few values to verify
  for (int i = 0; i < 100; i++) {
    printf("%f\n", array[i]);
  }

  return 0;
}