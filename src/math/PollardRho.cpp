#include <QDebug> //remove
#include <QtAlgorithms>

#include "Mod.h"
#include "GCD.h"
#include "Random.h"
#include "Primality.h"
#include "PollardRho.h"

namespace {
  quint32 func(quint32 x, quint32 a, quint32 n) {
    return mod(x * x + a, n);
  }  
}

void pollardRhoFactor(quint32 n, QList<quint32> &facs) {
  if (n < 2) return;
  
  if (isPropPrime(n)) {
    facs.push_back(n);
    qSort(facs.begin(), facs.end());
    return;
  }

  int g = pollardRho(n);
  if (!isPropPrime(g)) {
    QList<quint32> ext;
    pollardRhoFactor(g, ext);
    facs.append(ext);
  }
  else {
    facs.push_back(g);
  }

  pollardRhoFactor(n / g, facs);  
}

quint32 pollardRho(quint32 n) {
  if (n < 2) return n;
  
  if (n == 4) return 2;

  if (isPropPrime(n)) return n;

  Random rnd;
  quint32 a = rnd.getRange(1, n - 3),
    s = rnd.getRange(0, n - 1);

  quint32 g, u, v;    
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
