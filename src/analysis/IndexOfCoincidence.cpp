#include "IndexOfCoincidence.h"

float ic(const FreqMap &dist, quint32 len, quint32 alphLen) {
  float res = 0;

  foreach (quint32 freq, dist.values()) {
    res += (float) freq * (freq - 1.0);
  }

  res /= ((float) len * (len - 1.0)) / (float) alphLen;

  return res;
}
