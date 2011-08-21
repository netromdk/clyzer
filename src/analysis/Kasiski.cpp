#include <QSet>
#include <QMap>
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

void kasiskiMethod(const QString &data, QList<quint32> &candidates,
                   int maxAmount) {
  // Use a better solution later like Suffix trees!
  
  // Find repeated groups of letters.
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

  // Factor all of the intervals.
  KList keys = map.keys();
  qSort(keys.begin(), keys.end(), moreFirst);

  KList facs;    
  foreach (quint32 key, keys) {
    KMap2 &map2 = map[key];
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

    foreach (quint32 i, intervals) {
      pollardRhoFactor(i, facs);
    }
  }

  if (maxAmount <= 0) {
    maxAmount = 1;
  }
     
  QSet<quint32> ufacs = facs.toSet();
  if (ufacs.size() < maxAmount) {
    maxAmount = ufacs.size();
  }

  // Find the most common factors of the intervals.
  for (;;) {
    quint32 res = 0, amount = 0;
    foreach (qint32 i, ufacs) {
      if (candidates.contains(i)) {
        continue;
      }
      
      quint32 cnt = facs.count(i);
      if (cnt > amount) {
        amount = cnt;
        res = i;
      }
    }

    if (!candidates.contains(res)) {
      candidates.append(res);
    }

    if (candidates.size() == maxAmount) {
      break;
    }
  }
}
