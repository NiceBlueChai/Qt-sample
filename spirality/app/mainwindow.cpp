#include "mainwindow.h"
#include "paintarea.h"
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QPluginLoader>
#include <QScrollArea>
#include "interface.h"
#include <QScrollBar>

#include <QActionGroup>
#include <QDebug>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),paintArea(new Paintarea),
    scrollArea(new QScrollArea)
{

    scrollArea->setBackgroundRole(QPalette::Light);
    scrollArea->setWidget(paintArea);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCentralWidget(scrollArea);

    createActions();
    createMenus();
    loadPlugins();

    setWindowTitle("app");
    this->resize(800,800);

    toCenter();
}

void MainWindow::changeBrush()
{
    auto action = qobject_cast<QAction*>(sender());
    if(!action)return;
    auto iBrush = qobject_cast<IBrush*>(action->parent());
    if(!iBrush)
        return;
    const QString brush = action->text();

    paintArea->setBrush(iBrush, brush);
}

void MainWindow::toCenter()
{

    scrollArea->verticalScrollBar()->setSliderPosition((2000-height())/2+20);
    scrollArea->horizontalScrollBar()->setValue((2000-width())/2);
}

void MainWindow::createActions()
{
    brushActionGroup = new QActionGroup(this);
    QToolBar *toolBar = this->addToolBar(tr("Tool Bar"));
    auto center = new QAction(tr("&Center"),this);
    connect(center, &QAction::triggered, this, &MainWindow::toCenter);
    toolBar->addAction(center);

}

void MainWindow::createMenus()
{
    brushMenu = menuBar()->addMenu(tr("&Brush"));
}

void MainWindow::loadPlugins()
{
    const auto staticInstance = QPluginLoader::staticInstances();
    for(QObject* plugin : staticInstance)
        populateMenus(plugin);
}

void MainWindow::populateMenus(QObject *plugin)
{
    auto iBrush = qobject_cast<IBrush*>(plugin);
    if(iBrush)
        addToMenu(plugin, iBrush->brushs(), brushMenu, &MainWindow::changeBrush,
                  brushActionGroup);
}

void MainWindow::addToMenu(QObject *plugin, const QStringList &texts,
                           QMenu *menu, MainWindow::Member member,
                           QActionGroup *actionGroup)
{
    for(const auto & text: texts){
        auto action = new QAction(text,plugin);
        connect(action, &QAction::triggered, this, member);
        menu->addAction(action);

        if(actionGroup)
        {
            action->setCheckable(true);
            actionGroup->addAction(action);
        }
    }
}

