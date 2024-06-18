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
            qDebug("IperfServer : Процесс запускается, но программа еще не была вызвана %s", qPrintable(getUuid()));
//            setState(ProcessState::Starting);
            break;
        case 2: // running
//            qDebug("stateChanged : %s : Процесс запущен и готов к чтению и записи", qPrintable(uuid.toString()));

            break;
  }
    });

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](int exitCode, QProcess::ExitStatus exitStatus)
    {
        qDebug("IperfServer : Процес завершился %s. exitCode: %i. exitStatus: %i.",
               qPrintable(getUuid()),
               exitCode,
               exitStatus
        );
        setState(ProcessState::Finished);

        if(!stoped){
            QJsonObject body{
                {"efrewf", "fessef"}
            };
            network->post("/iperf", body);
            emit deleteProcess(getUuid());
        }
    });
}
