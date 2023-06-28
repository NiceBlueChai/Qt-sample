#include "drag-win-eventfilter.h"
#include <QMouseEvent>
#include <QWidget>
#include <QApplication>

DragWinEventFilter::DragWinEventFilter(QObject *parent)
    :QObject(parent)
{

}

DragWinEventFilter::DragWinEventFilter(QWidget *drage, QWidget *target)
    :QObject(drage),enable_drage_widget_(drage), target_(target)
{

}

DragWinEventFilter::~DragWinEventFilter()
{

}

void DragWinEventFilter::setTargetWindow(QWidget *target)
{
    target_ = target;
}

QWidget *DragWinEventFilter::getTargetWindow() const
{
    return target_;
}

bool DragWinEventFilter::isMaximized() const
{
    if (!target_)
        return false;
    return target_->isMaximized();
}

bool DragWinEventFilter::isFullScreen() const
{
    if (!target_)
        return false;
    return target_->isFullScreen();
}

bool DragWinEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    if (!target_ || !enable_drage_widget_) {
        return QObject::eventFilter(watched, event);
    }
    if (event->type() == QEvent::MouseMove){
        if (isMaximized() || isFullScreen())
            return QObject::eventFilter(watched, event);
        QMouseEvent *pMouse = dynamic_cast<QMouseEvent *>(event);
        if (pMouse) {
            if (watched == enable_drage_widget_ && m_bMousePressedTitle &&
                m_bEnableDrag && enable_drage_widget_->cursor().shape() == Qt::CursorShape::ArrowCursor) {
                target_->move(m_wndPos + (pMouse->globalPos() - m_mousePos));
            }
        }
    } else if (event->type() == QEvent::MouseButtonPress) {
        auto* pMouse = static_cast<QMouseEvent*>(event);
        if (watched == enable_drage_widget_) {
            m_bMousePressedTitle = true;
            m_mousePos = pMouse->globalPos();
            m_wndPos = target_->pos();
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        if (watched == enable_drage_widget_)
            m_bMousePressedTitle = false;
    }
    return QObject::eventFilter(watched, event);
}
