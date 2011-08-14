#include "Mod.h"
#include "Vigenere.h"
#include "SubstitutionAlphabet.h"

SubstitutionAlphabet *Vigenere::createSubstitution(const Alphabet &alph,
                                                   const QString &key) {
  Alphabet ciph(key);
  return new SubstitutionAlphabet(alph, ciph, true);
}
