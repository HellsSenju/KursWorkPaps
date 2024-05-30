#include "iperfserver.h"

IperfServer::IperfServer(QUuid processUuid)
{
    process = new QProcess();
    this->processUuid = processUuid;

    connect(process, &QProcess::started, this, &IperfServer::started);
    connect(process, &QProcess::readyReadStandardOutput, this, &IperfServer::output);
    connect(process, &QProcess::readyReadStandardError, this, &IperfServer::error);

//    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
//        [=](int exitCode, QProcess::ExitStatus exitStatus){

//    });

}

IperfServer::~IperfServer()
{
    process->deleteLater();
}

void IperfServer::setParams(const QString &program, const QStringList &args)
{
    process->setProgram(program);
    process->setArguments(args);
}

void IperfServer::start()
{
    process->start();
    process->waitForReadyRead();
}

void IperfServer::stop()
{

}

void IperfServer::waitForFinished(int sec)
{
    process->waitForFinished(sec);
}

void IperfServer::output()
{
    qDebug() << process->readAll();
}

void IperfServer::error()
{
    qDebug() << process->readAll();
}

bool IperfServer::started()
{
    qDebug() << "Процесс запустился";
    return true;
}
