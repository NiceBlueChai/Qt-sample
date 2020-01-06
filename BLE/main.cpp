#include "ble.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
#if QT_VERSION > QT_VERSION_CHECK(5,1,3)
#if _MSC_VER>1900
	BLE w;
	w.show();
	return a.exec();
#endif
#endif
}
