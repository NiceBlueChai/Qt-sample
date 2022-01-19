#ifndef NOTIFYMANAGER_H
#define NOTIFYMANAGER_H
#include <QPushButton>
#include <QQueue>
#include <QStyle>
#include <QWidget>
/**
 * @brief: 右下角通知气泡
 * 一般不单独使用, 使用NotifyManager的实例进行管理
 * @author: NiceBlueChai
 * @email: bluechai@qq.com
 * @date:  2020-01-04
 **/
class Notify : public QWidget {
    Q_OBJECT
public:
    Notify(int showTime, QWidget *parent);

    Notify(QWidget *parent,
           const QString &title,
           const QString &context,
           const QPixmap &pixmap = QPixmap(),
           int showTime = 0);

    Notify(QWidget *parent,
           const QString &title,
           const QString &context,
           const QString &svg_path,
           int showTime = 0);

    Notify(const Notify &) = delete;
    Notify &operator=(const Notify &) = delete;

    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

    QString getTitle() const;
    QString getContext() const;
    QPixmap getPixmap() const;

    // 设置显示时间
    void setShowTime(int value);
    // 设置标题
    void setTitle(const QString &value);
    // 设置正文
    void setContext(const QString &value);
    // 设置图标
    void setPixmap(const QPixmap &value);

Q_SIGNALS:
    // 点击标签
    void clicked();
    // 点了关闭按钮
    void canceled();
    void closed();

private slots:
    void onCloseAnimationFinished(QCloseEvent *event);

protected:
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    void drawBg(QPainter *painter);
    void drawText(QPainter *painter);
    void drawIcon(QPainter *painter);
    void init();

private:
    bool pressed;
    int showTime;
    QString title;
    QString context;
    QPixmap pixmap;
    QPushButton btnClose;
};

/**
 * @brief The NotifyManager class
 * 管理Notify的位置,创建与销毁
 * @author: NiceBlueChai
 * @date: 2020/1/4
 */
class QTimerEvent;
class NotifyManager : public QObject {
    Q_OBJECT
public:
    NotifyManager(QObject *parent);
    void notify(QWidget *parent, const QString &title,
        const QString &context, int showTime = 5000, const QPixmap &pixmap = QPixmap());
    void notify(QWidget *parent, const QString &title, const QString &context, const QString &svg_path, int showTime = 5000);
    void setMaxCount(int value);

protected:
    virtual void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void onDestroyed();

private:
    void showNext();
    QQueue<Notify *> m_queue;
    QList<Notify *> m_list;
    int maxCount;
    int timerId;
};

#endif // NOTIFYMANAGER_H
