#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <private/qdatetime_p.h>
#include <private/qdatetimeedit_p.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QCalendarPopup pp;
}

MainWindow::~MainWindow()
{
    delete ui;
}

