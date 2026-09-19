#include <iostream>
#include <random>
#include <pthread.h>



size_t calc(double r, size_t tests, size_t seed) {
  std::mt19937 gen(seed);
  std::uniform_real_distribution<double> dist(-r, r);
  
  size_t inside = 0;
  for (size_t i = 0; i < tests; ++i) {
    double x = dist(gen);
    double y = dist(gen);
    if (isInside(x, y, r)) ++inside;
  }

  return inside;
}



bool isInside(double x, double y, double r) {
  if (x * x + y * y <= r * r) {
    return true;
  }

  return false;
}

int main() {

}