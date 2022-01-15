#include "mainwindow.h"

#include <QApplication>
#include <QtPlugin>
#include "gui-utils.h"

Q_IMPORT_PLUGIN(BasicToolsPlugin)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    nbc::utils::moveToScreenCenter(w);
    return a.exec();
}
