#include "sekiro.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Sekiro w;
    w.show();
    return a.exec();
}
