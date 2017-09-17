
#include <QCoreApplication>
#include "MyServer.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    new MyServer;

    return app.exec();
}
