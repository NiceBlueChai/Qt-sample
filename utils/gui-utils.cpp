#include <QtGlobal>
#ifdef Q_OS_WIN
#pragma execution_character_set("utf-8")
#endif
#include "gui-utils.h"
#include <QApplication>
#include <QScreen>
#include <QWidget>
#include <QStyle>

QPoint nbc::utils::getWindowTopLeftPosCenterOfScreen(int width, int height, bool hasTitleBar) {
    auto screenRect = QApplication::primaryScreen()->availableGeometry();

    if (width > screenRect.width() || height>screenRect.height()) {
        return QPoint();
    }

    auto topLeftPos = screenRect.center() - QPoint(width/2, height/2);
    if (hasTitleBar) {
        int titleBarHeight = QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight);
        topLeftPos -= QPoint(0, titleBarHeight);
    }
    return topLeftPos;
}

inline QPoint nbc::utils::getWindowTopLeftPosCenterOfScreen(const QWidget& w) {
    return getWindowTopLeftPosCenterOfScreen(w.width(), w.height());
}

void nbc::utils::moveToScreenCenter(QWidget &w)
{
    auto topLeftPos = getWindowTopLeftPosCenterOfScreen(w);
    w.move(topLeftPos);
}
