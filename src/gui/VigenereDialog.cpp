#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>

#include "Utility.h"
#include "VigenereDialog.h"

VigenereDialog::VigenereDialog(QWidget *parent)
  : QDialog(parent), keyword(""), mode(true), dump(false)
{
  QLabel *header = new QLabel(tr("Input the keyword for the Vigenere cipher."));
  QLabel *lblKeyword = new QLabel(tr("Keyword:"));

  txtKeyword = new QLineEdit(keyword);

  chkMode = new QCheckBox(tr("Do decipherment"));
  chkMode->setCheckState(mode ? Qt::Checked : Qt::Unchecked);  

  chkDump = new QCheckBox(tr("Dump sequence"));
  chkDump->setCheckState(dump ? Qt::Checked : Qt::Unchecked);  

  QDialogButtonBox *box =
    new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(box, SIGNAL(accepted()), this, SLOT(onFinished()));
  connect(box, SIGNAL(rejected()), this, SLOT(reject()));

  QHBoxLayout *layoutKeyword = new QHBoxLayout;
  layoutKeyword->addWidget(lblKeyword);
  layoutKeyword->addWidget(txtKeyword);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(header);  
  layout->addLayout(layoutKeyword);
  layout->addWidget(chkMode);  
  layout->addWidget(chkDump);
  layout->addWidget(box);

  setLayout(layout);

  setWindowTitle(tr("Vigenere Transformation"));
  centerWidget(this);    
}

void VigenereDialog::onFinished() {
  keyword = txtKeyword->text();
  mode = (chkMode->checkState() == Qt::Checked);  
  dump = (chkDump->checkState() == Qt::Checked);

  accept();
}
