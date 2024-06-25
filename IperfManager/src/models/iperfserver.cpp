#include "iperfserver.h"

IperfServer::IperfServer(QUuid processUuid)
{
    process = new QProcess();
    uuid = processUuid;
    server = true;

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
            break;
        case 2: // running
            qDebug("IperfServer : %s : Процесс запущен и готов к чтению и записи", qPrintable(uuid.toString()));
            setState(ProcessState::Running);
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


        if(!stoped){
            QJsonObject body{
                {"uuid", getUuid()},
                {"from", "IperfManager"},
                {"exitStatus", exitStatus},
                {"exitCode", exitCode}
            };
            body["command"] = process->arguments().join(" ");

            if(!error.isEmpty())
                body["error"] = error;

            network->post("/iperf/finished", body);
            emit deleteProcess(getUuid());
        }

        if(exitStatus == 0){
            setState(ProcessState::Finished);
        }
        else{
            setState(ProcessState::Crashed);
        }
    });
}
