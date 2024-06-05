#include "iperfserver.h"

IperfServer::IperfServer(QUuid processUuid)
{
    process = new QProcess();
    uuid = processUuid;

//    process->setStandardOutputFile("../IperfManager/otherFiles/output.txt");

    connect(process, &QProcess::started, this, &IperfServer::onStarted);
    connect(process, &QProcess::readyReadStandardOutput, this, &IperfServer::onStandartOutput);
    connect(process, &QProcess::readyReadStandardError, this, &IperfServer::onStandartError);
    connect(process, &QProcess::errorOccurred, this, &IperfServer::onErrorOccurred);

    connect(process, &QProcess::stateChanged, this, [=](QProcess::ProcessState newState)
    {
        switch (newState) {
        case 0: //not running
            qDebug("IperfServer : stateChanged : %s : Процесс not running", qPrintable(uuid.toString()));
            break;
        case 1: //starting
            qDebug("IperfServer : stateChanged : %s : Процесс запускается, но программа еще не была вызвана", qPrintable(uuid.toString()));
            emit processStateChaned(uuid.toString(), ProcessState::Starting);
            break;
        case 2: // running
//            qDebug("stateChanged : %s : Процесс запущен и готов к чтению и записи", qPrintable(uuid.toString()));

            break;
  }
    });

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](int exitCode, QProcess::ExitStatus exitStatus)
    {
        qDebug("IperfServer : Процесс сервера(%s) завершился. exitCode: %i. exitStatus: %i.",
               qPrintable(this->uuid.toString()),
               exitCode,
               exitStatus
        );

        emit processStateChaned(uuid.toString(), ProcessState::Finished);
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
    process->waitForReadyRead();
}

void IperfServer::stop()
{
//    process->kill();
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
        qDebug("errorOccurred : %s : FailedToStart", qPrintable(uuid.toString()));
        break;

    case 1:
        qDebug("errorOccurred : %s : FailedToStart", qPrintable(uuid.toString()));
        break;

    case 2:
        qDebug("errorOccurred : %s : Timedout", qPrintable(uuid.toString()));
        break;

    case 3:
        qDebug("errorOccurred : %s : ReadError", qPrintable(uuid.toString()));
        break;

    case 4:
        qDebug("errorOccurred : %s : WriteError", qPrintable(uuid.toString()));
        break;

    case 5:
        qDebug("errorOccurred : %s : UnknownError", qPrintable(uuid.toString()));
        break;

    default:
        qDebug("errorOccurred : %s : default", qPrintable(uuid.toString()));
        break;
    }
}

void IperfServer::onStandartError()
{
    qDebug("IperfServer : standartError (%s): %s", qPrintable(uuid.toString()), qPrintable(process->errorString()));
}

void IperfServer::onStarted()
{
    qDebug("IperfServer : Процесс запущен и готов к чтению и записи %s", qPrintable(uuid.toString()));
    emit processStateChaned(uuid.toString(), ProcessState::Running);
}
