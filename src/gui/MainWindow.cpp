#include <QDir>
#include <QFile>
#include <QFont>
#include <QMenu>
#include <QDebug>
#include <QLabel>
#include <QRegExp>
#include <QMenuBar>
#include <QFileInfo>
#include <QTextEdit>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QApplication>
#include <QKeySequence>

#include "Types.h"
#include "Affine.h"
#include "Utility.h"
#include "Intervals.h"
#include "Frequency.h"
#include "MainWindow.h"
#include "AffineDialog.h"
#include "FrequencyDialog.h"

#ifdef __APPLE__
QString META = "Meta";
QString CMD = "Ctrl";
#else
QString META = "Ctrl";
QString CMD = "Meta";
#endif

MainWindow::MainWindow() : filePath("") {
  init();
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
  
  QAction *open = fileMenu->addAction(tr("Open"));
  open->setStatusTip(tr("Open a ciphertext to do analysis on."));
  open->setShortcut(QKeySequence::Open);
  connect(open, SIGNAL(triggered()), this, SLOT(onOpen()));

  restoreAct = fileMenu->addAction(tr("Restore"));
  restoreAct->setStatusTip(tr("Restore the latest opened ciphertext from file."));
  restoreAct->setShortcut(QKeySequence(CMD + "+R"));  
  restoreAct->setEnabled(false);
  connect(restoreAct, SIGNAL(triggered()), this, SLOT(onRestore()));  

  saveAct = fileMenu->addAction(tr("Save"));
  saveAct->setStatusTip(tr("Save the current ciphertext."));
  saveAct->setShortcut(QKeySequence::Save);
  saveAct->setEnabled(false);
  connect(saveAct, SIGNAL(triggered()), this, SLOT(onSave()));

  saveAsAct = fileMenu->addAction(tr("Save As"));
  saveAsAct->setStatusTip(tr("Save the current ciphertext to a new destination."));
  saveAsAct->setEnabled(false);  
  connect(saveAsAct, SIGNAL(triggered()), this, SLOT(onSaveAs()));

  fileMenu->addSeparator();

  QAction *exit = fileMenu->addAction(tr("Exit"));
  exit->setStatusTip(tr("Exit the program."));
  connect(exit, SIGNAL(triggered()), this, SLOT(onExit()));        

  // Analyses menu.
  QMenu *analysisMenu = menuBar()->addMenu(tr("Analysis"));

  QAction *freq = analysisMenu->addAction(tr("Frequency Distribution"));
  freq->setStatusTip(tr("Show a frequency distribution of the ciphertext."));
  freq->setShortcut(QKeySequence(META + "+F"));
  connect(freq, SIGNAL(triggered()), this, SLOT(onFrequencyDistribution()));
  analysisActions.append(freq);

  QAction *digraph = analysisMenu->addAction(tr("Digraph Distribution"));
  digraph->setStatusTip(tr("Show a digraph distribution of the ciphertext to see how the letters relate to each other."));
  digraph->setShortcut(QKeySequence(META + "+D"));  
  connect(digraph, SIGNAL(triggered()), this, SLOT(onDigraphDistribution()));
  analysisActions.append(digraph);

  QAction *lowfreq = analysisMenu->addAction(tr("Low-frequency Intervals"));
  lowfreq->setStatusTip(tr("Computes the low-frequency intervals from the frequency distribution of the ciphertext."));
  lowfreq->setShortcut(QKeySequence(META + "+I"));    
  connect(lowfreq, SIGNAL(triggered()), this, SLOT(onLowFrequencyIntervals()));
  analysisActions.append(lowfreq);      

  QAction *slide = analysisMenu->addAction(tr("Sliding Comparison"));
  slide->setStatusTip(tr("Show a sliding comparison of the plain- and ciphertext distributions."));
  slide->setShortcut(QKeySequence(META + "+S"));    
  connect(slide, SIGNAL(triggered()), this, SLOT(onSlidingComparison()));
  analysisActions.append(slide);    

  // Transformation menu.
  QMenu *transMenu = menuBar()->addMenu(tr("Transformation"));
  
  QAction *affine = transMenu->addAction(tr("Affine"));
  affine->setStatusTip(tr("Do an Affine transformation of the ciphertext."));
  affine->setShortcut(QKeySequence(META + "+A"));
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

void MainWindow::loadFile(QString theFile) {
  restoreAct->setText(tr("Restore"));    
  restoreAct->setEnabled(false);
  saveAct->setEnabled(false);
  saveAsAct->setEnabled(false);  
  
  QFile file(theFile);
  if (!file.open(QFile::ReadOnly)) {
    QMessageBox::critical(this, "", tr("Could not open file for reading: ") + filePath);
    return;
  }
    
  QByteArray data = file.readAll();
  cipherPad->setText(data);

  if (data.size() == 0) {
    QMessageBox::critical(this, "", tr("The file is empty!"));
    return;
  }
    
  // If the base name is longer than 20 characters (plus the
  // .extension) then trim it, so that 10 characters from the left
  // and 10 characters from the right (plus the extension length)
  // remains.
  QString baseName = QFileInfo(theFile).fileName();
  int len = baseName.size(),
    extPos = baseName.lastIndexOf(".");
  if (extPos == -1) {
    extPos = len;
  }
      
  if (len > 20 + (len - extPos)) {
    int rem = 10; 
    baseName = baseName.left(rem) + "..." +
      baseName.right(rem + (len - extPos));
  }
      
  restoreAct->setEnabled(true);
  restoreAct->setText(tr("Restore") + " (" + baseName + ")");
  saveAct->setEnabled(true);
  saveAsAct->setEnabled(true);        
  filePath = theFile;      
}

void MainWindow::saveToFile(QString filePath) {
  QFile file(filePath);
  if (!file.open(QFile::WriteOnly)) {
    QMessageBox::critical(this, "", tr("Could not open file for writing: ") + filePath);
    return;
  }
  
  QByteArray data = getCiphertext().toUtf8();
  qint64 wrote = file.write(data);
  if (wrote != data.size()) {
    QMessageBox::critical(this, "",
                          tr("Could only write ") + QString::number(wrote) +
                          tr(" of ") + QString::number(data.size()) +
                          tr(" to the file: ") + filePath);
    return;
  }

  restoreAct->setEnabled(true);  
  saveAsAct->setEnabled(true);  
}

void MainWindow::closeEvent(QCloseEvent *event) {
  onExit();

  // Ignore event because onExit() will terminate the program if
  // necessary.
  event->ignore();
}

void MainWindow::onOpen() {
  QString caption = tr("Choose a file containing a ciphertext");
  QString filter = "";//tr("Text file (*.txt)");
  QString filePath = QFileDialog::getOpenFileName(this, caption, QDir::homePath(), filter);

  if (!filePath.isEmpty()) {
    loadFile(filePath);
  }
}

void MainWindow::onSave() {
  if (filePath.size() == 0) {
    onSaveAs();
    return;
  }
  
  if (!cipherPad->document()->isModified()) {
    return;
  }

  saveToFile(filePath);
}

void MainWindow::onSaveAs() {
  QString caption = tr("Save the ciphertext to a file");
  QString newFilePath = QFileDialog::getSaveFileName(this, caption, QDir::homePath());
  if (newFilePath.size() == 0) {
    return;
  }

  filePath = newFilePath;
  saveToFile(filePath);
}

void MainWindow::onRestore() {
  if (filePath.size() > 0) {
    loadFile(filePath);
  }
}

void MainWindow::onExit() {
  if (cipherPad->document()->isModified()) {
    QString question = tr("Do you want to save before quitting?");
    QMessageBox::StandardButton btn =
      QMessageBox::question(this, "", question,
                            QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);

    switch (btn) {
    default:
    case QMessageBox::Cancel:
      return;

    case QMessageBox::No:
      QApplication::exit(0);      
      break;

    case QMessageBox::Yes:
      onSave();
      break;      
    }
  }
  
  QApplication::exit(0);
}

void MainWindow::onCiphertextChanged() {
  QString txt = cipherPad->toPlainText();

  // Disable certain menu items if no ciphertext is present.
  enableMenus(txt.size() != 0);

  if (txt.size() > 0) {
    if (filePath.size() == 0) {
      saveAct->setEnabled(true);
    }
    else {
      restoreAct->setEnabled(true);
    }
  }

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

void MainWindow::onLowFrequencyIntervals() {
  FreqIntv intervals = lowFreqIntervals(getCiphertext(false),
                                        alphabet.getAlphabet());

  QString out(tr("Low-frequency intervals between letters based on frequency distribution:") + "\n\n");
  
  typedef QPair<quint32, quint32> Elm;
  foreach (Elm interval, intervals) {
    QString dist = QString::number(interval.first),
      amt = QString::number(interval.second);
    
    out += QString("%1: %2\n").arg(dist, 3).arg(amt, 4);
  }
  
  scratchPad->setText(out);
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
