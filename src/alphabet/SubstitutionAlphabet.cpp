#include "SubstitutionAlphabet.h"

SubstitutionAlphabet::SubstitutionAlphabet(const Alphabet &plain,
                                           const Alphabet &cipher,
                                           bool poly)
  : plain(plain), cipher(cipher), poly(poly)
{
  // Generate the Vigenére square.
  if (poly) {
    polyAlphs.clear();
    QString alph = plain.getAlphabet(),
      alph2 = alph;
    for (int i = 0; i < alph.size(); i++) {
      polyAlphs[alph[i]] = alph2;
      alph2 = alph2.right(alph2.size() - 1) + alph2.left(1);
    }
  }
}

QString SubstitutionAlphabet::transform(const QString &data) {
  return implTransform(false, data);
}

QString SubstitutionAlphabet::inverseTransform(const QString &data) {
  return implTransform(true, data);  
}

QString SubstitutionAlphabet::dump() const {
  if (poly) {
    return "Poly key: " + cipher.dump(false);
  }

  return "Plain:  " + plain.dump() + "\nCipher: " + cipher.dump();
}

QString SubstitutionAlphabet::implTransform(bool inverse,
                                            const QString &data) {
  QString res = "",
    alph = plain.getAlphabet(),
    ciph = cipher.getAlphabet(),
    origCiph = ciph; 

  if (!poly && inverse) {
    QString tmp = alph;
    alph = ciph;
    ciph = tmp;
  }

  int pos = 0, cpos = 0;
  foreach (QChar c, data) {
    if (poly) {
      ciph = polyAlphs[origCiph[cpos]];

      if (inverse) {
        pos = ciph.indexOf(c);
      }
      else {
        pos = alph.indexOf(c);
      }
    }
    else {
      pos = alph.indexOf(c);
    }

    if (pos == -1 || pos >= ciph.size()) {
      res.append(c);
      continue;
    }    

    if (poly) {
      cpos++;      
      cpos %= origCiph.size();
    }
    
    if (poly && inverse) {
      res.append(alph[pos]);
    }
    else {
      res.append(ciph[pos]);
    }
  }
  
  return res;
}
