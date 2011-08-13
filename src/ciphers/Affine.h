#ifndef CLYZER_AFFINE_H
#define CLYZER_AFFINE_H

#include "Alphabet.h"

class SubstitutionAlphabet;

class Affine {
public:
  static SubstitutionAlphabet *createSubstitution(const Alphabet &alph,
                                                  int a, int b);
};

#endif // CLYZER_AFFINE_H
