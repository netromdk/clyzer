#include <QDebug> // remove
#include <QSet>
#include <QMap>
#include <QList>
#include <QtAlgorithms>

#include "Kasiski.h"
#include "PollardRho.h"

namespace {
  typedef QList<quint32> KList;
  typedef QMap<QString, KList> KMap2;
  typedef QMap<quint32, KMap2> KMap; // k -> string -> start pos
  
  bool moreFirst(quint32 e1, quint32 e2) {
    return e1 >= e2;
  }      
}

quint32 kasiskiMethod(const QString &data) {
  // Use a better solution later like Suffice trees!
  
  // 1. Find repeated groups of letters.  For instance, if the
  // ciphertext was FGXTHJAQWNFGXQ, the distance between FGX's is 10.
  KMap map;
  int n = data.size(), maxL, maxR, maxLen;
  maxL = maxR = maxLen = 0;
  for (int l = 0; l < n; l++) {
    for (int r = 0; r < n; r++) {
      if (l == r) continue;
      
      // Find the longest matching substring starting from l and r.
      int k = 0;
      for (;; k++) {
        int p0 = l + k, p1 = r + k;
        if (p0 >= n - 1 || p1 >= n - 1 || data[p0] != data[p1]) {
          k--;
          break;
        }
      }

      if (k > 0) {
        k++;
        
        KMap2 &map2 = map[k];
        KList &list = map2[data.mid(l, k)];
        if (!list.contains(l)) {
          list.append(l);
        }
      }
    }    
  }

  // 2. Factor each of the found numbers, starting with the ones that
  // were repeated most times, and if any number is repeated in the
  // majority of these factorings, it is likely to be the length of
  // the keyword.
  KList keys = map.keys();
  if (keys.size() > 0) {
    qSort(keys.begin(), keys.end(), moreFirst);
    
    KMap2 &map2 = map[keys[0]];
    KList intervals;
    foreach (const QString &key, map2.keys()) {
      KList &starts = map2[key];
      if (starts.size() < 2) {
        continue;
      }

      for (int i = 0; i < starts.size() - 1; i++) {
        intervals.append(abs(starts[i] - starts[i + 1]));
      }
    }

    // Find the most common factor of the intervals.
    QList<qint32> facs;    
    foreach (qint32 i, intervals) {
      pollardRhoFactor(i, facs);
    }
    
    QSet<qint32> ufacs = facs.toSet();
    qint32 res = 0, amount = 0;
    foreach (qint32 i, ufacs) {
      qint32 cnt = facs.count(i);
      if (cnt > amount) {
        amount = cnt;
        res = i;
      }
    }
    
    return res;
  }
  
  return 0;
}
