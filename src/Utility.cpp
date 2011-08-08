#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>

#include "Utility.h"

void centerWidget(QWidget *widget) {
  QDesktopWidget *desktop = QApplication::desktop();
  QRect rect = desktop->availableGeometry(desktop->primaryScreen());
  widget->move((rect.width() - widget->width()) / 2,
               (rect.height() - widget->height()) / 2);  
}
