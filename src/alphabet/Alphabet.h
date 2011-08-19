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
  quint32 size() const { return alphabet.size(); }
  QString getAlphabet() const { return alphabet; }
  QString dump(bool spaces = true) const;

signals:
  void changed();

private:
  QString alphabet;
};

#endif // CLYZER_ALPHABET_H
