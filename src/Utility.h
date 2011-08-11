#ifndef CLYZER_UTILITY_H
#define CLYZER_UTILITY_H

#include <QPixmap>

#include "Types.h"

class QWidget;
class SubstitutionAlphabet;

void centerWidget(QWidget *widget);

QPixmap generateFrequencyPixmap(const FreqMap &map, int height);

SubstitutionAlphabet *keywordMixedSequence(const QString &keyword,
                                           bool columnar,
                                           const QString &alphabet);

#endif // CLYZER_UTILITY_H
