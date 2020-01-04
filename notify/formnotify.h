#ifndef FORMNOTIFY_H
#define FORMNOTIFY_H

#include <QWidget>
#include "notifymanager.h"
namespace Ui {
class FormNotify;
}

class FormNotify : public QWidget
{
    Q_OBJECT

public:
    explicit FormNotify(QWidget *parent = nullptr);
    ~FormNotify();

private slots:
    void on_pushButton_clicked();

private:
    Ui::FormNotify *ui;
    NotifyManager man;
};

#endif // FORMNOTIFY_H
