#include <QLabel>
#include <QWidget>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>

#include "Utility.h"
#include "AlignInBlocksDialog.h"

AlignInBlocksDialog::AlignInBlocksDialog(QWidget *parent)
  : QDialog(parent), blockLen(5)
{
  QLabel *header = new QLabel(tr("Select the size of the blocks to rearrange the ciphertext into."));
  QLabel *lblLen = new QLabel(tr("Block length:"));

  spinLen = new QSpinBox;
  spinLen->setValue(blockLen);

  QDialogButtonBox *box =
    new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(box, SIGNAL(accepted()), this, SLOT(onFinished()));
  connect(box, SIGNAL(rejected()), this, SLOT(reject()));

  QHBoxLayout *layoutLen = new QHBoxLayout;
  layoutLen->addWidget(lblLen);
  layoutLen->addWidget(spinLen);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(header);    
  layout->addLayout(layoutLen);
  layout->addWidget(box);

  setLayout(layout);

  setWindowTitle(tr("Align in Blocks"));
  centerWidget(this);   
}

void AlignInBlocksDialog::onFinished() {
  blockLen = spinLen->value();

  accept();
}
