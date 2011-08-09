#include <QDebug>
#include <QWidget>
#include <QPainter>
#include <QApplication>
#include <QFontMetrics>
#include <QDesktopWidget>

#include "Utility.h"

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
  int width = 0, lineHeight, step = 10, tot = 0, max = 0, pxCnt;
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
  width += 2 * step;

  QFontMetrics fm2(font2);
  fm2.width("A");
  int lh2 = fm2.height();

  // Determine the amount of pixels pr. count of one
  // letter-occurrence.
  pxCnt = (height - lineHeight - lh2 - (2 * step)) / max;

  QPixmap pix(width, height);
  pix.fill();

  QPainter painter(&pix);
  painter.setFont(font);

  int x = step, y = lineHeight + step / 2;
  foreach (QString key, map.keys()) {
    width = fm.width(key) + step / 2;
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

    int x1 = x0 + width / 2,
      y1 = y0 + step,
      h = y1 + pxCnt * map[key];
    if (h - y1 > 0) {
      painter.drawLine(x1, y1, x1, h);
    }

    x += width + step;
  }

  return pix;
}
