#ifndef CLYZER_MAIN_WINDOW_H
#define CLYZER_MAIN_WINDOW_H

#include <QList>
#include <QRegExp>
#include <QMainWindow>

#include "Alphabet.h"

class QAction;
class QToolBar;
class QTextEdit;
class QCloseEvent;
class SubstitutionAlphabet;

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
  void onMonographicDistribution();
  void onDigraphicDistribution();
  void onTrigraphicDistribution();  
  void onLowFrequencyIntervals();
  void onSlidingComparison();
  void onAffineTransformation();
  void onVigenereTransformation();
  void onKeywordMixedSequence();  
  void onCopyDown();
  void onCopyUp();  
  void onUpCase();
  void onDownCase();

private:
  void init();
  void enableMenus(bool enable = true);
  QString getCiphertext(bool whitespace = true);
  void loadFile(QString filePath);
  void saveToFile(QString filePath);
  void setRestoreName();
  void employSubstitutionAlphabet(SubstitutionAlphabet *subst,
                                  bool decipher, bool dump);

  Alphabet alphabet;
  QRegExp whitespace;
  QList<QAction*> analysisActions, transActions;
  QAction *saveAct, *saveAsAct, *restoreAct;
  QTextEdit *cipherPad, *scratchPad;
  QToolBar *toolBar;
  QString filePath;
};

#endif // CLYZER_MAIN_WINDOW_H
