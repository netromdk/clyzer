#ifndef CLYZER_FREQUENCY_DIALOG_H
#define CLYZER_FREQUENCY_DIALOG_H

#include <QDialog>

#include "Types.h"

class FrequencyDialog : public QDialog {
  Q_OBJECT
  
public:
  FrequencyDialog(FreqMap &map, QWidget *parent = NULL);
  ~FrequencyDialog() { }
};

#endif // CLYZER_FREQUENCY_DIALOG_H
