#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>

#include "Utility.h"
#include "AffineDialog.h"

AffineDialog::AffineDialog(QWidget *parent)
  : QDialog(parent), a(1), b(0), decipher(true), dump(false)
{
  QLabel *header = new QLabel(tr("Select (a, b) of the Affine equation: C = a(P + b) (mod alph. size)."));
  QLabel *lblA = new QLabel(tr("a:"));
  QLabel *lblB = new QLabel(tr("b:"));

  spinA = new QSpinBox;
  spinA->setValue(a);
  
  spinB = new QSpinBox;
  spinB->setValue(b);
  
  chkMode = new QCheckBox(tr("Do decipherment?"));
  chkMode->setCheckState(decipher ? Qt::Checked : Qt::Unchecked);
  
  chkDump = new QCheckBox(tr("Dump substitute alphabet?"));
  chkDump->setCheckState(dump ? Qt::Checked : Qt::Unchecked);  

  QDialogButtonBox *box =
    new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(box, SIGNAL(accepted()), this, SLOT(onFinished()));
  connect(box, SIGNAL(rejected()), this, SLOT(reject()));    

  QHBoxLayout *layoutA = new QHBoxLayout;
  layoutA->addWidget(lblA);
  layoutA->addWidget(spinA);  

  QHBoxLayout *layoutB = new QHBoxLayout;
  layoutB->addWidget(lblB);
  layoutB->addWidget(spinB);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(header);
  layout->addLayout(layoutA);
  layout->addLayout(layoutB);
  layout->addWidget(chkMode);  
  layout->addWidget(chkDump);
  layout->addWidget(box);  

  setLayout(layout);
  
  setWindowTitle(tr("Affine Transformation"));
  centerWidget(this);  
}

void AffineDialog::onFinished() {
  a = spinA->value();
  b = spinB->value();
  decipher = (chkMode->checkState() == Qt::Checked);
  dump = (chkDump->checkState() == Qt::Checked);  

  accept();
}
