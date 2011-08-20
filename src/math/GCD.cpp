#include "GCD.h"

qint32 gcd(qint32 a, qint32 b) {
  for (;;) {
    a = a % b;
    if (a == 0) return b;
    
    b = b % a;
    if (b == 0) return a;
  }  
}
