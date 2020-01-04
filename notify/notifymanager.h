#ifndef NOTIFYMANAGER_H
#define NOTIFYMANAGER_H

/**
 * @brief The NotifyManager class
 * 管理Notify的位置,创建与销毁
 * @author: NiceBlueChai
 * @date: 2020/1/4
 */

#include "notify.h"
#include <QObject>
#include <QQueue>
#include <QStyle>

class NotifyManager : public QObject {
    Q_OBJECT
public:
    NotifyManager(QObject *parent = 0);
    void notify(const QString &title,
        const QString &context, int showTime = 5000, const QPixmap &pixmap = QPixmap());
    void setMaxCount(int value);

private Q_SLOTS:
    void onDestroyed();

private:
    void showNext();
    QQueue<Notify *> m_queue;
    QList<Notify *> m_list;
    int maxCount;
};

#endif // NOTIFYMANAGER_H
