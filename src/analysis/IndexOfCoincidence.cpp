#include "IndexOfCoincidence.h"

float ic(const FreqMap &dist, quint32 len) {
  float res = 0;

  foreach (quint32 freq, dist.values()) {
    res += (float) freq * (freq - 1.0);
  }

  res /= (float) len * (len - 1.0);

  return res;
}
