#include "networkinterface.h"

NetworkInterface::NetworkInterface(QObject *parent)
    : QObject{parent}
{

}

QStringList NetworkInterface::getNetworkInterfacesList()
{
    QStringList resList;

    QProcess *process = new QProcess();
    process->start("ls", QStringList() << "/sys/class/net" );

    if(!process->waitForStarted() || !process->waitForFinished() ) {
        qDebug() << "Процесс не запустился";
        return resList;
    }

//    qDebug() << process->readAllStandardError();
    QString res = process->readAllStandardOutput();
    resList = res.split("\n", Qt::SkipEmptyParts);

    return resList;
}

QStringList NetworkInterface::getExistingRules(QString interface)
{
    QStringList resList;

    QProcess *process = new QProcess();
    process->start("/usr/sbin/tc", QStringList() << "-p" << "qdisc" << "ls" << "dev" << interface);

    if(!process->waitForStarted() || !process->waitForFinished() ) {
        qDebug() << "Процесс не запустился";
        return resList;
    }

    QString error = process->readAllStandardError();
    if(!error.isEmpty()){
        qDebug() << "Возникли ошибки. " << error;
        return resList;
    }

    QString res = process->readAllStandardOutput();
    resList = res.split("\n", Qt::SkipEmptyParts);

    return resList;
}
