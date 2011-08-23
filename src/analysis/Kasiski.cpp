#include <QDebug> //remove

#include <QSet>
#include <QMap>
#include <QtAlgorithms>

#include "Kasiski.h"
#include "PollardRho.h"

namespace {
  typedef QSet<quint32> KSet;  
  typedef QList<quint32> KList;
  typedef QMap<QString, KList> KMap2;
  typedef QMap<quint32, KMap2> KMap; // k -> string -> start pos
  
  bool moreFirst(quint32 e1, quint32 e2) {
    return e1 >= e2;
  }      
}

void kasiskiMethod(const QString &data, QList<quint32> &candidates) {
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

    QList<KList> facList;    
    foreach (quint32 i, intervals) {
      KList facs;            
      pollardRhoFactor(i, facs);
      facList.append(facs);
    }
  
    // Find the common factors.
    KSet set = facList[0].toSet();
    for (int i = 1; i < facList.size(); i++) {
      KSet facs = facList[i].toSet();
      set = set.intersect(facs);
    }
    candidates.append(set.toList());
  }

  candidates = candidates.toSet().toList();
}
