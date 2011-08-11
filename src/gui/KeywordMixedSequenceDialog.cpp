#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>

#include "Utility.h"
#include "KeywordMixedSequenceDialog.h"

KeywordMixedSequenceDialog::KeywordMixedSequenceDialog(QWidget *parent)
  : QDialog(parent), keyword(""), columnar(false), mode(true), dump(false)
{
  QLabel *header = new QLabel(tr("Generate the keyword-mixed sequence."));
  QLabel *lblKeyword = new QLabel(tr("Keyword:"));

  txtKeyword = new QLineEdit(keyword);

  chkColumnar = new QCheckBox(tr("Use columnar transposition"));
  chkColumnar->setCheckState(columnar ? Qt::Checked : Qt::Unchecked);

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
  layout->addWidget(chkColumnar);
  layout->addWidget(chkMode);  
  layout->addWidget(chkDump);
  layout->addWidget(box);

  setLayout(layout);

  setWindowTitle(tr("Keyword-mixed Sequence"));
  centerWidget(this);    
}

void KeywordMixedSequenceDialog::onFinished() {
  keyword = txtKeyword->text();
  columnar = (chkColumnar->checkState() == Qt::Checked);
  mode = (chkMode->checkState() == Qt::Checked);  
  dump = (chkDump->checkState() == Qt::Checked);

  accept();
}
