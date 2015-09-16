#include "FMainWidgt.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FMainWidgt w;
    w.show();

    return a.exec();
}
