#include <QDir>
#include <QFile>
#include <QFont>
#include <QMenu>
#include <QDebug>
#include <QLabel>
#include <QRegExp>
#include <QMenuBar>
#include <QTextEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QKeySequence>

#include "Types.h"
#include "Affine.h"
#include "Utility.h"
#include "Frequency.h"
#include "MainWindow.h"
#include "AffineDialog.h"
#include "FrequencyDialog.h"

MainWindow::MainWindow() {
  init();
}

MainWindow::~MainWindow() {
  
}

void MainWindow::init() {
  // Set English alphabet at first.
  alphabet.setAlphabet("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  
  // Setup layout.
  QFont txtFont("Courier");
  
  cipherPad = new QTextEdit;
  cipherPad->setFont(txtFont);
  connect(cipherPad, SIGNAL(textChanged()), this, SLOT(onCiphertextChanged()));
  
  scratchPad = new QTextEdit;
  scratchPad->setFont(txtFont);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->setContentsMargins(5, 5, 5, 5);
  layout->addWidget(new QLabel(tr("Ciphertext")));  
  layout->addWidget(cipherPad);
  layout->addWidget(new QLabel(tr("Scratch Pad")));    
  layout->addWidget(scratchPad);  
  
  QWidget *center = new QWidget;
  center->setLayout(layout);

  // Takes responsibility of the deletion of 'center' and descendants.
  setCentralWidget(center);

  // Setup menu: file menu.
  QMenu *fileMenu = menuBar()->addMenu(tr("File"));
  
  QAction *open = fileMenu->addAction(tr("Open Ciphertext"));
  open->setStatusTip(tr("Open a ciphertext to do analysis on."));
  open->setShortcut(QKeySequence::Open);
  connect(open, SIGNAL(triggered()), this, SLOT(onOpenCiphertext()));

  // Analyses menu.
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

  // Transformations menu.
  QMenu *transMenu = menuBar()->addMenu(tr("Transformations"));
  
  QAction *affine = transMenu->addAction(tr("Affine"));
  affine->setStatusTip(tr("Do an Affine transformation of the ciphertext."));
  affine->setShortcut(QKeySequence(btn + "+A"));
  connect(affine, SIGNAL(triggered()), this, SLOT(onAffineTransformation()));
  transActions.append(affine);

  transMenu->addSeparator();

  QMenu *transTextMenu = transMenu->addMenu(tr("Text"));

  QAction *upper = transTextMenu->addAction(tr("Uppercase"));
  upper->setStatusTip(tr("Transform the ciphertext into uppercase."));
  connect(upper, SIGNAL(triggered()), this, SLOT(onUpperTransformation()));
  transActions.append(upper);

  QAction *lower = transTextMenu->addAction(tr("Lowercase"));
  lower->setStatusTip(tr("Transform the ciphertext into lowercase."));
  connect(lower, SIGNAL(triggered()), this, SLOT(onLowerTransformation()));
  transActions.append(lower);    

  enableMenus(false);  

  // Setup window.
  setWindowTitle(tr("Clyzer - The Cryptanalytic Tool"));
  resize(800, 600);
  centerWidget(this);  
}

void MainWindow::enableMenus(bool enable) {
  foreach(QAction *action, analysisActions) {
    action->setEnabled(enable);
  }

  foreach(QAction *action, transActions) {
    action->setEnabled(enable);
  }  
}

QString MainWindow::getCiphertext(bool whitespace) {
  QString ciphertext = cipherPad->toPlainText();

  if (!whitespace) {
    ciphertext = ciphertext.remove(QRegExp("[\\t\\n\\r\\v\\f\\a\\s]"));
  }

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
      cipherPad->setText(data);

      if (data.size() == 0) {
        QMessageBox::critical(this, "", tr("The file is empty!"));
      }
    }
    else {
      QMessageBox::critical(this, "", tr("Could not open file for reading: ") + filePath);
    }
  }
}

void MainWindow::onCiphertextChanged() {
  QString txt = cipherPad->toPlainText();

  // Disable certain menu items if no ciphertext is present.
  enableMenus(txt.size() != 0);
}

void MainWindow::onFrequencyDistribution() {
  FreqMap dist;
  foreach (QChar c, alphabet.getAlphabet()) {
    dist[c] = 0;
  }

  dist = frequencyDistribution(getCiphertext(false), dist);

  FrequencyDialog diag(dist);
  diag.exec();
}

void MainWindow::onDigraphDistribution() {

}

void MainWindow::onSlidingComparison() {

}

void MainWindow::onAffineTransformation() {
  AffineDialog diag;
  if (diag.exec() == QDialog::Accepted) {
    int a = diag.getA(), b = diag.getB();
    bool decipher = diag.doDeciphering();
    bool dump = diag.doDump();

    SubstitutionAlphabet *subst = Affine::createSubstitution(alphabet, a, b);

    QString out = "";
    if (dump) {
      out = subst->dump() + "\n\n";
    }

    QString ciph = getCiphertext();
    if (decipher) {
      out += subst->inverseTransform(ciph);
    }
    else {
      out += subst->transform(ciph);
    }
    delete subst;    

    scratchPad->setText(out);
  }
}

void MainWindow::onUpperTransformation() {
  cipherPad->setText(getCiphertext().toUpper());
}

void MainWindow::onLowerTransformation() {
  cipherPad->setText(getCiphertext().toLower());  
}
