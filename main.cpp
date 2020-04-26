#include "json_tool.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	json_tool w;
	w.show();

	return a.exec();
}
