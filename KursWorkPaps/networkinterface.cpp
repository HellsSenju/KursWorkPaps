#include "networkinterface.h"

NetworkInterface::NetworkInterface(QObject *parent)
    : QObject{parent}
{

}

QStringList NetworkInterface::getNetworkInterfacesList()
{
    QStringList resList;
    QString program = "ls";
    QStringList arguments;
    arguments << "/sys/class/net";

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
