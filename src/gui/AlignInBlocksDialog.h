#ifndef CLYZER_ALIGN_IN_BLOCKS_DIALOG_H
#define CLYZER_ALIGN_IN_BLOCKS_DIALOG_H

#include <QDialog>

class QWidget;
class QSpinBox;

class AlignInBlocksDialog : public QDialog {
  Q_OBJECT

public:
  AlignInBlocksDialog(QWidget *parent = NULL);
  ~AlignInBlocksDialog() { }

  quint32 getBlockLength() const { return blockLen; }

private slots:
  void onFinished();
  
private:
  quint32 blockLen;

  QSpinBox *spinLen;
};

#endif // CLYZER_ALIGN_IN_BLOCKS_DIALOG_H
