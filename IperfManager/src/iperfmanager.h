#ifndef IPERFMANAGER_H
#define IPERFMANAGER_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QMap>
#include <QUuid>
#include <QStringList>

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
    QMap<QString, AbstractIperf*> pool;


public slots:
    void startNewProcess(bool server, const QString &uuid, const QString &command);
    void stopProcess(const QString &uuid);

    void onProcessStateChaned(const QString &uuid, ProcessState state);

};

#endif // IPERFMANAGER_H
