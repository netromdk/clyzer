#ifndef CLYZER_DISTRIBUTION_H
#define CLYZER_DISTRIBUTION_H

#include <QRegExp>

#include "Types.h"

FreqMap monographicDistribution(const QString &data,
                                const FreqMap &init = FreqMap());

FreqMap polygraphicDistribution(quint32 slideSize, const QString &data,
                                const QRegExp &whitespace,
                                const FreqMap &init = FreqMap());

#endif // CLYZER_DISTRIBUTION_H
