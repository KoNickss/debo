#include "debo.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    debo w;
    w.show();
    return a.exec();
}
