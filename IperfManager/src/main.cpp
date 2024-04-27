#include <QCoreApplication>
#include "iperfserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    IperfServer server;
    QStringList list;
    list << "-s";
    list << "-p 20000";
    list << "-u";
    list << "-i 1";
    list << "-t 1005";
    server.setParams("iperf", list);
    server.start();

    return a.exec();
}

