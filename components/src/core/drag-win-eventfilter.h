#ifndef EDMS_DRAG_WIN_EVENTFILTER_H
#define EDMS_DRAG_WIN_EVENTFILTER_H

#include "global/nbc.h"
#include <QObject>
#include <QPoint>
class QWidget;

class NBC_EXPORT DragWinEventFilter : public QObject {
    Q_OBJECT
public:
    explicit DragWinEventFilter(QObject* parent = nullptr);
    explicit DragWinEventFilter(QWidget* drage, QWidget* target);
    ~DragWinEventFilter() override;
    void setTargetWindow(QWidget* target);
    QWidget* getTargetWindow() const;

private:
    bool isMaximized() const;
    bool isFullScreen() const;

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QWidget* enable_drage_widget_ = nullptr;
    QWidget* target_ = nullptr;
    QPoint m_mousePos;
    QPoint m_wndPos;
    QPoint m_lastPos;
    bool m_bMousePressed = false;
    bool m_bMousePressedTitle = false;
    bool m_bEnableDrag = true; // 能否拖动
};

#endif  // EDMS_DRAG_WIN_EVENTFILTER_H
