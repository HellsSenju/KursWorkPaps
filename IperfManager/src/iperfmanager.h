#ifndef IPERFMANAGER_H
#define IPERFMANAGER_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QMap>
#include <QUuid>
#include <QStringList>

#include "models/abstractiperf.h"
#include "models/iperfserver.h"
#include "models/iperfclient.h"

class IperfManager : public QObject
{
    Q_OBJECT

public:
    explicit IperfManager(QObject* parent=0);
    ~IperfManager();

    ProcessState getProcessStatus(const QString &uuid){
        return pool.value(uuid)->getState();
    };

private:
    QMap<QString, AbstractIperf*> pool;


public slots:
    bool startNewProcess(bool server, const QString &uuid, const QString &command);
    bool stopProcess(const QString &uuid);
    void deleteProcess(const QString &uuid);

    void onStateChanged(ProcessState state);

signals:
    void iperfChanged();

};

#endif // IPERFMANAGER_H
