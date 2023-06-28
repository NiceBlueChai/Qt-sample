#ifndef NAVIGATION_PANEL_H
#define NAVIGATION_PANEL_H

#include "global/nbc.h"
#include <QFrame>
#include <functional>
class NavigationWidget;
class NavigationPanelPrivate;
class NBC_EXPORT NavigationPanel : public QFrame
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(NavigationPanel)
public:
    NavigationPanel(QWidget* parent = nullptr);
    virtual ~NavigationPanel();

    enum NavigationDisplayMode {
        MINIMAL,
        COMPACT,
        EXPAND,
        MENU,
    };
    enum NavigationItemPosition {
        TOP,
        SCROLL,
        BOTTOM,
    };

    Q_ENUM(NavigationDisplayMode)
    Q_ENUM(NavigationItemPosition)

public:
    void addSeparator(NavigationItemPosition position = NavigationItemPosition::TOP);
    void addSpacing(int spacing, NavigationItemPosition position = NavigationItemPosition::TOP);
    void addWidget(const QString& routeKey, NavigationWidget* w, NavigationItemPosition position = NavigationItemPosition::TOP);
    void expand();
    void compact();
    void toggle();
    int getSpacing() const;
    QMargins getMargins() const;
    QString getCurrentRouteKey() const;
    NavigationWidget* getCurrentItem();
    void clearSelection();

    typedef std::function<void(bool)> ClickFuncType; // bool userTrigger
    void onClicked(const QString& routeKey, ClickFuncType func);

public slots:
    void forward(); // 前进
    void back();    // 后退
    void setItemHeight(int h);
    void setCompactWidth(int w);
    void setExpandWidth(int w);
    void setCurrentItem(const QString& routeKey);
    void setSpacing(int spacing);
    void setMargins(int left, int top, int right, int bottom);

private:
    void addWidgetToLayout(NavigationWidget* w, NavigationItemPosition position);

signals:
    void displayModeChanged(NavigationDisplayMode);
    void aboutToClicked(const QString& routeKey, bool userTrigger = true); // before clicked hook
    void clicked(const QString& routeKey, bool userTrigger = true);
    void currentItemChanged(const QString& routeKey);

private:
    NavigationPanelPrivate* d_ptr = nullptr;
};


class NBC_EXPORT NavigationHistory : public QObject {
    Q_OBJECT
public:
    NavigationHistory(QMap<QString, NavigationWidget*>& items, QObject* parent = nullptr);

public:
    void push(const QString& routeKey);
    void setMaxCounts(int v); // 设置最大历史记录数量， 默认100
    int getMaxCounts() const;
    bool isBackEmpty() const;
    bool isForwardEmpty() const;

public slots:
    void forward(); // 前进
    void back();    // 后退

private:
    void navigate_back();
    void navigate_forwark();

signals:
    void forwardEmptyChanged(bool);// 历史记录是否为空
    void backEmptyChanged(bool);

private:
    QMap<QString, NavigationWidget*>& items_;
    QList<QString> back_history_;
    QList<QString> forward_history_;
    int max_counts_;
};

#endif // NAVIGATION_PANEL_H
