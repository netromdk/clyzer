#include <QDir>
#include <QFile>
#include <QMenu>
#include <QDebug>
#include <QMenuBar>
#include <QTextEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QKeySequence>

#include "Utility.h"
#include "MainWindow.h"

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

  QAction *freq = analysisMenu->addAction(tr("Frequency Distribution"));
  freq->setStatusTip(tr("Show a frequency distribution of the ciphertext."));
  connect(freq, SIGNAL(triggered()), this, SLOT(onFrequencyDistribution()));
  analysisActions.append(freq);

  QAction *digraph = analysisMenu->addAction(tr("Digraph Distribution"));
  digraph->setStatusTip(tr("Show a digraph distribution of the ciphertext to see how the letters relate to each other."));
  connect(digraph, SIGNAL(triggered()), this, SLOT(onDigraphDistribution()));
  analysisActions.append(digraph);

  analysisMenu->addSeparator();

  QAction *slide = analysisMenu->addAction(tr("Sliding Comparison"));
  slide->setStatusTip(tr("Show a sliding comparison of the plain- and ciphertext distributions."));
  connect(slide, SIGNAL(triggered()), this, SLOT(onSlidingComparison()));
  analysisActions.append(slide);    

  enableAnalyses(false);

  // Setup window.
  resize(800, 600);
  centerWidget(this);  
}

void MainWindow::enableAnalyses(bool enable) {
  foreach(QAction *action, analysisActions) {
    action->setEnabled(enable);
  }
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
  
}

void MainWindow::onDigraphDistribution() {

}

void MainWindow::onSlidingComparison() {

}
