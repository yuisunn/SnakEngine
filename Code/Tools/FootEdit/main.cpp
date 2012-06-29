#include "footedit.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//a.setStyleSheet("QDockWidget,QTabWidget{ background-color: yellow;}");
	//a.setStyleSheet("QPushButton:hover{border-image: url(:/images}");
	FootEdit w;
	w.show();
	return a.exec();
}
