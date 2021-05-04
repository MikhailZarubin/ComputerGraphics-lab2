#include <QApplication>
#include "Dialog_window.h"
#include <string>

#pragma comment (lib, "opengl32.lib")

int main(int argc, char* argv[])
{
    std::string s;
    for (int i = 0; i < argc; i++)
    {
        if (!strcmp(argv[i], "-p") && (i + 1 < argc))
        {
            s = argv[i + 1];
        }
    }

    QApplication a(argc, argv);
    Window window(Q_NULLPTR, s.c_str());
    window.show();
    return a.exec();
}