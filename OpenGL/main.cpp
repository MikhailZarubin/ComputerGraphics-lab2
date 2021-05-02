#include "View.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    View w;
    QSurfaceFormat format;
    QSurfaceFormat::setDefaultFormat(format);
    w.setFormat(format);
    w.LoadData("FOURDIX-1.bin");
    w.show();
    return a.exec();
}