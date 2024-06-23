#include "networklib.h"

NetworkLib::NetworkLib():
    network()
{
}

NetworkLib::~NetworkLib()
{
    network->deleteLater();
}
