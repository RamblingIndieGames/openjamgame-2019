#include <stdlib.h>
#include "random.h"

unsigned long long rng_state[2] = { 1, 2 };

unsigned rng_seed(int n, int n2) {
  if (n) {
    rng_state[0] = n;
  } else {
    rng_state[0] = rand() * 65535 | 0;
  }
  if (n2) {
    rng_state[1] = n2;
  } else {
    rng_state[1] = (unsigned long long)(rand() * 65535 | 0) >> 32;
  }
  return 0;
}

unsigned rng_next() {
  unsigned long s0 = rng_state[1];
  rng_state[0] = s0;
  unsigned long s1 = rng_state[0];
  s1 ^= s1 << 23;
  s1 ^= s1 >> 17;
  s1 ^= s0;
  s1 ^= s0 >> 26;
  rng_state[1] = s1;
  return rng_state[0] + rng_state[1];
}

unsigned rng_random(int low, int high) {
  return low + (rng_next() % (high - low));
}
