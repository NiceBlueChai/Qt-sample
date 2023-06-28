#include "navigation-widget.h"
#include <QDebug>
#include <QPainter>
#include <QStyle>
#include <QTimerEvent>
#include "navigation-widget_p.h"
#include "utils/paint-utils.h"

namespace {
const int kCompactedWidth = 40;
const int kCompactedHeight = 64;
const int kExpandWidth = 312;
}  // namespace

#if defined(NBC_NAMESPACE)
    using namespace NBC_NAMESPACE;
#endif
using namespace utils;
NavigationWidgetPrivate::NavigationWidgetPrivate()
    : expand_width_(kExpandWidth),
    compact_width_(kCompactedWidth),
    bg_color_(245, 245, 245),
    hover_bg_color_(236, 236, 238),
    selected_bg_color_(216, 216, 218),
    text_color_(255, 255, 255),
    font_size_(14) {

}

NavigationWidgetPrivate::~NavigationWidgetPrivate() {}

NavigationPushButtonPrivate::NavigationPushButtonPrivate()
    : NavigationWidgetPrivate(), icon_size_(16., 16.), alignment_(Qt::AlignCenter) {}

NavigationToolButtonPrivate::NavigationToolButtonPrivate(NavigationToolButton* q)
    : NavigationPushButtonPrivate(), top_padding_(0.) {}

void NavigationToolButtonPrivate::init() {
    toolButtonStyle = Qt::ToolButtonIconOnly;
}

NavigationWidget::NavigationWidget(QWidget* parent, bool selectable)
    : QWidget(parent), d_ptr(new NavigationWidgetPrivate) {
    Q_D(NavigationWidget);
    d_ptr->q_ptr = this;
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    d->is_selectable_ = selectable;
}

NavigationWidget::NavigationWidget(NavigationWidgetPrivate& dd, QWidget* parent)
    : QWidget(parent), d_ptr(&dd) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

NavigationWidget::~NavigationWidget() {
    delete d_ptr;
}

void NavigationWidget::setCompacted(bool isCompacted) {
    Q_D(NavigationWidget);
    if (d->is_compacted_ == isCompacted) {
        return;
    }
    d->is_compacted_ = isCompacted;

    this->update();
}

void NavigationWidget::setSelected(bool isSelected) {
    Q_D(NavigationWidget);
    if (!d->is_selectable_ || d->is_selected_ == isSelected) {
        return;
    }
    d->is_selected_ = isSelected;
    this->update();
}

void NavigationWidget::setSelectable(bool selectable) {
    Q_D(NavigationWidget);
    d->is_selectable_ = selectable;
    update();
}

int NavigationWidget::getExpandWidth() const {
    Q_D(const NavigationWidget);
    return d->expand_width_;
}

int NavigationWidget::getCompactWidth() const {
    Q_D(const NavigationWidget);
    return d->compact_width_;
}
void NavigationWidget::setCompactWidth(int w) {
    Q_D(NavigationWidget);
    if (d->compact_width_ == w)
        return;
    d->compact_width_ = w;
    update();
}

void NavigationWidget::setExpandWidth(int w) {
    Q_D(NavigationWidget);
    if (d->expand_width_ == w)
        return;
    d->expand_width_ = w;
    update();
}

bool NavigationWidget::isCompacted() const {
    Q_D(const NavigationWidget);
    return d->is_compacted_;
}

bool NavigationWidget::isSelected() const {
    Q_D(const NavigationWidget);
    return d->is_selected_;
}

bool NavigationWidget::isSelectable() const {
    Q_D(const NavigationWidget);
    return d->is_selectable_;
}

bool NavigationWidget::isClickable() const {
    Q_D(const NavigationWidget);
    return d->is_clickable_;
}

QColor NavigationWidget::getBgColor() const {
    Q_D(const NavigationWidget);
    return d->bg_color_;
}

void NavigationWidget::setBgColor(const QColor& color) {
    Q_D(NavigationWidget);
    d->bg_color_ = color;
    update();
}

QColor NavigationWidget::getHoverBgColor() const {
    Q_D(const NavigationWidget);
    return d->hover_bg_color_;
}

void NavigationWidget::setHoverBgColor(const QColor& color) {
    Q_D(NavigationWidget);
    d->hover_bg_color_ = color;
    update();
}

QColor NavigationWidget::getSelectedBgColor() const {
    Q_D(const NavigationWidget);
    return d->selected_bg_color_;
}

QColor NavigationWidget::getTextColor() const {
    Q_D(const NavigationWidget);
    return d->text_color_;
}

int NavigationWidget::getFontSize() const {
    Q_D(const NavigationWidget);
    return d->font_size_;
}

void NavigationWidget::setSelectedBgColor(const QColor& color) {
    Q_D(NavigationWidget);
    d->selected_bg_color_ = color;
    update();
}

void NavigationWidget::setTextColor(const QColor& color) {
    Q_D(NavigationWidget);
    d->text_color_ = color;
    update();
}

void NavigationWidget::setFontSize(int value) {
    Q_D(NavigationWidget);
    d->font_size_ = value;
    update();
}

void NavigationWidget::setClickable(bool b) {
    Q_D(NavigationWidget);
    d->is_clickable_ = b;
}

void NavigationWidget::enterEvent(QEvent*) {
    Q_D(NavigationWidget);
    d->is_enter_ = true;
    this->update();
}

void NavigationWidget::leaveEvent(QEvent*) {
    Q_D(NavigationWidget);
    d->is_enter_ = false;
    this->update();
}

void NavigationWidget::mousePressEvent(QMouseEvent*) {
    Q_D(NavigationWidget);
    d->is_pressed_ = true;
    this->update();
}

void NavigationWidget::mouseReleaseEvent(QMouseEvent*) {
    Q_D(NavigationWidget);
    d->is_pressed_ = false;
    this->update();
    if (d->is_clickable_) {
        emit clicked(true);
    }
}

NavigationSeparator::NavigationSeparator(QWidget* parent) : NavigationWidget(parent) {
    setCompacted(true);
    setSelectable(false);
    setFixedHeight(3);
}

void NavigationSeparator::setCompacted(bool isCompacted) {
    if (this->isCompacted() == isCompacted) {
        return;
    } else {
        this->setFixedSize(getExpandWidth() + 10, 3);
    }
    this->update();
}

void NavigationSeparator::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    QPen pen(getBgColor());
    pen.setCosmetic(true);
    painter.setPen(pen);
    painter.drawLine(0, 1, this->width(), 1);
}

NavigationPushButton::NavigationPushButton(QWidget* parent)
    : NavigationPushButton(*new NavigationPushButtonPrivate, parent) {}

NavigationPushButton::NavigationPushButton(const QString& text,
                                           const QString& icon,
                                           bool isSelectable,
                                           QWidget* parent)
    : NavigationPushButton(parent) {
    Q_D(NavigationPushButton);
    d->text_ = text;
    d->icon_ = icon;
    setSelectable(isSelectable);
}

void NavigationPushButton::setText(const QString& text) {
    Q_D(NavigationPushButton);
    d->text_ = text;
    update();
}

void NavigationPushButton::setIcon(const QString& filePath) {
    Q_D(NavigationPushButton);
    if (d->icon_ == filePath)
        return;
    d->icon_ = filePath;
    update();
}

void NavigationPushButton::setIconSize(const QSizeF& size) {
    Q_D(NavigationPushButton);
    if (size == d->icon_size_)
        return;
    d->icon_size_ = size;
    update();
}

QSizeF NavigationPushButton::getIconSize() const {
    Q_D(const NavigationPushButton);
    return d->icon_size_;
}

QString NavigationPushButton::getText() const {
    Q_D(const NavigationPushButton);
    return d->text_;
}

QString NavigationPushButton::getIcon() const {
    Q_D(const NavigationPushButton);
    return d->icon_;
}

void NavigationPushButton::setIndicatorVisibale(bool v) {
    Q_D(NavigationPushButton);
    if (d->indicator_visibale_ == v) {
        return;
    }
    d->indicator_visibale_ = v;
    update();
}

bool NavigationPushButton::getIndicatorVisibale() const {
    Q_D(const NavigationPushButton);
    return d->indicator_visibale_;
}

void NavigationPushButton::setTextAlign(Qt::Alignment flag) {
    Q_D(NavigationPushButton);
    d->alignment_ = flag;
    update();
}

void NavigationPushButton::paintEvent(QPaintEvent*) {
    Q_D(NavigationPushButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);

    auto bgColor = d->bg_color_;
    if (d->is_enter_)
        bgColor = d->hover_bg_color_;
    if (d->is_selected_) {
        bgColor = d->selected_bg_color_;
    }

    painter.setBrush(bgColor);
    painter.drawRoundedRect(rect(), 6, 6);

    // draw indicator
    if (d->indicator_visibale_) {
        paintIndicator(painter);
    }

    // draw icon
    paintIcon(painter);

    // draw text
    QFont font = painter.font();
    font.setPixelSize(getFontSize());
    painter.setFont(font);
    paintText(painter);
}

void NavigationPushButton::paintIcon(QPainter& painter) {
    Q_D(NavigationPushButton);
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);
    int left = d->indicator_visibale_ ? 4 : 0;
    auto icon_size = d->icon_size_;
    auto x = icon_size.isValid() ? (d->compact_width_ - left - icon_size.width()) / 2 : left;
    auto y = icon_size.isValid() ? (height() - icon_size.height()) / 2 : 0;
    drawIcon(d->icon_, &painter, QRectF(x, y, icon_size.width(), icon_size.height()));
    painter.restore();
}

void NavigationPushButton::paintIndicator(QPainter& painter) {
    painter.setBrush(QColor("skyblue"));
    painter.drawRoundedRect(0, 10, 3, height() - 20, 1.5, 1.5);
}

void NavigationPushButton::paintText(QPainter& painter) {
    Q_D(NavigationPushButton);
    painter.setPen(getTextColor());
    painter.drawText(QRect(d->compact_width_ + 4, 0, width() - 57, height()),
                     Qt::AlignVCenter | Qt::AlignLeft, d->text_);
}

NavigationPushButton::NavigationPushButton(NavigationPushButtonPrivate& dd, QWidget* parent)
    : NavigationWidget(dd, parent) {
    setSelectable(true);
}

void RotateButton::timerEvent(QTimerEvent* e) {
    if (e->timerId() == timer_id_) {
        if (is_enter_) {
            if (rotate_ < 180)
                rotate_ += 10;
        } else {
            if (rotate_ > 0)
                rotate_ -= 10;
        }
        if (rotate_ == 0 || rotate_ == 180) {
            killTimer(timer_id_);
            timer_id_ = 0;
        }
        update();
    }
}

void RotateButton::paintIcon(QPainter& painter) {
    painter.save();
    auto icon_size = getIconSize();
    int left = getIndicatorVisibale() ? 4 : 0;
    auto x = (getCompactWidth() - left - icon_size.width()) / 2;
    auto y = (height() - icon_size.height()) / 2;
    painter.translate(x + 8, y + 8);
    painter.rotate(rotate_);
    drawIcon(getIcon(), &painter, QRectF(-8., -8., icon_size.width(), icon_size.height()));
    painter.restore();
}

void RotateButton::enterEvent(QEvent* event) {
    NavigationPushButton::enterEvent(event);
    is_enter_ = true;
    if (timer_id_ == 0)
        timer_id_ = startTimer(10);
}

void RotateButton::leaveEvent(QEvent* event) {
    NavigationPushButton::leaveEvent(event);
    is_enter_ = false;
    if (timer_id_ == 0)
        timer_id_ = startTimer(10);
}

NavigationToolButton::NavigationToolButton(QWidget* parent)
    : NavigationPushButton(*new NavigationToolButtonPrivate(this), parent) {
    Q_D(NavigationToolButton);
    d->init();
    setCursor(Qt::PointingHandCursor);
}

NavigationToolButton::NavigationToolButton(const QString& text,
                                           const QString& icon,
                                           QWidget* parent)
    : NavigationToolButton(parent) {
    Q_D(NavigationToolButton);
    d->text_ = text;
    d->icon_ = icon;
}

Qt::ToolButtonStyle NavigationToolButton::toolButtonStyle() const {
    Q_D(const NavigationToolButton);
    return d->toolButtonStyle;
}

qreal NavigationToolButton::getTopPadding() const {
    Q_D(const NavigationToolButton);
    return d->top_padding_;
}

void NavigationToolButton::setToolButtonStyle(Qt::ToolButtonStyle style) {
    Q_D(NavigationToolButton);
    if (style == d->toolButtonStyle)
        return;
    d->toolButtonStyle = style;
    if (isVisible()) {
        update();
    }
}

void NavigationToolButton::setTopPadding(qreal v) {
    Q_D(NavigationToolButton);
    d->top_padding_ = v;
    update();
}

void NavigationToolButton::paintEvent(QPaintEvent* e) {
    NavigationPushButton::paintEvent(e);
}

void NavigationToolButton::paintIcon(QPainter& painter) {
    Q_D(NavigationToolButton);
    Qt::ToolButtonStyle toolButtonStyle = d->toolButtonStyle;
    if (toolButtonStyle == Qt::ToolButtonFollowStyle) {
        toolButtonStyle = Qt::ToolButtonStyle(style()->styleHint(QStyle::SH_ToolButtonStyle));
    }
    auto icon_size = d->icon_size_;
    int left = d->indicator_visibale_ ? 4 : 0;
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);
    if (toolButtonStyle == Qt::ToolButtonTextBesideIcon) {
        NavigationPushButton::paintIcon(painter);
    } else if (toolButtonStyle == Qt::ToolButtonIconOnly) {
        auto x = icon_size.isValid() ? (width() - left - icon_size.width()) / 2 : 0;
        auto y = icon_size.isValid() ? (height() - icon_size.height()) / 2 : 0;
        drawIcon(d->icon_, &painter,
                 QRectF(x, y + d->top_padding_, icon_size.width(), icon_size.height()));
    } else if (toolButtonStyle == Qt::ToolButtonTextUnderIcon) {
        auto x = icon_size.isValid() ? (width() - left - icon_size.width()) / 2 : 0;
        auto y = 4;
        drawIcon(d->icon_, &painter,
                 QRectF(x, y + d->top_padding_, icon_size.width(), icon_size.height()));
    }
    painter.restore();
}

void NavigationToolButton::paintText(QPainter& painter) {
    Q_D(NavigationToolButton);
    Qt::ToolButtonStyle toolButtonStyle = d->toolButtonStyle;
    if (toolButtonStyle == Qt::ToolButtonFollowStyle) {
        toolButtonStyle = Qt::ToolButtonStyle(style()->styleHint(QStyle::SH_ToolButtonStyle));
    }

    if (toolButtonStyle == Qt::ToolButtonIconOnly)
        return;
    if (toolButtonStyle == Qt::ToolButtonTextBesideIcon) {
        if (d->alignment_ & Qt::AlignLeft) {
            painter.setPen(getTextColor());
            int left = d->indicator_visibale_ ? 4 : 0;
            auto icon_size = getIconSize();
            auto x =
                icon_size.isValid() ? (d->compact_width_ - left - icon_size.width()) / 2 : left;
            painter.drawText(QRect(getIconSize().width() + x + 6, 0, width() - 57, height()),
                             Qt::AlignVCenter | Qt::AlignLeft, d->text_);
        } else {
            NavigationPushButton::paintText(painter);
        }
    } else if (toolButtonStyle == Qt::ToolButtonTextOnly) {
        painter.setPen(getTextColor());
        painter.drawText(QRect(4, 0, width() - 4, height()), Qt::AlignCenter, d->text_);
    } else if (toolButtonStyle == Qt::ToolButtonTextUnderIcon) {
        painter.setPen(getTextColor());
        painter.drawText(QRect(4, 0, width() - 4, height() - 4), Qt::AlignHCenter | Qt::AlignBottom,
                         d->text_);
    }
}
