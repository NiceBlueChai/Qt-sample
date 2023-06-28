#include <QApplication>
#include <QDebug>
#include <QWidget>

int main(int argc, char *argv[])
{
    //  QApplication::setHighDpiScaleFactorRoundingPolicy(
    //  Qt::HighDpiScaleFactorRoundingPolicy.PassThrough)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication a(argc, argv);


    QWidget w;
    w.show();
    return a.exec();
}
