#include "notifymanager.h"
#include <QApplication>
#include <QScreen>
#include <QSvgRenderer>
#include <QGraphicsEffect>
#include <QMouseEvent>
#include <QTimerEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QTimer>
#include <QImage>

namespace {
const int kIconSize = 64;
const int kClostBtnSize = 24;
const int kMaxNotifyCount = 6;
const int kCheckQueueInterval = 500;
}


Notify::Notify(int showTime, QWidget *parent)
    : QWidget(parent),
      pressed(false),
      showTime(showTime)
{
    init();
}

Notify::Notify(QWidget *parent,
    const QString &title,
    const QString &context,
    const QPixmap &pixmap,
    int showTime)
    : QWidget(parent),
      pressed(false),
      showTime(showTime),
      title(title),
      context(context),
      pixmap(pixmap)
{
    init();
}

Notify::Notify(QWidget *parent, const QString &title, const QString &context, const QString &svg_path, int showTime)
    :Notify(parent, title, context, QPixmap(), showTime)
{
    QImage image(kIconSize, kIconSize, QImage::Format_RGB32);
    QPainter painter(&image);
//    painter.fillRect(image.rect(), QColor("white"));
    QSvgRenderer sr(svg_path);
    sr.render(&painter);
    painter.end();
    pixmap = QPixmap::fromImage(image);
}

QSize Notify::sizeHint() const
{
    return QSize(300, 100);
}

QString Notify::getTitle() const {
    return title;
}

QString Notify::getContext() const {
    return context;
}

QPixmap Notify::getPixmap() const {
    return pixmap;
}

void Notify::setShowTime(int value)
{
    showTime = value;
}

void Notify::setTitle(const QString &value)
{
    title = value;
}

void Notify::setContext(const QString &value)
{
    context = value;
}

void Notify::setPixmap(const QPixmap &value)
{
    pixmap = value;
}

void Notify::onCloseAnimationFinished(QCloseEvent *event)
{
    emit closed();
    QWidget::closeEvent(event);
}

void Notify::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        pressed = true;
        update();
    }
}

void Notify::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && pressed) {
        pressed = false;
        emit clicked();
        this->close();
    }
}

void Notify::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    // 设置画笔抗锯齿
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
//    painter.setBrush(Qt::NoBrush);
//    painter.setPen(Qt::NoPen);
    // 绘制背景
    drawBg(&painter);
    // 绘制文字
    drawText(&painter);
    // 绘制图标
    drawIcon(&painter);
}

// 显示动画
void Notify::showEvent(QShowEvent *event)
{
    if (showTime > 0)
        QTimer::singleShot(showTime + 300, this, SLOT(close()));
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry", this);
    animation->setDuration(300);
    animation->setStartValue(QRect(pos(), QSize(0, 0)));
    animation->setEndValue(QRect(pos(), this->sizeHint()));
    animation->setEasingCurve(QEasingCurve::InOutBounce);
    connect(animation, &QPropertyAnimation::finished, this, [=]() { animation->deleteLater(); });
    animation->start();
    QWidget::showEvent(event);
}

// 关闭动画
void Notify::closeEvent(QCloseEvent *event)
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry", this);
    animation->setDuration(200);
    animation->setStartValue(this->geometry());
    QPoint pos = this->geometry().bottomRight();
    animation->setEndValue(QRect(pos, pos));
    animation->setEasingCurve(QEasingCurve::InCirc);
    animation->start();
    // Linux下GCC不支持lambda函数中调用基类的protected成员
#if defined Q_OS_LINUX
    connect(animation, &QPropertyAnimation::finished, this, [=]() {
        animation->deleteLater();
        onCloseAnimationFinished(event);
    });
#elif defined Q_OS_WIN
    connect(animation, &QPropertyAnimation::finished, this, [=]() {
		animation->deleteLater();
		emit closed();
                QWidget::closeEvent(event);
    });
#endif
    event->ignore();
}

void Notify::drawBg(QPainter *painter)
{
    painter->save();
    QPen pen;
    pen.setWidthF(0.5);
    pen.setColor(Qt::black);
    painter->setPen(pen);
    QColor color;
    if (pressed)
        color = QColor(220, 220, 255, 220);
    else
        color = QColor(240, 240, 250, 200);
    painter->setBrush(color);
    painter->drawRoundedRect(this->rect().adjusted(1, 1, -1, -1), 5, 5);
    painter->restore();
}

// TODO: 省略多余的文字
void Notify::drawText(QPainter *painter)
{
    painter->save();
    QRect rect = this->rect().adjusted(100, 5, -5, -5);
    QPen pen;
    pen.setColor(Qt::black);
    QFont font;
    font.setPixelSize(16);
    painter->setPen(pen);
    painter->setFont(font);
    // context
    painter->drawText(rect.adjusted(0, 45, 0, 0), Qt::AlignVCenter, context);
    // title
    font.setBold(true);
    font.setPixelSize(16);
    painter->setPen(pen);
    painter->setFont(font);
    painter->drawText(rect.adjusted(0, 15, 0, -50), Qt::AlignVCenter, title);
    painter->restore();
}

void Notify::drawIcon(QPainter *painter)
{
    painter->save();
    painter->setOpacity(1);
    painter->drawPixmap(20, (height() - kIconSize)/2, kIconSize, kIconSize, pixmap);
    painter->restore();
}

void Notify::init()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    this->setWindowFlag(Qt::WindowSystemMenuHint);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
//        setWindowOpacity(0.9);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnClose.setObjectName("close-btn");
    btnClose.setText("X");
    btnClose.setParent(this);
    btnClose.setFixedSize(kClostBtnSize, kClostBtnSize);
    btnClose.setStyleSheet("QPushButton{"
                      "border:none;"
                      "font:16px bold;"
                      "background:rgba(255, 255, 255, 0);"
                      "}"
                      "QPushButton:hover{"
                      "background:rgba(100, 234, 255, 200)"
                      "}");
    btnClose.move(300 - 26, 1);
    connect(&btnClose, &QPushButton::clicked, this, [this]() {
        emit canceled();
        close();
    });
}

NotifyManager::NotifyManager(QObject *parent)
    : maxCount(kMaxNotifyCount), timerId(0)
{

}

void NotifyManager::notify(QWidget *parent, const QString &title, const QString &context, int showTime, const QPixmap &pixmap)
{
    auto pix = pixmap;
    if (pix.isNull()) {
        QStyle *style = QApplication::style();
        pix = style->standardPixmap(QStyle::SP_MessageBoxInformation);
    }
    auto notify = new Notify(parent, title, context, pix, showTime);
    m_queue.push_back(notify);
}

void NotifyManager::notify(QWidget *parent, const QString &title, const QString &context, const QString &svg_path, int showTime)
{
    auto notify = new Notify(parent, title, context, svg_path, showTime);
    m_queue.push_back(notify);
    // 线程不安全的
    if (0 == timerId) {
        timerId = startTimer(kCheckQueueInterval);
    }
}

void NotifyManager::setMaxCount(int value)
{
    maxCount = value;
}

void NotifyManager::timerEvent(QTimerEvent* event) {
    if (event->timerId() == timerId) {
        if (m_queue.isEmpty()) {
            killTimer(timerId);
            timerId = 0;
        } else if(m_list.count() < maxCount) {
            showNext();
        }
    }
}

void NotifyManager::onDestroyed()
{
    Notify *notify = static_cast<Notify *>(sender());
    //    assert(notify != nullptr);
    m_list.removeOne(notify);
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
    auto screen = QApplication::primaryScreen();
    QRect rect = screen->availableGeometry();

    int w = rect.width();
    int h = rect.height();
    QPoint firstPos = QPoint(w - 320, h - 120);
    int count = m_list.length();
    for (const auto &tmp_notify : qAsConst(m_list)) {
        int index = m_list.indexOf(tmp_notify);
        QPoint pos = firstPos - QPoint(0, (count - index - 1) * 120);
        tmp_notify->move(pos);
    }
    if (notify)
        notify->show();
}
