#include "navigation-panel.h"
#include "navigation-widget.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QMap>
#include <QVariant>

class NavigationPanelPrivate {
    Q_DECLARE_PUBLIC(NavigationPanel)
public:
    NavigationPanelPrivate(NavigationPanel* q)
        :q_ptr(q),
        display_mode_(NavigationPanel::NavigationDisplayMode::COMPACT),
        history_(items_){
    }
    void init();
    void initLayout();
    void resetItemsCompactWidth();
    void resetItemsExpandWidth();
    void setItemsCompacted();
    void setSpacing(int spacing);
    void setContentsMargins(const QMargins& margins);
    int getItemCompactWidth() {
        return compact_width_ - margins_.left() - margins_.right();
    }
    NavigationPanel* q_ptr = nullptr;
    NavigationToolButton* menu_button_ = nullptr;
    QVBoxLayout* top_layout_ = nullptr;
    QVBoxLayout* scroll_layout_ = nullptr;
    QVBoxLayout* bottom_layout_ = nullptr;
    QVBoxLayout* vlayout_ = nullptr;
    QScrollArea* scroll_area_ = nullptr;
    QWidget* scroll_widget_ = nullptr;
    NavigationPanel::NavigationDisplayMode display_mode_;
    int item_height_ = 40;
    int expand_width_ = 312;
    int compact_width_ = 70;
    int inside_spacing_ = 18;
    QMap<QString, NavigationWidget*>  items_;
    QMargins margins_;
    QString current_routeKey_;
    NavigationWidget* current_item_ = nullptr;
    NavigationHistory history_;
    QMap<QString, NavigationPanel::ClickFuncType> on_clicks_;
};
void NavigationPanelPrivate::init(){
    margins_ = QMargins(6, 18, 6, 18);

    initLayout();
}
void NavigationPanelPrivate::setSpacing(int spacing) {
    if (spacing == inside_spacing_)
        return;
    inside_spacing_ = spacing;
    vlayout_->setSpacing(inside_spacing_);
    top_layout_->setSpacing(inside_spacing_);
    scroll_layout_->setSpacing(inside_spacing_);
    bottom_layout_->setSpacing(inside_spacing_);
}

void NavigationPanelPrivate::setContentsMargins(const QMargins& margins) {
    if (margins == margins_)
        return;
    margins_ = margins;
    vlayout_->setContentsMargins(margins);
}
void NavigationPanelPrivate::initLayout()
{
    Q_Q(NavigationPanel);
    vlayout_ = new QVBoxLayout(q);
    top_layout_ = new QVBoxLayout;
    scroll_layout_ = new QVBoxLayout;
    bottom_layout_ = new QVBoxLayout;
    scroll_area_ = new QScrollArea(q);
    scroll_area_->setFrameStyle(QFrame::NoFrame);
    scroll_widget_ = new QWidget;
    scroll_area_->setWidget(scroll_widget_);
    scroll_widget_->setObjectName("scrollWidget");
    scroll_widget_->setLayout(scroll_layout_);
    vlayout_->addLayout(top_layout_);
    vlayout_->addWidget(scroll_area_, 1);
    vlayout_->addLayout(bottom_layout_);
    scroll_area_->setWidgetResizable(true);
    scroll_area_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll_area_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    vlayout_->setSpacing(inside_spacing_);
    vlayout_->setContentsMargins(margins_);
    top_layout_->setSpacing(inside_spacing_);
    top_layout_->setMargin(0);
    scroll_layout_->setSpacing(inside_spacing_);
    scroll_layout_->setMargin(0);
    bottom_layout_->setSpacing(inside_spacing_);
    bottom_layout_->setMargin(0);
    scroll_layout_->setAlignment(Qt::AlignTop);
    top_layout_->setAlignment(Qt::AlignTop);
    bottom_layout_->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);

    menu_button_ = new NavigationToolButton("fsdf", ":/nbcui/images/icons/Menu_black.svg");
    menu_button_->setFixedSize(getItemCompactWidth(), item_height_);
    menu_button_->setToolButtonStyle(Qt::ToolButtonIconOnly);
    top_layout_->addWidget(menu_button_, 0, Qt::AlignTop|Qt::AlignLeft);
    QObject::connect(menu_button_,  &NavigationToolButton::clicked, q, &NavigationPanel::toggle);
}

void NavigationPanelPrivate::resetItemsCompactWidth()
{
    for (auto &item : items_)  {
        item->setCompactWidth(compact_width_ - margins_.left() - margins_.right());
    }
    menu_button_->setFixedSize(getItemCompactWidth(), item_height_);
}

void NavigationPanelPrivate::resetItemsExpandWidth()
{
    for (auto &item : items_)  {
        item->setExpandWidth(expand_width_ - margins_.left() - margins_.right());
    }
}

void NavigationPanelPrivate::setItemsCompacted()
{
    for (auto &item : items_)  {
        item->setCompacted(display_mode_ == NavigationPanel::COMPACT);
    }
}

NavigationPanel::NavigationPanel(QWidget *parent):
    QFrame(parent),d_ptr(new NavigationPanelPrivate(this))
{
    Q_D(NavigationPanel);
    d->init();
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    if (d_ptr->display_mode_ == COMPACT) {
        setFixedWidth(d_ptr->compact_width_);
    } else {
        setFixedWidth(d_ptr->expand_width_);
    }
    setFrameStyle(NoFrame);
}

NavigationPanel::~NavigationPanel()
{
    delete d_ptr;
}

void NavigationPanel::addSeparator(NavigationPanel::NavigationItemPosition position)
{
    auto separator = new NavigationSeparator(this);
    this->addWidgetToLayout(separator, position);
}

void NavigationPanel::addSpacing(
    int spacing,
    NavigationItemPosition position) {
    Q_D(NavigationPanel);
    switch (position) {
    case NavigationItemPosition::TOP: {
        d->top_layout_->addSpacing(spacing);
        break;
    }
    case NavigationItemPosition::SCROLL: {
        d->scroll_layout_->addSpacing(spacing);
        break;
    }
    case NavigationItemPosition::BOTTOM: {
        d->bottom_layout_->addSpacing(spacing);
        break;
    }
    }
}

void NavigationPanel::addWidget(const QString &routeKey, NavigationWidget *w, NavigationPanel::NavigationItemPosition position)
{
    Q_D(NavigationPanel);
    if (d->items_.keys().contains(routeKey))
        return;

    w->setProperty("routeKey", routeKey);
    d->items_[routeKey] = w;
    if (!qobject_cast<NavigationSeparator*>(w)
        && w->sizePolicy().horizontalPolicy() != QSizePolicy::Fixed) {
        auto var = w->property("dynamic_size");
        if (!var.isValid() && !var.toBool()) {
            w->setFixedHeight(d->item_height_);
        }
    }
    w->setExpandWidth(d->expand_width_ - d->margins_.left() - d->margins_.right());
    w->setCompactWidth(d->compact_width_ - d->margins_.left() - d->margins_.right());
    w->setCompacted(d->display_mode_ == COMPACT);
    addWidgetToLayout(w, position);
    connect(w, &NavigationWidget::clicked, this, [this, d](bool userTrigger){
        auto w = qobject_cast<NavigationWidget*>(sender());
        if (!w) {
            return;
        }
        QString routeKey = w->property("routeKey").toString();
        if (w->isSelectable()) {
            setCurrentItem(routeKey);
        }
        emit aboutToClicked(routeKey, userTrigger);
        if (d->on_clicks_.contains(routeKey)) {
            d->on_clicks_[routeKey](userTrigger);
        }
        emit clicked(routeKey, userTrigger);
    });
}

void NavigationPanel::expand()
{
    Q_D(NavigationPanel);
    d->display_mode_ = NavigationDisplayMode::EXPAND;
    setFixedWidth(d->expand_width_);
    d->setItemsCompacted();
    emit displayModeChanged(d->display_mode_);
    update();
}

void NavigationPanel::compact()
{
    Q_D(NavigationPanel);
    d->display_mode_ = NavigationDisplayMode::COMPACT;
    setFixedWidth(d->compact_width_);
    d->setItemsCompacted();
    emit displayModeChanged(d->display_mode_);
    update();
}

void NavigationPanel::toggle()
{
    Q_D(NavigationPanel);
    if (NavigationPanel::EXPAND == d->display_mode_) {
        this->compact();
    } else {
        this->expand();
    }
}

void NavigationPanel::setItemHeight(int h)
{
    Q_D(NavigationPanel);
    if (d->item_height_ == h)
        return;
    d->item_height_ = h;
    d->menu_button_->setFixedHeight(h);
    update();
}

void NavigationPanel::setCompactWidth(int w)
{
    Q_D(NavigationPanel);
    if (d->compact_width_ == w)
        return;
    d->compact_width_ = w;
    setMinimumWidth(d->compact_width_);
    d->resetItemsCompactWidth();
    update();
}

void NavigationPanel::setExpandWidth(int w)
{
    Q_D(NavigationPanel);
    if (d->expand_width_ == w)
        return;
    d->expand_width_ = w;
    setMaximumWidth(w);
    d->resetItemsExpandWidth();
    update();
}

int NavigationPanel::getSpacing() const
{
    Q_D(const NavigationPanel);
    return d->inside_spacing_;
}

QMargins NavigationPanel::getMargins() const
{
    Q_D(const NavigationPanel);
    return d->margins_;
}

QString NavigationPanel::getCurrentRouteKey() const
{
    Q_D(const NavigationPanel);
    return d->current_routeKey_;
}

NavigationWidget *NavigationPanel::getCurrentItem()
{
    Q_D(NavigationPanel);
    return d->current_item_;
}

void NavigationPanel::clearSelection()
{
    Q_D(NavigationPanel);
    for (auto iter = d->items_.begin(); iter != d->items_.end(); ++iter) {
        iter.value()->setSelected(false);
    }
    d->current_item_ = nullptr;
    d->current_routeKey_ = "";
}

void NavigationPanel::onClicked(const QString &routeKey, ClickFuncType func)
{
    Q_D(NavigationPanel);
    d->on_clicks_[routeKey] = func;
}

void NavigationPanel::forward()
{
    Q_D(NavigationPanel);
    d->history_.forward();
}

void NavigationPanel::back()
{
    Q_D(NavigationPanel);
    d->history_.back();
}

void NavigationPanel::setSpacing(int spacing)
{
    Q_D(NavigationPanel);
    d->setSpacing(spacing);
}

void NavigationPanel::setMargins(int left, int top, int right, int bottom)
{
    Q_D(NavigationPanel);
    d->setContentsMargins(QMargins(left, top, right, bottom));
}

void NavigationPanel::setCurrentItem(const QString &routeKey)
{
    Q_D(NavigationPanel);
    if (!d->items_.keys().contains(routeKey) || d->current_routeKey_ == routeKey) {
        return;
    }
    d->history_.push(routeKey);
    for (auto iter = d->items_.begin(); iter != d->items_.end(); ++iter) {
        bool b = iter.key() == routeKey;
        if (b) {
            d->current_item_ = iter.value();
        }
        iter.value()->setSelected(b);
    }
    d->current_routeKey_ = routeKey;
    emit currentItemChanged(routeKey);
}

void NavigationPanel::addWidgetToLayout(NavigationWidget *w, NavigationPanel::NavigationItemPosition position)
{
    Q_D(NavigationPanel);
    switch (position) {
    case NavigationItemPosition::TOP: {
        d->top_layout_->addWidget(w, 0, Qt::AlignTop);
        break;
    }
    case NavigationItemPosition::SCROLL: {
        d->scroll_layout_->addWidget(w, 0, Qt::AlignTop);
        break;
    }
    case NavigationItemPosition::BOTTOM: {
        d->bottom_layout_->addWidget(w, 0, Qt::AlignBottom );
        break;
    }
    }
    w->show();
}


NavigationHistory::NavigationHistory(QMap<QString, NavigationWidget *> &items, QObject *parent)
    :QObject(parent), items_(items), max_counts_(100)
{

}

void NavigationHistory::push(const QString &routeKey)
{
    if (back_history_.isEmpty()) {
        back_history_.push_back(routeKey);
        emit backEmptyChanged(false);
    }else if (back_history_.last() != routeKey) {
        back_history_.push_back(routeKey);
    }
    if (back_history_.size() > max_counts_) {
        back_history_.pop_front();
    }
    if (!forward_history_.isEmpty()) {
        forward_history_.clear();
        emit forwardEmptyChanged(true);
    }
}

void NavigationHistory::setMaxCounts(int v)
{
    max_counts_ = v;
}

int NavigationHistory::getMaxCounts() const
{
    return max_counts_;
}

bool NavigationHistory::isBackEmpty() const
{
    return back_history_.isEmpty();
}

bool NavigationHistory::isForwardEmpty() const
{
    return forward_history_.isEmpty();
}


void NavigationHistory::forward()
{
    if (forward_history_.isEmpty()) {
        return;
    }
    if (back_history_.isEmpty()) {
        back_history_.push_back(forward_history_.last());
        emit backEmptyChanged(false);
    } else {
        back_history_.push_back(forward_history_.last());
    }

    forward_history_.pop_back();
    navigate_forwark();
}

void NavigationHistory::back()
{
    if (back_history_.isEmpty()) {
        return;
    }
    if (forward_history_.isEmpty()) {
        forward_history_.push_back(back_history_.last());
        emit forwardEmptyChanged(false);
    } else {
        forward_history_.push_back(back_history_.last());
    }

    back_history_.pop_back();
    navigate_back();
}

void NavigationHistory::navigate_forwark()
{
    if (forward_history_.isEmpty()) {
        emit forwardEmptyChanged(true);
    } else {
        emit items_[forward_history_.last()]->clicked(false);
    }
}

void NavigationHistory::navigate_back()
{
    if (back_history_.isEmpty()) {
        emit backEmptyChanged(true);
    } else {
        emit items_[back_history_.last()]->clicked(false);
    }
}
