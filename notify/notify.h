#ifndef NOTIFY_H
#define NOTIFY_H
/**
 * @brief: 右下角通知气泡
 * 一般不单独使用, 使用NotifyManager的实例进行管理
 * @author: NiceBlueChai
 * @email: bluechai@qq.com
 * @date:  2020-01-04
 **/
#include <QPushButton>
#include <QWidget>

class Notify : public QWidget {
    Q_OBJECT
public:
    explicit Notify(int showTime = 0, QWidget *parent = nullptr);
    explicit Notify(QWidget *parent, const QString &title,
        const QString &context, const QPixmap &pixmap = QPixmap(),
        int showTime = 0);

    Notify(const Notify &) = delete;
    Notify &operator=(const Notify &) = delete;

    QSize sizeHint() const;

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
    // QWidget interface
private slots:
    void onCloseAnimationFinished(QCloseEvent* event);
protected:
    virtual void mousePressEvent(QMouseEvent *event)override;
    virtual void mouseReleaseEvent(QMouseEvent *event)override;
    virtual void paintEvent(QPaintEvent *event)override;
    virtual void showEvent(QShowEvent *event)override;
    virtual void closeEvent(QCloseEvent *event) override;

private:
    void drawBg(QPainter *painter);
    void drawText(QPainter *painter);
    void drawIcon(QPainter *painter);
    inline void init();

private:
    bool pressed;
    int showTime;
    QString title;
    QString context;
    QPixmap pixmap;
    QPushButton btn;
};

#endif
