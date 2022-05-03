#include "listview.h"
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QDebug>
#include <QToolTip>
#include <QImage>

static const int kItemHeight = 50;

void ListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    if (index.data(ListModel::PosIndexRole).toBool()) {
        auto opt_rect = option.rect;
        auto rect = QRect(opt_rect.x() + opt_rect.width() - opt_rect.height(),
                             opt_rect.y(), opt_rect.height(), opt_rect.height());

        auto image = QImage("D:\\gitee\\QtDemos\\listview-paint-btn\\folder-open.svg");
        painter->drawImage(rect, image);
        const_cast<QAbstractItemModel*>(index.model())->setData(index, rect, ListModel::OpenBtnRectRole);
    }
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(checkIndex(index, CheckIndexOption::IndexIsValid) && "index is invalid");

    switch(role) {
    case Qt::SizeHintRole:
        return QSize(0, kItemHeight);
    case Qt::DisplayRole:
    case Qt::ToolTipRole:
        return QString("line %1").arg(index.row());
    case PosIndexRole:
        return index == pos_index;
    case OpenBtnRectRole:
        return m_rect;
    case OpenBtnToolTipRole:
        return "open";
    }

    return QVariant();
}

bool ListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    switch(role) {
    case PosIndexRole:{
        auto old_index = pos_index;
        pos_index = index;
        if(!pos_index.isValid()) {
            m_rect = QRect();
        }
        emit dataChanged(old_index, old_index);
        emit dataChanged(pos_index, pos_index);
        break;
    }
    case OpenBtnRectRole:
        m_rect = value.toRect();
        break;
    }
    return true;
}

ListView::ListView(QWidget *parent)
{
    setMouseTracking(true);
    viewport()->installEventFilter(this);
}

bool ListView::event(QEvent *event)
{

    return QListView::event(event);
}

bool ListView::eventFilter(QObject *watched, QEvent *event)
{
    if (watched != viewport()) {
        return QListView::eventFilter(watched, event);
    }

    switch(event->type()) {
    case QEvent::Wheel:{
        auto wheel_ev = static_cast<QWheelEvent*>(event);
        QApplication::postEvent(viewport(),
                                new QMouseEvent(QEvent::MouseMove, wheel_ev->pos(),
                                                Qt::NoButton, Qt::NoButton, Qt::NoModifier));
        break;
    }
    case QEvent::HoverEnter:
    case QEvent::HoverMove:
    case QEvent::MouseMove:{
        auto mouse_ev = static_cast<QMouseEvent*>(event);
        auto index = indexAt(mouse_ev->pos());
        model()->setData(index, QVariant(), ListModel::PosIndexRole);
        if (!index.isValid()){
            break;
        }
        auto rect = model()->data(index, ListModel::OpenBtnRectRole).toRect();
        if (rect.contains(mouse_ev->pos())) {
            setCursor(Qt::PointingHandCursor);
        } else {
            unsetCursor();
        }
        if (QToolTip::isVisible()){
            QToolTip::hideText();
        }
        break;
    }
    case QEvent::Leave: {
        model()->setData(QModelIndex(), QVariant(), ListModel::PosIndexRole);
        break;
    }
    case QEvent::ToolTip: {
        auto mouse_ev = static_cast<QHelpEvent*>(event);
        auto index = indexAt(mouse_ev->pos());
        auto rect = model()->data(index, ListModel::OpenBtnRectRole).toRect();
        auto tip_text = model()->data(index, ListModel::OpenBtnToolTipRole).toString();
        if (rect.contains(mouse_ev->pos())) {
            QToolTip::showText(mouse_ev->globalPos(), tip_text, viewport());
            event->accept();
            return true;
        }
        break;
    }
    case QEvent::MouseButtonPress: {
        auto mouse_ev = static_cast<QMouseEvent*>(event);
        if (!(mouse_ev->buttons() & Qt::LeftButton)) {
            break;
        }
        auto index = indexAt(mouse_ev->pos());
        auto rect = model()->data(index, ListModel::OpenBtnRectRole).toRect();
        if (rect.contains(mouse_ev->pos())) {
            qDebug() << "open clicked";
        }
    }
    default: break;
    }

    return QListView::eventFilter(watched, event);
}
