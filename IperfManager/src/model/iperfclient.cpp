#include "iperfclient.h"

IperfClient::IperfClient(QUuid processUuid)
{
    process = new QProcess();
    this->processUuid = processUuid;


}

IperfClient::~IperfClient()
{
    process->deleteLater();
}

void IperfClient::setParams(const QString &program, const QStringList &args)
{
    process->setProgram(program);
    process->setArguments(args);
}

void IperfClient::start()
{
    process->start();
    process->waitForReadyRead();
}

void IperfClient::stop()
{

}

void IperfClient::waitForFinished(int sec)
{
    process->waitForFinished(sec);
}

void IperfClient::output()
{
    qDebug() << process->readAll();
}

void IperfClient::error()
{
    qDebug() << process->readAll();
}

bool IperfClient::started()
{
    qDebug() << "Процесс запустился";
    return true;
}
