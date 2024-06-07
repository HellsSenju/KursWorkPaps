#include <QCoreApplication>
#include "sockettest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SocketTest socket;
    socket.connect();
    return a.exec();
}
