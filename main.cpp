#include "gamepanel.h"
#include "cards.h"
#include "loading.h"
#include <QApplication>
#include <QResource>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<Cards>("Cards&");
 //   QResource::registerResource("./resource.rcc");

    Loading window;
    window.show();

    return a.exec();
}
