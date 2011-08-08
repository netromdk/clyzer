#ifndef CLYZER_MAIN_WINDOW_H
#define CLYZER_MAIN_WINDOW_H

#include <QList>
#include <QMainWindow>

class QMenu;
class QAction;
class QTextEdit;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow();

private slots:
  void onOpenCiphertext();
  void onFrequencyDistribution();
  void onDigraphDistribution();
  void onSlidingComparison();

private:
  void init();
  void enableAnalyses(bool enable = true);
  QString getCiphertext();

  QMenu *analysisMenu;
  QList<QAction*> analysisActions;
  QTextEdit *txt;
};

#endif // CLYZER_MAIN_WINDOW_H
