#include <QApplication>
#include "widget.h"
#include "gui-utils.h"

int main(int argc, char** argv)
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    nbc::utils::moveToScreenCenter(w);
    return a.exec();
}
