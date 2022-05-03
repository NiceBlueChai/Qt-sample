#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QStyledItemDelegate>
#include <QListView>
#include <QAbstractListModel>

class ListItemDelegate : public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};


class ListModel : public QAbstractListModel {
    Q_OBJECT
public:
    using QAbstractListModel::QAbstractListModel;

    enum Roles {
        PosIndexRole = Qt::UserRole,
        OpenBtnRectRole,
        OpenBtnToolTipRole
    };

    int rowCount(const QModelIndex& index) const override { return 100;}
    QVariant data(const QModelIndex& index, int role) const override;

    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

private:
    QModelIndex pos_index;
    QRect m_rect;
};

class ListView : public QListView
{
public:
    explicit ListView(QWidget* parent = nullptr);

    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // LISTVIEW_H
