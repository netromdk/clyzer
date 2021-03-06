#include "Mod.h"
#include "Affine.h"
#include "SubstitutionAlphabet.h"

SubstitutionAlphabet *Affine::createSubstitution(const Alphabet &alph,
                                                 int a, int b) {
  QString ciph = "", aalph = alph.getAlphabet();

  // C = aP + b
  foreach (QChar c, aalph) {
    // Remember to work from 1 to .. and not from zero.
    int pos = aalph.indexOf(c) + 1;

    pos *= a;      
    pos += b;            
    pos = mod(pos, aalph.size());
    if (pos == 0) {
      pos = aalph.size();
    }
      
    ciph.append(aalph[pos - 1]);
  }
    
  return new SubstitutionAlphabet(alph, ciph);
}
