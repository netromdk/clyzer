#include <QDebug>

#include "Frequency.h"

FreqMap monographicDistribution(const QString &data,
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

FreqMap polygraphicDistribution(quint32 slideSize, const QString &data,
                                const QRegExp &whitespace,
                                const FreqMap &init) {
  FreqMap dist(init);

  for (int s = 0, e = slideSize; e <= data.size(); s++, e++) {
    QString sub = data.mid(s, e - s);
    if (!sub.contains(whitespace)) {
      if (!dist.contains(sub)) {
        dist[sub] = 1;
      }
      else {
        dist[sub]++;
      }
    }
  }

  return dist;
}
