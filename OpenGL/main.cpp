#include "View.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    View w;
    ViewNewLayer wn;
    QSurfaceFormat format;
    QSurfaceFormat::setDefaultFormat(format);
    w.setFormat(format);
    wn.setFormat(format);
    w.LoadData("FOURDIX-1.bin");
    wn.LoadData("FOURDIX-1.bin");
    w.show();
    wn.show();
    return a.exec();
}