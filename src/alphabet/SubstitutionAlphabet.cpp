#include <QDebug>

#include "SubstitutionAlphabet.h"

SubstitutionAlphabet::SubstitutionAlphabet(const Alphabet &plain,
                                           const Alphabet &cipher)
  : plain(plain), cipher(cipher)
{
  
}

QString SubstitutionAlphabet::transform(const QString &data) {
  return implTransform(false, data);
}

QString SubstitutionAlphabet::inverseTransform(const QString &data) {
  return implTransform(true, data);  
}

QString SubstitutionAlphabet::dump() const {
  return "Plain:  " + plain.dump() + "\nCipher: " + cipher.dump();
}

QString SubstitutionAlphabet::implTransform(bool inverse,
                                            const QString &data) {
  QString res = "",
    alph = plain.getAlphabet(),
    ciph = cipher.getAlphabet(); 

  if (inverse) {
    QString tmp = alph;
    alph = ciph;
    ciph = tmp;
  }

  foreach (QChar c, data) {
    int pos = alph.indexOf(c);
    if (pos == -1 || pos >= ciph.size()) {
      res.append(c);
      continue;
    }

    res.append(ciph[pos]);
  }
  
  return res;
}
