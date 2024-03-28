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
    static QStringList getExistingRules(QString interface);

signals:

};

#endif // NETWORKINTERFACE_H
