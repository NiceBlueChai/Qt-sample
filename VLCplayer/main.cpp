#include "vlcplayer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	VLCplayer w;
	w.show();
	return a.exec();
}
