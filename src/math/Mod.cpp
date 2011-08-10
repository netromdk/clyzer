#include <cmath>

#include "Mod.h"

int mod(int n, int m) {
  // The % operator doesn't work on negative numbers so do the
  // operation here.
  if (n < 0) { 
    return n + ceil((double) (n * -1.0) / m) * m;
  }
    
  return n % m;
}
