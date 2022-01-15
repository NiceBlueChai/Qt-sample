#pragma execution_character_set("utf-8")
#include "formnotify.h"
#include "ui_formnotify.h"

FormNotify::FormNotify(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormNotify)
{
    ui->setupUi(this);
    ui->spinBox->setValue(10000);
    ui->spinBox->setSuffix("ms");
    man = new NotifyManager(this);
    man->setMaxCount(5);
}

FormNotify::~FormNotify()
{
    delete ui;
}

void FormNotify::on_pushButton_clicked()
{
    int showTime = ui->spinBox->value();
    man->notify(this, "你好", "新年快乐...", showTime);
}

void FormNotify::on_pushButton_2_clicked()
{
    int showTime = ui->spinBox->value();
    man->notify(this, "你好", "新年快乐...", ":/images/INFO.svg", showTime);
}

