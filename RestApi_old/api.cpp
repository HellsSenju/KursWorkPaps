#include "api.h"

Api::Api(QObject *parent)
    : QObject{parent}
{
    manager = new QNetworkAccessManager(this);
}
