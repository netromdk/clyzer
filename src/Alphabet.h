#ifndef CLYZER_ALPHABET_H
#define CLYZER_ALPHABET_H

#include <QString>
#include <QObject>

class Alphabet : public QObject {
  Q_OBJECT

public:
  Alphabet();
  Alphabet(const Alphabet &alphabet);
  Alphabet(const QString &alphabet);
  ~Alphabet() { }

  void setAlphabet(const QString &alphabet);
  QString getAlphabet() const { return alphabet; }
  QString dump() const;

signals:
  void changed();

private:
  QString alphabet;
};

#endif // CLYZER_ALPHABET_H
