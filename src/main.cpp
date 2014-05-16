#include <QApplication>

#include "windows/main_window.h"
#include "windows/graphics_window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    GraphicsWindow gw;
    gw.show();

    return a.exec();
}
