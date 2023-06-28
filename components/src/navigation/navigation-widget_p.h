#ifndef NAVIGATION_WIDGET_P_H
#define NAVIGATION_WIDGET_P_H

#include "global/nbc.h"
#include <QColor>
#include <QString>
#include <QSizeF>

class NavigationWidget;
class NavigationPushButton;
class NavigationToolButton;

class NBC_EXPORT NavigationWidgetPrivate {
    Q_DECLARE_PUBLIC(NavigationWidget)
public:
    NavigationWidgetPrivate();
    virtual ~NavigationWidgetPrivate();

    NavigationWidget* q_ptr = nullptr;
    bool is_compacted_ = true;
    bool is_selected_ = false;
    bool is_pressed_ = false;
    bool is_enter_ = false;
    bool is_selectable_ = true;
    bool is_clickable_ = true;
    int expand_width_ ;
    int compact_width_ ;
    QColor bg_color_;
    QColor hover_bg_color_;
    QColor selected_bg_color_;
    QColor text_color_;
    int font_size_;
};


class NBC_EXPORT NavigationPushButtonPrivate : public NavigationWidgetPrivate {
    Q_DECLARE_PUBLIC(NavigationPushButton)
public:
    NavigationPushButtonPrivate();
    QString icon_;
    QString text_;
    QSizeF icon_size_;
    Qt::Alignment alignment_;
    bool indicator_visibale_ = false;
};

class NBC_EXPORT NavigationToolButtonPrivate : public NavigationPushButtonPrivate {
public:
    NavigationToolButtonPrivate(NavigationToolButton* q);
    void init();
    Qt::ToolButtonStyle toolButtonStyle;
    qreal top_padding_;
};

#endif // NAVIGATION_WIDGET_P_H
