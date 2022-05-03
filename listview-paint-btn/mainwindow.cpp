#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "listview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->listView->setModel(new ListModel(this));
    ui->listView->setItemDelegate(new ListItemDelegate(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

