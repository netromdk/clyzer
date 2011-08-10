#include "Alphabet.h"

Alphabet::Alphabet() {
  
}

Alphabet::Alphabet(const Alphabet &alphabet_)
  : alphabet(alphabet_.getAlphabet())
{
  
}

Alphabet::Alphabet(const QString &alphabet) : alphabet(alphabet) {
  
}

void Alphabet::setAlphabet(const QString &alphabet_) {
  alphabet = alphabet_;
  emit changed();
}

QString Alphabet::dump() const {
  QString res = "";

  foreach(QChar c, alphabet) {
    res += QString(c) + " ";
  }

  return res;
}
