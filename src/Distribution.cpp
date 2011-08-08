#include <QDebug>

#include "Distribution.h"

FreqMap frequencyDistribution(const QString &data,
                              const FreqMap &init) {  
  FreqMap dist(init);

  foreach(QString ch, data) {
    if (!dist.contains(ch)) {
      dist[ch] = 1;
    }
    else {
      dist[ch]++;
    }
  }

  return dist;
}
