#ifndef CLYZER_MAIN_WINDOW_H
#define CLYZER_MAIN_WINDOW_H

#include <QList>
#include <QMainWindow>

#include "Alphabet.h"

class QAction;
class QTextEdit;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow();

private slots:
  void onOpenCiphertext();
  void onCiphertextChanged();
  void onFrequencyDistribution();
  void onDigraphDistribution();
  void onLowFrequencyIntervals();
  void onSlidingComparison();
  void onAffineTransformation();
  void onUpperTransformation();
  void onLowerTransformation();

private:
  void init();
  void enableMenus(bool enable = true);
  QString getCiphertext(bool whitespace = true);

  Alphabet alphabet;
  QList<QAction*> analysisActions, transActions;
  QTextEdit *cipherPad, *scratchPad;
};

#endif // CLYZER_MAIN_WINDOW_H
