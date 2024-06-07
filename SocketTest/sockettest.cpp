#include "sockettest.h"

SocketTest::SocketTest(QObject *parent)
    : QObject{parent}
{

}

void SocketTest::connect()
{
    socket = new QTcpSocket(this);


}
