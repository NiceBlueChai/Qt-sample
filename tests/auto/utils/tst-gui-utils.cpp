#include <QTest>
#include <QWidget>
#include "gui-utils.h"

class tst_GuiUtils : public QObject {
    Q_OBJECT

private slots:
    void testMoveToScreenCenter(){
        QWidget widget;
        widget.setWindowFlag(Qt::FramelessWindowHint);
        widget.show();
        auto pos1 = widget.pos();

        QWidget widget2;
        widget2.setWindowTitle("w2");
        nbc::utils::moveToScreenCenter(widget2);
        widget2.show();
        QCOMPARE(pos1, widget.pos());
    }
};


QTEST_MAIN(tst_GuiUtils)
#include "tst-gui-utils.moc"
