#ifndef IPERFMANAGER_H
#define IPERFMANAGER_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QUuid>

#include "model/abstractiperf.h"
#include "model/iperfserver.h"
#include "model/iperfclient.h"

class IperfManager : public QObject
{
    Q_OBJECT

public:
    IperfManager(QObject* parent=0);
    ~IperfManager();

private:
    QList<AbstractIperf*> iperfsPool;

public slots:
    void onStart(bool server, const QString &uuid, const QString &command);

};

#endif // IPERFMANAGER_H
