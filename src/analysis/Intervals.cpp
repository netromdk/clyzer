#include <QtAlgorithms>

#include <cmath>

#include "Intervals.h"
#include "Frequency.h"

namespace {
  typedef QPair<quint32, quint32> Elm;
  bool lessFirst(const Elm &e1, const Elm &e2) {
    return e1.second < e2.second;
  }

  bool moreFirst(const Elm &e1, const Elm &e2) {
    return !lessFirst(e1, e2);
  }    
}

FreqIntv lowFreqIntervals(QString data, QString alphabet) {
  // Get frequency distribution.
  FreqMap dist;
  foreach (QChar c, alphabet) {
    dist[c] = 0;
  }  
  dist = frequencyDistribution(data, dist);

  // Sort the list so that the #occurrences appears lowest first.  
  FreqIntv all;
  foreach (QString c, dist.keys()) {
    all.append(Elm(alphabet.indexOf(c), dist[c]));
  }
  qSort(all.begin(), all.end(), lessFirst);

  // Pick the lowest occurring elements (about 30-40%).
  int amount = (float) alphabet.size() * 0.35;
  QString chosen = "";
  for (int i = 0; i < amount; i++) {
    chosen += alphabet[all[i].first];
  }

  // Determine the intervals.
  QMap<quint32, quint32> counts;
  foreach (QChar a, chosen) {
    foreach (QChar b, chosen) {
      if (a == b) continue;

      int posA = alphabet.indexOf(a),
        posB = alphabet.indexOf(b),
        diff1 = abs(posA - posB),
        diff2 = alphabet.size() - diff1;

      int diff = diff1;
      if (diff2 < diff1) {
        diff = diff2;
      }

      if (!counts.contains(diff)) {
        counts[diff] = 1;
      }
      else {
        counts[diff]++;        
      }
    }    
  }

  // Return a sorted list of pairs (Interval distance, #occurrences)
  // with the one with the most occurrences first and so on.
  FreqIntv intervals;
  foreach (quint32 d, counts.keys()) {
    if (d <= 1) continue;
    intervals.append(Elm(d, counts[d]));
  }
  qSort(intervals.begin(), intervals.end(), moreFirst);

  return intervals;
}
