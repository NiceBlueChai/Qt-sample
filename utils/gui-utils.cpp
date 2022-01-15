#include <QtGlobal>
#ifdef Q_OS_WIN
#pragma execution_character_set("utf-8")
#endif
#include "gui-utils.h"
#include <QApplication>
#include <QScreen>
#include <QWidget>

void nbc::utils::moveToScreenCenter(QWidget &w)
{
    auto rect = w.rect();
    w.move(QApplication::primaryScreen()->availableGeometry().center() - QPoint(rect.width()/2, rect.height()/2));
}
