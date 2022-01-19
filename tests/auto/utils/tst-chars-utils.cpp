#ifdef _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif
#include <QTest>

#include "chars-utils.h"
using namespace nbc::utils;
class tst_CharsUtils : public QObject {
    Q_OBJECT

private slots:

    void tstFromUtf8() {
        QCOMPARE(1, fromUtf8(0x00, nullptr, nullptr));
        QCOMPARE(-1, fromUtf8(0xc0, nullptr, nullptr));
        QCOMPARE(-1, fromUtf8(0xF5, nullptr, nullptr));
        QCOMPARE(-1, fromUtf8(0xC1, nullptr, nullptr));
        QCOMPARE(-1, fromUtf8(0x80, nullptr, nullptr));
    }

    void tstTruncationUtf8() {
        QByteArray ba("Hello尼赛积分虫三扥看");
        int ret = truncationUtf8(reinterpret_cast<unsigned char*>(ba.data()), ba.length(), 10);
        QVERIFY(ret == 8);

        ret = truncationUtf8(reinterpret_cast<unsigned char*>(ba.data()), ba.length(), 5);
        QVERIFY(ret == 5);
    }
};


QTEST_MAIN(tst_CharsUtils)
#include "tst-chars-utils.moc"
