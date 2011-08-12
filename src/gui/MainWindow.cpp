#include <QDir>
#include <QFile>
#include <QFont>
#include <QMenu>
#include <QDebug>
#include <QLabel>
#include <QMenuBar>
#include <QToolBar>
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
#include "KeywordMixedSequenceDialog.h"

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
  whitespace = QRegExp("[\\t\\n\\r\\v\\f\\a\\s]");
  
  // Setup layout.
  toolBar = new QToolBar(tr("laawl"));

  QAction *copyDown = toolBar->addAction(tr("Copy down"));
  connect(copyDown, SIGNAL(triggered()), this, SLOT(onCopyDown()));
  
  QAction *copyUp = toolBar->addAction(tr("Copy up"));
  connect(copyUp, SIGNAL(triggered()), this, SLOT(onCopyUp()));  
  
  toolBar->addSeparator();
  
  QAction *upCase = toolBar->addAction(tr("Upcase"));
  connect(upCase, SIGNAL(triggered()), this, SLOT(onUpCase()));    
  
  QAction *downCase = toolBar->addAction(tr("Downcase"));
  connect(downCase, SIGNAL(triggered()), this, SLOT(onDownCase()));    
  
  QFont txtFont("Courier");
  
  cipherPad = new QTextEdit;
  cipherPad->setFont(txtFont);
  connect(cipherPad, SIGNAL(textChanged()), this, SLOT(onCiphertextChanged()));
  
  scratchPad = new QTextEdit;
  scratchPad->setFont(txtFont);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->setContentsMargins(5, 5, 5, 5);
  layout->addWidget(toolBar);  
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

  QMenu *freqMenu = analysisMenu->addMenu(tr("Frequency Distribution"));  

  QAction *mono = freqMenu->addAction(tr("Monographic"));
  mono->setStatusTip(tr("Show a monographic monouency distribution of the ciphertext."));
  mono->setShortcut(QKeySequence(META + "+M"));
  connect(mono, SIGNAL(triggered()), this, SLOT(onMonographicDistribution()));
  analysisActions.append(mono);

  QAction *digraph = freqMenu->addAction(tr("Digraphic"));
  digraph->setStatusTip(tr("Show a digraphic distribution of the ciphertext."));
  digraph->setShortcut(QKeySequence(META + "+D"));  
  connect(digraph, SIGNAL(triggered()), this, SLOT(onDigraphicDistribution()));
  analysisActions.append(digraph);

  QAction *trigraph = freqMenu->addAction(tr("Trigraphic"));
  trigraph->setStatusTip(tr("Show a trigraphic distribution of the ciphertext."));
  trigraph->setShortcut(QKeySequence(META + "+T"));  
  connect(trigraph, SIGNAL(triggered()), this, SLOT(onTrigraphicDistribution()));
  analysisActions.append(trigraph);  

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

  QMenu *ciphersMenu = transMenu->addMenu(tr("Ciphers"));  
  
  QAction *affine = ciphersMenu->addAction(tr("Affine"));
  affine->setStatusTip(tr("Do an Affine transformation of the ciphertext."));
  affine->setShortcut(QKeySequence(META + "+A"));
  connect(affine, SIGNAL(triggered()), this, SLOT(onAffineTransformation()));
  transActions.append(affine);

  QAction *mixedSeq = transMenu->addAction(tr("Keyword-mixed sequence"));
  mixedSeq->setStatusTip(tr("Use a keyword-mixed sequence."));
  mixedSeq->setShortcut(QKeySequence(META + "+K"));
  connect(mixedSeq, SIGNAL(triggered()), this, SLOT(onKeywordMixedSequence()));  
  transActions.append(mixedSeq);  

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
    ciphertext = ciphertext.remove(whitespace);
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
    
  saveAct->setEnabled(true);
  saveAsAct->setEnabled(true);        
  filePath = theFile;

  setRestoreName();
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

  setRestoreName();  
}

void MainWindow::setRestoreName() {
  // If the base name is longer than 20 characters (plus the
  // .extension) then trim it, so that 10 characters from the left
  // and 10 characters from the right (plus the extension length)
  // remains.
  QString baseName = QFileInfo(filePath).fileName();
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
}

void MainWindow::employSubstitutionAlphabet(SubstitutionAlphabet *subst,
                                            bool decipher, bool dump) {
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

  scratchPad->setText(out);  
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

  if (txt.size() > 0 && filePath.size() == 0) {
    saveAct->setEnabled(true);
  }
}

void MainWindow::onMonographicDistribution() {
  FreqMap dist;
  foreach (QChar c, alphabet.getAlphabet()) {
    dist[c] = 0;
  }

  dist = polygraphicDistribution(1, getCiphertext(), whitespace, dist);

  FrequencyDialog diag(dist);
  diag.exec();
}

void MainWindow::onDigraphicDistribution() {
  FreqMap dist = polygraphicDistribution(2, getCiphertext(), whitespace);

  FrequencyDialog diag(dist);
  diag.exec();
}

void MainWindow::onTrigraphicDistribution() {
  FreqMap dist = polygraphicDistribution(3, getCiphertext(), whitespace);

  FrequencyDialog diag(dist);
  diag.exec();
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
    employSubstitutionAlphabet(subst, decipher, dump);
    delete subst;
  }
}

void MainWindow::onKeywordMixedSequence() {
  KeywordMixedSequenceDialog diag;
  if (diag.exec() == QDialog::Accepted) {
    QString keyword = diag.getKeyword();
    bool columnar = diag.useColumnarTransposition();
    bool decipher = diag.doDeciphering();    
    bool dump = diag.doDump();

    SubstitutionAlphabet *subst =
      keywordMixedSequence(keyword, columnar, alphabet.getAlphabet());
    employSubstitutionAlphabet(subst, decipher, dump);
    delete subst;    
  }
}

void MainWindow::onCopyDown() {
  scratchPad->setText(cipherPad->toPlainText());
}

void MainWindow::onCopyUp() {
  cipherPad->setText(scratchPad->toPlainText());  
}

void MainWindow::onUpCase() {
  cipherPad->setText(cipherPad->toPlainText().toUpper());
}

void MainWindow::onDownCase() {
  cipherPad->setText(cipherPad->toPlainText().toLower());    
}
