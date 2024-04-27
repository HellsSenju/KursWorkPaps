#include "iperfserver.h"

IperfServer::IperfServer()
{
    process = new QProcess();
    connect(process, &QProcess::started, this, &IperfServer::start);
    connect(process, &QProcess::finished, this, &IperfServer::finished);
}

void IperfServer::setParams(const QString &program, const QStringList &args)
{
    process->setProgram(program);
    process->setArguments(args);
}

void IperfServer::start()
{
    process->start();
}

bool IperfServer::finished(int exitCode, QProcess::ExitStatus exitStatus)
{

}
