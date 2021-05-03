#include <QApplication>

#include "Dialog_window.h"

#pragma comment (lib, "opengl32.lib")

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    View* v = new View;
    QSurfaceFormat format;
    QSurfaceFormat::setDefaultFormat(format);
    v->setFormat(format);
    v->LoadData("FOURDIX-1.bin");
    Window window(v);
    window.show();
    return a.exec();
}