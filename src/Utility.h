#ifndef CLYZER_UTILITY_H
#define CLYZER_UTILITY_H

#include <QPixmap>

#include "Types.h"

class QWidget;

void centerWidget(QWidget *widget);

QPixmap generateFrequencyPixmap(const FreqMap &map, int height);

#endif // CLYZER_UTILITY_H
