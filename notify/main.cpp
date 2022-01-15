#pragma execution_character_set("utf-8")
#include "formnotify.h"
#include <QApplication>
#if (QT_VERSION <= QT_VERSION_CHECK(6, 0, 0))
#include <QTextCodec>
#endif
#include "gui-utils.h"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
#if (QT_VERSION <= QT_VERSION_CHECK(6, 0, 0))
#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
#if _MSC_VER
    QTextCodec *codec = QTextCodec::codecForName("gbk");
#else
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
#endif
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#else
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
#endif
#endif

    FormNotify w;
    w.show();
    nbc::utils::moveToScreenCenter(w);
    a.exec();
}
