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
//            qDebug("IperfClient : stateChanged : %s : Процесс not running", qPrintable(getUuid()));
            break;
        case 1: //starting
            qDebug("IperfClient : Процесс запускается, но программа еще не была вызвана %s", qPrintable(getUuid()));
//            setState(ProcessState::Starting);
            break;
        case 2: // running
//            qDebug("stateChanged : %s : Процесс запущен и готов к чтению и записи", qPrintable(getUuid()));

            break;
  }
    });

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](int exitCode, QProcess::ExitStatus exitStatus)
    {
        qDebug("IperfClient : Процесс клиента завершился %s. exitCode: %i. exitStatus: %i.",
               qPrintable(getUuid()),
               exitCode,
               exitStatus
        );

        setState(ProcessState::Finished);
    });
}
