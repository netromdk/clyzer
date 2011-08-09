#ifndef CLYZER_AFFINE_DIALOG_H
#define CLYZER_AFFINE_DIALOG_H

#include <QDialog>
#include <QWidget>

class QSpinBox;
class QCheckBox;

class AffineDialog : public QDialog {
  Q_OBJECT
  
public:
  AffineDialog(QWidget *parent = NULL);
  ~AffineDialog() { }

  int getA() const { return a; }
  int getB() const { return b; }
  bool doDeciphering() const { return decipher; }
  bool doDump() const { return dump; }

private slots:
  void onFinished();

private:
  int a, b;
  bool decipher, dump;

  QSpinBox *spinA, *spinB;
  QCheckBox *chkMode, *chkDump;
};

#endif // CLYZER_AFFINE_DIALOG_H
