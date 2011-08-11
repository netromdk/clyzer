#ifndef CLYZER_MAIN_WINDOW_H
#define CLYZER_MAIN_WINDOW_H

#include <QList>
#include <QMainWindow>

#include "Alphabet.h"

class QAction;
class QTextEdit;
class QCloseEvent;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow() { }

protected:
  void closeEvent(QCloseEvent *event);
               
private slots:
  void onOpen();
  void onSave();
  void onSaveAs();
  void onRestore();    
  void onExit();
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
  void loadFile(QString filePath);
  void saveToFile(QString filePath);

  Alphabet alphabet;
  QList<QAction*> analysisActions, transActions;
  QAction *saveAct, *saveAsAct, *restoreAct;
  QTextEdit *cipherPad, *scratchPad;
  QString filePath;
};

#endif // CLYZER_MAIN_WINDOW_H
