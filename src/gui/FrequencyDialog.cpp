#include "Utility.h"
#include "FrequencyDialog.h"

#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QDialogButtonBox>

FrequencyDialog::FrequencyDialog(FreqMap &map, QWidget *parent)
  : QDialog(parent)
{
  QLabel *lbl = new QLabel;
  QPixmap pix = generateFrequencyPixmap(map, 250);
  lbl->setPixmap(pix);

  int count = 0;
  foreach (QString key, map.keys()) {
    count += map[key];
  }

  QScrollArea *lblScroll = new QScrollArea;
  lblScroll->setWidget(lbl);
  
  QLabel *lbl2 = new QLabel;
  lbl2->setText(tr("Character count: ") + QString::number(count));

  QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok);
  connect(box, SIGNAL(accepted()), this, SLOT(accept()));

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(lblScroll);
  layout->addWidget(lbl2);  
  layout->addWidget(box);
  setLayout(layout);

  setWindowTitle(tr("Frequency Distribution"));
  centerWidget(this);
}

