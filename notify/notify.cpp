#include "notify.h"

#include <QGraphicsEffect>
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QTimer>

Notify::Notify(int showTime, QWidget *parent)
    : QWidget(parent)
    , showTime(showTime)
    , pressed(false)
{
    init();
}

Notify::Notify(QWidget *parent,
    const QString &title,
    const QString &context,
    const QPixmap &pixmap,
    int showTime)
    : QWidget(parent)
    , title(title)
    , context(context)
    , pixmap(pixmap)
    , showTime(showTime)
    , pressed(false)
{
    init();
}

QSize Notify::sizeHint() const
{
    return QSize(300, 100);
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
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::NoPen);
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
    connect(animation, &QPropertyAnimation::finished, this, [=](){animation->deleteLater(); onCloseAnimationFinished(event);});
//    connect(animation, &QPropertyAnimation::finished, this, [=]() {
//        animation->deleteLater();
//        emit closed();
//        QWidget::closeEvent(event); });
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
    painter->setOpacity(1);
    painter->drawPixmap(26, 26, 48, 48, pixmap);
}

void Notify::init()
{
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::Tool);
    this->setWindowFlag(Qt::WindowSystemMenuHint);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    //    setWindowOpacity(0.9);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btn.setObjectName("close-btn");
    btn.setText("X");
    btn.setParent(this);
    btn.setFixedSize(25, 25);
    btn.setStyleSheet("QPushButton{"
                  "border:none;"
                  "font:16px bold;"
                  "background:rgba(240, 240, 250, 200);"
                  "}"
                  "QPushButton:hover{"
                  "background:rgba(100, 234, 255, 200)"
                  "}");
    btn.move(300 - 26, 1);
    connect(&btn, &QPushButton::clicked, [=]() {
        emit canceled();
        close();
    });
}
