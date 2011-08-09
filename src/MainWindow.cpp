#include <QDir>
#include <QFile>
#include <QMenu>
#include <QDebug>
#include <QRegExp>
#include <QMenuBar>
#include <QTextEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QKeySequence>

#include "Types.h"
#include "Utility.h"
#include "MainWindow.h"
#include "Distribution.h"
#include "FrequencyDialog.h"

MainWindow::MainWindow() {
  init();
}

MainWindow::~MainWindow() {
  delete txt;
}

void MainWindow::init() {
  // Setup objects.
  txt = new QTextEdit;
  txt->setReadOnly(true);
  setCentralWidget(txt);

  // Setup menu.
  QMenu *fileMenu = menuBar()->addMenu(tr("File"));
  
  QAction *open = fileMenu->addAction(tr("Open Ciphertext"));
  open->setStatusTip(tr("Open a ciphertext to do analysis on."));
  open->setShortcut(QKeySequence::Open);
  connect(open, SIGNAL(triggered()), this, SLOT(onOpenCiphertext()));

  analysisMenu = menuBar()->addMenu(tr("Analysis"));

  QString btn;
#ifdef __APPLE__
  btn = "Meta";
#else
  btn = "Ctrl";
#endif
  
  QAction *freq = analysisMenu->addAction(tr("Frequency Distribution"));
  freq->setStatusTip(tr("Show a frequency distribution of the ciphertext."));
  freq->setShortcut(QKeySequence(btn + "+F"));
  connect(freq, SIGNAL(triggered()), this, SLOT(onFrequencyDistribution()));
  analysisActions.append(freq);

  QAction *digraph = analysisMenu->addAction(tr("Digraph Distribution"));
  digraph->setStatusTip(tr("Show a digraph distribution of the ciphertext to see how the letters relate to each other."));
  digraph->setShortcut(QKeySequence(btn + "+D"));  
  connect(digraph, SIGNAL(triggered()), this, SLOT(onDigraphDistribution()));
  analysisActions.append(digraph);

  analysisMenu->addSeparator();

  QAction *slide = analysisMenu->addAction(tr("Sliding Comparison"));
  slide->setStatusTip(tr("Show a sliding comparison of the plain- and ciphertext distributions."));
  slide->setShortcut(QKeySequence(btn + "+S"));    
  connect(slide, SIGNAL(triggered()), this, SLOT(onSlidingComparison()));
  analysisActions.append(slide);    

  enableAnalyses(false);

  // Setup window.
  setWindowTitle(tr("Clyzer - The Cryptanalytic Tool"));
  resize(800, 600);
  centerWidget(this);  
}

void MainWindow::enableAnalyses(bool enable) {
  foreach(QAction *action, analysisActions) {
    action->setEnabled(enable);
  }
}

QString MainWindow::getCiphertext() {
  QString ciphertext = txt->toPlainText();

  // Remove white space.
  ciphertext = ciphertext.remove(QRegExp("[\\t\\n\\r\\v\\f\\a\\s]"));  

  return ciphertext;
}

void MainWindow::onOpenCiphertext() {
  QString caption = tr("Choose a file containing a ciphertext");
  QString filter = tr("Text file (*.txt)");
  QString filePath = QFileDialog::getOpenFileName(this, caption, QDir::homePath(), filter);

  if (!filePath.isEmpty()) {
    QFile file(filePath);
    if (file.open(QFile::ReadOnly)) {
      QByteArray data = file.readAll();
      txt->setText(data);

      if (data.size() == 0) {
        enableAnalyses(false);
        QMessageBox::critical(this, "", tr("The file is empty!"));
      }
      else {
        enableAnalyses();        
      }
    }
    else {
      QMessageBox::critical(this, "", tr("Could not open file for reading: ") + filePath);
    }
  }
}

void MainWindow::onFrequencyDistribution() {
  // Assume English alphabet at first [A-Z]!
  FreqMap dist;
  for (char c = 'A'; c != 'Z'; c++) {
    dist[QString(c)] = 0;
  }

  dist = frequencyDistribution(getCiphertext(), dist);

  FrequencyDialog diag(dist);
  diag.exec();
}

void MainWindow::onDigraphDistribution() {

}

void MainWindow::onSlidingComparison() {

}
