#ifndef NETWORKLIB_H
#define NETWORKLIB_H

#include "NetworkLib_global.h"
#include "network.h"

#include <QJsonObject>


class NETWORKLIB_EXPORT NetworkLib
{
public:
    NetworkLib();

    ~NetworkLib();

    Network* get(){
        return network;
    };

private:
    Network* network;

};

#endif // NETWORKLIB_H
