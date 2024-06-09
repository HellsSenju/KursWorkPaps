#include "iperfclient.h"

IperfClient::IperfClient(QUuid processUuid)
{
    process = new QProcess();
    uuid = processUuid;
    server = false;

    connect(process, &QProcess::started, this, &IperfClient::onStarted);
    connect(process, &QProcess::readyReadStandardOutput, this, &IperfClient::onStandartOutput);
    connect(process, &QProcess::readyReadStandardError, this, &IperfClient::onStandartError);
    connect(process, &QProcess::errorOccurred, this, &IperfClient::onErrorOccurred);

    connect(process, &QProcess::stateChanged, this, [=](QProcess::ProcessState newState)
    {
        switch (newState) {
        case 0: //not running
            qDebug("IperfClient : stateChanged : %s : Процесс not running", qPrintable(uuid.toString()));
            break;
        case 1: //starting
            qDebug("IperfClient : stateChanged : %s : Процесс запускается, но программа еще не была вызвана", qPrintable(uuid.toString()));
            emit stateChanged(ProcessState::Starting);
            break;
        case 2: // running
//            qDebug("stateChanged : %s : Процесс запущен и готов к чтению и записи", qPrintable(uuid.toString()));

            break;
  }
    });

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](int exitCode, QProcess::ExitStatus exitStatus)
    {
        qDebug("IperfClient : Процесс клиента(%s) завершился. exitCode: %i. exitStatus: %i.",
               qPrintable(getUuid()),
               exitCode,
               exitStatus
        );

        emit stateChanged(ProcessState::Finished);
    });
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
//    process->waitForReadyRead();
}

void IperfClient::stop()
{
    process->terminate();
}

void IperfClient::waitForFinished(int sec)
{
    process->waitForFinished(sec);
}

void IperfClient::onStandartOutput()
{
    qDebug() << "IperfServer : standartOutput: " <<  process->readAll();

//    qDebug("IperfServer : standartOutput (%s): %s", qPrintable(processUuid.toString()), qPrintable(process->readAll()));
}

void IperfClient::onErrorOccurred(QProcess::ProcessError error)
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

void IperfClient::onStandartError()
{
    qDebug("IperfServer : standartError (%s): %s", qPrintable(uuid.toString()), qPrintable(process->errorString()));
}

void IperfClient::onStarted()
{
    qDebug("IperfClient : Процесс запущен и готов к чтению и записи %s", qPrintable(uuid.toString()));
    started = true;
    emit stateChanged(ProcessState::Running);
}
