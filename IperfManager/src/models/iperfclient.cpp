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
//            qDebug("IperfClient : stateChanged : %s : Процесс not running", qPrintable(uuid.toString()));
            break;
        case 1: //starting
            qDebug("IperfClient : Процесс запускается, но программа еще не была вызвана %s", qPrintable(uuid.toString()));
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
        qDebug("IperfClient : Процесс клиента завершился %s. exitCode: %i. exitStatus: %i.",
               getUuidChar(),
               exitCode,
               exitStatus
        );

        setState(ProcessState::Finished);
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
        qDebug("IperfClient : errorOccurred : %s : FailedToStart", qPrintable(uuid.toString()));
        break;

    case 1:
        qDebug("IperfClient : errorOccurred : %s : Crashed", qPrintable(uuid.toString()));
        break;

    case 2:
        qDebug("IperfClient : errorOccurred : %s : Timedout", qPrintable(uuid.toString()));
        break;

    case 3:
        qDebug("IperfClient : errorOccurred : %s : ReadError", qPrintable(uuid.toString()));
        break;

    case 4:
        qDebug("IperfClient : errorOccurred : %s : WriteError", qPrintable(uuid.toString()));
        break;

    case 5:
        qDebug("IperfClient : errorOccurred : %s : UnknownError", qPrintable(uuid.toString()));
        break;

    default:
        qDebug("IperfClient : errorOccurred : %s : default", qPrintable(uuid.toString()));
        break;
    }
}

void IperfClient::onStandartError()
{
    qDebug("IperfClient : standartError (%s): %s", getUuidChar(), qPrintable(process->errorString()));
}

void IperfClient::onStarted()
{
    qDebug("IperfClient : Процесс запущен и готов к чтению и записи %s", qPrintable(uuid.toString()));
    setState(ProcessState::Running);
}
