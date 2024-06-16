#include "iperfserver.h"

IperfServer::IperfServer(QUuid processUuid)
{
    process = new QProcess();
    uuid = processUuid;
    server = true;

    connect(process, &QProcess::started, this, &IperfServer::onStarted);
    connect(process, &QProcess::readyReadStandardOutput, this, &IperfServer::onStandartOutput);
    connect(process, &QProcess::readyReadStandardError, this, &IperfServer::onStandartError);
    connect(process, &QProcess::errorOccurred, this, &IperfServer::onErrorOccurred);

    connect(process, &QProcess::stateChanged, this, [=](QProcess::ProcessState newState)
    {
        switch (newState) {
        case 0: //not running
//            qDebug("IperfServer : %s : Процесс not running", qPrintable(uuid.toString()));
            break;
        case 1: //starting
            qDebug("IperfServer : Процесс запускается, но программа еще не была вызвана %s", qPrintable(uuid.toString()));
            setState(ProcessState::Starting);
            break;
        case 2: // running
//            qDebug("stateChanged : %s : Процесс запущен и готов к чтению и записи", qPrintable(uuid.toString()));

            break;
  }
    });

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](int exitCode, QProcess::ExitStatus exitStatus)
    {
        qDebug() << getUuid();
        qDebug() << getUuidChar();
        qDebug("IperfServer : Процес завершился %c. exitCode: %i. exitStatus: %i.",
               getUuidChar(),
               exitCode,
               exitStatus
        );

        setState(ProcessState::Finished);
    });
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
//    process->waitForReadyRead(-1);
}

void IperfServer::stop()
{
    process->terminate();
}

void IperfServer::waitForFinished(int sec)
{
    process->waitForFinished(sec);
}

void IperfServer::onStandartOutput()
{
    qDebug() << "IperfServer : standartOutput: " <<  process->readAll();

//    qDebug("IperfServer : standartOutput (%s): %s", qPrintable(processUuid.toString()), qPrintable(process->readAll()));
}

void IperfServer::onErrorOccurred(QProcess::ProcessError error)
{
    switch (error) {
    case 0:
        qDebug("IperfServer : errorOccurred : %s : FailedToStart", qPrintable(uuid.toString()));
        break;

    case 1:
        qDebug("IperfServer : errorOccurred : %s : Crashed", qPrintable(uuid.toString()));
        break;

    case 2:
        qDebug("IperfServer : errorOccurred : %s : Timedout", qPrintable(uuid.toString()));
        break;

    case 3:
        qDebug("IperfServer : errorOccurred : %s : ReadError", qPrintable(uuid.toString()));
        break;

    case 4:
        qDebug("IperfServer : errorOccurred : %s : WriteError", qPrintable(uuid.toString()));
        break;

    case 5:
        qDebug("IperfServer : errorOccurred : %s : UnknownError", qPrintable(uuid.toString()));
        break;

    default:
        qDebug("IperfServer : errorOccurred : %s : default", qPrintable(uuid.toString()));
        break;
    }
}

void IperfServer::onStandartError()
{
    qDebug("IperfServer : standartError (%s): %s", getUuidChar(), qPrintable(process->errorString()));
}

void IperfServer::onStarted()
{
    qDebug("IperfServer : Процесс запущен и готов к чтению и записи %s", qPrintable(uuid.toString()));
    setState(ProcessState::Running);
}
