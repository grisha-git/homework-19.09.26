#include <iostream>
#include <random>
#include <pthread.h>
#include <vector>
#include <cstring>

struct calcStruct {
  double r;
  size_t tests;
  size_t seed;
};

size_t calc(double r, size_t tests, size_t seed);
void* calcThread(void* data);
bool isInside(double x, double y, double r);

double area(double r, size_t threads, size_t tests) {
  size_t inside = 0;
  size_t remainder = tests % threads;
  std::vector< pthread_t > threadsArr(threads);
  std::vector< calcStruct > parametrs(threads);

  for (size_t i = 0; i < threads; ++i) {
    if (i == threads - 1) {
      parametrs[i] = {r, (tests / threads) + (tests % threads), i};
    } else {
      parametrs[i] = {r, tests / threads, i};
    }
    
    int err = pthread_create(&threadsArr[i], nullptr, calcThread, &parametrs[i]);
    if (err) {
      std::cerr << "create error\n";
      return -1;
    }
  }

  for (size_t i = 0; i < threads; ++i) {
    size_t result[1] = {};
    int err = pthread_join(threadsArr[i], reinterpret_cast< void** >(&result));
    if (err) {
      std::cerr << "join error\n";
      return -1;
    }

    inside += result[0];
  }

  double ratio = static_cast< double >(inside) / tests;
  return ratio * 4.0 * r * r; 
}

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

void* calcThread(void* data) {
  calcStruct* pod = static_cast< calcStruct* >(data);
  size_t res = calc(pod->r, pod->tests, pod->seed);

  return reinterpret_cast< void* >(res);
}

bool isInside(double x, double y, double r) {
  if (x * x + y * y <= r * r) {
    return true;
  }

  return false;
}

int main() {
  double radius;
  size_t tests;

  std::cin >> radius >> tests;
  double circleArea = area(radius, 4, tests);
  std::cout << circleArea << '\n';
}