#ifndef CLYZER_KEYWORD_MIXED_SEQUENCE_DIALOG_H
#define CLYZER_KEYWORD_MIXED_SEQUENCE_DIALOG_H

#include <QDialog>

class QWidget;
class QCheckBox;
class QLineEdit;

class KeywordMixedSequenceDialog : public QDialog {
  Q_OBJECT

public:
  KeywordMixedSequenceDialog(QWidget *parent = NULL);
  ~KeywordMixedSequenceDialog() { }

  QString getKeyword() const { return keyword; }
  bool useColumnarTransposition() const { return columnar; }
  bool doDeciphering() const { return mode; }  
  bool doDump() const { return dump; }

private slots:
  void onFinished();
  
private:
  QLineEdit *txtKeyword;
  QCheckBox *chkColumnar, *chkMode, *chkDump;
  
  QString keyword;
  bool columnar, mode, dump;
};

#endif // CLYZER_KEYWORD_MIXED_SEQUENCE_DIALOG_H
