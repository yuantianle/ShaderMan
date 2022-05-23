#include "ShaderMan.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ShaderMan w;
    w.show();
    return a.exec();
}
