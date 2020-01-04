#include "notifymanager.h"
#include <QApplication>
#include <QDesktopWidget>

NotifyManager::NotifyManager(QObject *parent)
    : maxCount(6)
{
}

void NotifyManager::notify(const QString &title, const QString &context, int showTime, const QPixmap &pixmap)
{
    auto pix = pixmap;
    if (pix.isNull()) {
        QStyle *style = QApplication::style();
        pix = style->standardPixmap(QStyle::SP_MessageBoxInformation);
    }
    auto notify = new Notify(nullptr, title, context, pix, showTime);
    m_queue.push_back(notify);
    showNext();
}

void NotifyManager::setMaxCount(int value)
{
    maxCount = value;
}

void NotifyManager::onDestroyed()
{
    Notify *notify = static_cast<Notify *>(sender());
//    assert(notify != nullptr);
    for (auto n : m_list) {
        if (notify == n) {
            int index = m_list.indexOf(n);
            m_list.takeAt(index)->deleteLater();
            break;
        }
    }
    showNext();
}

void NotifyManager::showNext()
{
    Notify *notify = nullptr;
    if (m_list.length() == maxCount)
        return;
    if (m_queue.length() > 0) {
        notify = m_queue.first();
        m_queue.pop_front();
        connect(notify, &Notify::closed, this, &NotifyManager::onDestroyed);
        m_list.push_back(notify);
    }
    // 调整位置
    // TODO: 动画
    QDesktopWidget *desktop = QApplication::desktop();
    QRect rect = desktop->availableGeometry();
    int w = rect.width();
    int h = rect.height();
    QPoint firstPos = QPoint(w - 320, h - 120);
    int count = m_list.length();
    for (auto n : m_list) {
        int index = m_list.indexOf(n);
        QPoint pos = firstPos - QPoint(0, (count - index - 1) * 120);
        n->move(pos);
    }
    if (notify)
        notify->show();
}
