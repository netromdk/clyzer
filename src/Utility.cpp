#include <QDebug>
#include <QWidget>
#include <QPainter>
#include <QStringList>
#include <QApplication>
#include <QFontMetrics>
#include <QDesktopWidget>

#include <cmath>

#include "Utility.h"
#include "SubstitutionAlphabet.h"

void centerWidget(QWidget *widget) {
  QDesktopWidget *desktop = QApplication::desktop();
  QRect rect = desktop->availableGeometry(desktop->primaryScreen());
  widget->move((rect.width() - widget->width()) / 2,
               (rect.height() - widget->height()) / 2);  
}

QPixmap generateFrequencyPixmap(const FreqMap &map, int height) {
  QFont font("Helvetica");
  font.setBold(true);
  font.setPixelSize(12);

  QFont font2("Helvetica");
  font2.setPixelSize(9);  
  
  // Calculate the width and height of the picture.
  int width = 0, lineHeight, step = 15, tot = 0, max = 0;
  QFontMetrics fm(font);
  foreach (QString key, map.keys()) {
    width += fm.width(key) + step;
    lineHeight = fm.height();
    
    int val = map[key];
    if (val > max) {
      max = val;
    }
    tot += val;
  }
  width += step;

  QFontMetrics fm2(font2);
  fm2.width("A");
  int lh2 = fm2.height();

  QPixmap pix(width, height);
  pix.fill();

  // If there's nothing to analyze then just return.
  if (max == 0) return pix;  

  // Determine the amount of pixels pr. count of one
  // letter-occurrence.
  int pxCnt = (height - lineHeight - lh2 - (2 * step)) / max;      

  QPainter painter(&pix);
  painter.setFont(font);

  int x = step, y = lineHeight + step / 2;
  foreach (QString key, map.keys()) {
    width = fm.width(key);
    painter.setFont(font);        
    painter.drawText(x, y, key);

    int val = map[key];
    double pct = 0.0;
    if (val > 0) {
      pct = double(val) / double(tot) * 100.0;
    }
    
    QString str = QString::number(pct, 'f', 1);
    int w = fm2.width(str),
      lh = fm2.height(),
      x0 = x - w / 4,
      y0 = y + lh + step / 2;
    painter.setFont(font2);
    painter.drawText(x0, y0, str);    

    int x1 = x + width / 2,
      y1 = y0 + step / 2,
      h = y1 + pxCnt * map[key];
    if (h - y1 > 0) {
      painter.drawLine(x1, y1, x1, h);
    }

    x += width + step;
  }

  return pix;
}

SubstitutionAlphabet *keywordMixedSequence(const QString &keyword,
                                           bool columnar,
                                           const QString &alphabet) {
  QString ciph = "";

  // Write the keyword on the row whatnot duplicates.
  int len = 0;
  foreach (QChar c, keyword) {
    if (!ciph.contains(c)) {
      ciph += c;
    }
  }
  len = ciph.size();

  // Continue the alphabet.
  foreach (QChar c, alphabet) {
    if (!ciph.contains(c)) {
      ciph += c;
    }
  }

  // Columnar transposition.
  QString cipherAlph = ciph;
  if (columnar) {
    cipherAlph = "";
    int amount = ceil((float) alphabet.size() / len);
    QStringList lines;
    for (int i = 0; i < amount; i++) {
      int left = alphabet.size() - (i * amount);
      int ln = len;
      if (ln > left) {
        ln = left;
      }
      lines.append(ciph.mid(i * len, ln));
    }

    for (int i = 0; i < len; i++) {
      foreach (QString line, lines) {
        if (i >= line.size()) continue;

        cipherAlph += line[i];
      }
    }
  }
  
  return new SubstitutionAlphabet(alphabet, cipherAlph);
}
