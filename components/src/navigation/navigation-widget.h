#ifndef NAVIGATION_WIDGET_H
#define NAVIGATION_WIDGET_H

#include <QWidget>
#include "global/nbc.h"

class NavigationWidgetPrivate;
class NBC_EXPORT NavigationWidget : public QWidget
{
    Q_DECLARE_PRIVATE(NavigationWidget)
    Q_PROPERTY(QColor bgColor READ getBgColor WRITE setBgColor)
    Q_PROPERTY(QColor hoverBgColor READ getHoverBgColor WRITE setHoverBgColor)
    Q_PROPERTY(QColor selectedBgColor READ getSelectedBgColor WRITE setSelectedBgColor)
    Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)
    Q_PROPERTY(int fontSize READ getFontSize WRITE setFontSize)
    Q_OBJECT
public:
    explicit NavigationWidget(QWidget *parent = nullptr, bool selectable = false);
    virtual ~NavigationWidget() override;
    int getExpandWidth() const;
    int getCompactWidth() const;
    void setCompactWidth(int w);
    void setExpandWidth(int w);
    bool isCompacted() const;
    bool isSelected() const;
    bool isSelectable() const;
    bool isClickable() const;
    QColor getBgColor() const;

    QColor getHoverBgColor() const;

    QColor getSelectedBgColor() const;

    QColor getTextColor() const;
    int getFontSize() const;


signals:
    void clicked(bool);

public slots:
    virtual void setCompacted(bool isCompacted);
    void setSelected(bool isSelected);
    void setSelectable(bool selectable);
    void setBgColor(const QColor& color);
    void setHoverBgColor(const QColor& color);
    void setSelectedBgColor(const QColor& color);
    void setTextColor(const QColor &color);
    void setFontSize(int value);
    void setClickable(bool b);

protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

protected:
    NavigationWidget(NavigationWidgetPrivate &dd, QWidget* parent);
    NavigationWidgetPrivate* d_ptr = nullptr;
};

class NavigationPushButtonPrivate;
class NBC_EXPORT NavigationPushButton : public NavigationWidget {
    Q_DECLARE_PRIVATE(NavigationPushButton)
    Q_OBJECT
public:
    explicit NavigationPushButton(QWidget* parent = nullptr);
    NavigationPushButton(const QString& text, const QString& icon, bool isSelectable = true, QWidget*parent = nullptr);
    void setText(const QString& text);
    void setIcon(const QString& filePath);
    void setIconSize(const QSizeF& size);
    QSizeF getIconSize() const;
    QString getText() const;
    QString getIcon() const;
    void setIndicatorVisibale(bool v);
    bool getIndicatorVisibale() const;
    void setTextAlign(Qt::Alignment flag);

protected:
    void paintEvent(QPaintEvent* e) override;
    virtual void paintIcon(QPainter& painter);
    virtual void paintIndicator(QPainter& painter);
    virtual void paintText(QPainter& painter);
    NavigationPushButton(NavigationPushButtonPrivate &dd, QWidget* parent);
};

class NBC_EXPORT RotateButton : public NavigationPushButton {
public:
    using NavigationPushButton::NavigationPushButton;

protected:
    void timerEvent(QTimerEvent*) override;

    void paintIcon(QPainter& painter) override;
private:
    int timer_id_ = 0;
    int rotate_ = 0;
    bool is_enter_ = false;

    // QWidget interface
protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
};

class NBC_EXPORT NavigationSeparator:public NavigationWidget {
    Q_OBJECT
public:
    NavigationSeparator(QWidget* parent = nullptr);

public:
    virtual void setCompacted(bool isCompacted) override;

protected:
    void paintEvent(QPaintEvent*) override;
};

class NavigationToolButtonPrivate;
class NBC_EXPORT NavigationToolButton: public NavigationPushButton {
    Q_DECLARE_PRIVATE(NavigationToolButton)
    Q_OBJECT
    Q_PROPERTY(Qt::ToolButtonStyle toolButtonStyle READ toolButtonStyle WRITE setToolButtonStyle)
    Q_PROPERTY(qreal topPadding READ getTopPadding WRITE setTopPadding)
public:
    explicit NavigationToolButton(QWidget* parent = nullptr);
    NavigationToolButton(const QString& text, const QString &icon, QWidget* parent=nullptr);

    Qt::ToolButtonStyle toolButtonStyle() const;
    qreal getTopPadding() const;

public Q_SLOTS:
    void setToolButtonStyle(Qt::ToolButtonStyle style);
    void setTopPadding(qreal v);

protected:
    virtual void paintEvent(QPaintEvent *e) override;
    virtual void paintIcon(QPainter &painter) override;
    virtual void paintText(QPainter &painter) override;
};


#endif // NAVIGATION_WIDGET_H
