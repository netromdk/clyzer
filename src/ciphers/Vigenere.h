#ifndef CLYZER_VIGENERE_H
#define CLYZER_VIGENERE_H

#include "Alphabet.h"

class SubstitutionAlphabet;

class Vigenere {
public:
  static SubstitutionAlphabet *createSubstitution(const Alphabet &alph,
                                                  const QString &key);
};

#endif // CLYZER_VIGENERE_H
