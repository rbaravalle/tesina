#include <QApplication>

#include "escena.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Escena escena;
    escena.show();
    return app.exec();
}

