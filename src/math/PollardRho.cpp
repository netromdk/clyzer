#include <QtAlgorithms>

#include "Mod.h"
#include "GCD.h"
#include "Random.h"
#include "Primality.h"
#include "PollardRho.h"

namespace {
  qint32 func(qint32 x, qint32 a, qint32 n) {
    return mod(x * x + a, n);
  }  
}

void pollardRhoFactor(qint32 n, QList<qint32> &facs) {
  if (isPropPrime(n)) {
    facs.push_back(n);
    qSort(facs.begin(), facs.end());
    return;
  }

  int g = pollardRho(n);
  if (!isPropPrime(g)) {
    QList<qint32> ext;
    pollardRhoFactor(g, ext);
    facs.append(ext);
  }
  else {
    facs.push_back(g);
  }

  pollardRhoFactor(n / g, facs);  
}

qint32 pollardRho(qint32 n) {
  if (n < 2) return 0;
  
  if (n == 4) return 2;
      
  if (isPropPrime(n)) return n;

  Random rnd;
  qint32 a = rnd.getRange(1, n - 3),
    s = rnd.getRange(0, n - 1);    

  qint32 g, u, v;    
  u = v = s;
    
  do {
    u = func(u, a, n);
    v = func(func(v, a, n), a, n);
    g = gcd(mod(u - v, n), n);
  } while (g == 1);

  if (g == n) {
    return pollardRho(n);
  }
    
  return g;
}
