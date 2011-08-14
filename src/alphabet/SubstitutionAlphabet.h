#ifndef CLYZER_SUBSTITUTION_ALPHABET_H
#define CLYZER_SUBSTITUTION_ALPHABET_H

#include <QMap>

#include "Alphabet.h"

class SubstitutionAlphabet {
public:
  SubstitutionAlphabet(const Alphabet &plain, const Alphabet &cipher, bool poly = false);
  ~SubstitutionAlphabet() { }

  QString transform(const QString &data);
  QString inverseTransform(const QString &data);
  QString dump() const;

private:
  QString implTransform(bool inverse, const QString &data);

  Alphabet plain, cipher;
  bool poly;  
  QMap<QChar, QString> polyAlphs;
};

#endif // CLYZER_SUBSTITUTION_ALPHABET_H
