#ifndef CLYZER_KASISKI_METHOD_H
#define CLYZER_KASISKI_METHOD_H

#include <QList>
#include <QString>

void kasiskiMethod(const QString &data, QList<quint32> &candidates,
                   int maxAmount = 3);

#endif // CLYZER_KASISKI_METHOD_H
