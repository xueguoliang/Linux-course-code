
#include <QApplication>
#include "MainWnd.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWnd w;
    w.show();

    return app.exec();
}
