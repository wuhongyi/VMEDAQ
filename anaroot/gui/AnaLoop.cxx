#include <QApplication>
#include "TAlMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TAlMainWindow mainWin;
    mainWin.show();
    return app.exec();
}
