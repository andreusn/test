#include "servergui.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QApplication::setStyle("Fusion");

    ServerGUI w;
    w.show();

    return a.exec();
}