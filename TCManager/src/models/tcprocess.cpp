#include "tcprocess.h"

TCProcess::TCProcess(QUuid processUuid)
{
    process = new QProcess();
    uuid = processUuid;

    connect(process, &QProcess::readyReadStandardOutput, this, &TCProcess::onStandartOutput);
    connect(process, &QProcess::readyReadStandardError, this, &TCProcess::onStandartError);
    connect(process, &QProcess::errorOccurred, this, &TCProcess::onErrorOccurred);
    connect(process, &QProcess::stateChanged, this, [=](QProcess::ProcessState newState)
    {
        switch (newState) {
        case 0: //not running
            qDebug("stateChanged : Процесс not running %s", qPrintable(uuid.toString()));
            break;
        case 1: //starting
            qDebug("stateChanged : Процесс запускается, но программа еще не была вызвана %s", qPrintable(uuid.toString()));
            setState(ProcessState::Starting);
            break;
        case 2: // running
            qDebug("stateChanged : Процесс запущен и готов к чтению и записи %s", qPrintable(uuid.toString()));
            setState(ProcessState::Running);
            break;
  }
    });

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](int exitCode, QProcess::ExitStatus exitStatus)
    {
        qDebug("Процес выполнился %s. exitCode: %i. exitStatus: %i.",
               getUuidChar(),
               exitCode,
               exitStatus
        );
        setState(ProcessState::Finished);
        emit stateChanged(ProcessState::Finished);
    });
}

TCProcess::~TCProcess()
{
    process->deleteLater();
}

void TCProcess::setParams(const QString &program, const QStringList &args)
{
    process->setProgram(program);
    process->setArguments(args);
}

void TCProcess::execute()
{
    process->start();
}

void TCProcess::stop()
{
    process->terminate();
}

void TCProcess::onStandartOutput()
{
    qDebug() << "IperfServer : standartOutput: " <<  process->readAll();

//    qDebug("IperfServer : standartOutput (%s): %s", qPrintable(processUuid.toString()), qPrintable(process->readAll()));
}

void TCProcess::onErrorOccurred(QProcess::ProcessError error)
{
    switch (error) {
    case 0:
        qDebug("errorOccurred : %s : FailedToStart", qPrintable(uuid.toString()));
        setState(ProcessState::FailedToStart);
        emit stateChanged(ProcessState::FailedToStart);
        break;

    case 1:
        qDebug("errorOccurred : %s : Crashed", qPrintable(uuid.toString()));
        setState(ProcessState::Crashed);
        emit stateChanged(ProcessState::Crashed);
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

void TCProcess::onStandartError()
{
    qDebug("standartError (%s): %s", getUuidChar(), qPrintable(process->errorString()));
}
