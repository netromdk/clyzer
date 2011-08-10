#ifndef CLYZER_SUBSTITUTION_ALPHABET_H
#define CLYZER_SUBSTITUTION_ALPHABET_H

#include "Alphabet.h"

class SubstitutionAlphabet {
public:
  SubstitutionAlphabet(const Alphabet &plain, const Alphabet &cipher);
  ~SubstitutionAlphabet() { }

  QString transform(const QString &data);
  QString inverseTransform(const QString &data);
  QString dump() const;

private:
  QString implTransform(bool inverse, const QString &data);
  
  Alphabet plain, cipher;
};

#endif // CLYZER_SUBSTITUTION_ALPHABET_H
