#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <QObject>
#include <QStringList>
#include <QProcess>
#include <QDebug>

class NetworkInterface : public QObject
{
    Q_OBJECT
public:
    explicit NetworkInterface(QObject *parent = nullptr);
    static QStringList getNetworkInterfacesList();

signals:

};

#endif // NETWORKINTERFACE_H
