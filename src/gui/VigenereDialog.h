#ifndef CLYZER_VIGENERE_DIALOG_H
#define CLYZER_VIGENERE_DIALOG_H

#include <QDialog>

class QWidget;
class QCheckBox;
class QLineEdit;

class VigenereDialog : public QDialog {
  Q_OBJECT

public:
  VigenereDialog(QWidget *parent = NULL);
  ~VigenereDialog() { }

  QString getKeyword() const { return keyword; }
  bool doDeciphering() const { return mode; }  
  bool doDump() const { return dump; }

private slots:
  void onFinished();
  
private:
  QLineEdit *txtKeyword;
  QCheckBox *chkMode, *chkDump;
  
  QString keyword;
  bool mode, dump;
};

#endif // CLYZER_VIGENERE_DIALOG_H
