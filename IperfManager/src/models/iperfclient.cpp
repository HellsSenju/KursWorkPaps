#include "iperfclient.h"

IperfClient::IperfClient(QUuid processUuid)
{
    process = new QProcess();
    uuid = processUuid;
    server = false;

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
            qDebug("stateChanged : %s : Процесс запущен и готов к чтению и записи", qPrintable(getUuid()));
            setState(ProcessState::Running);
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

        QStringList output = QString(process->readAllStandardOutput()).split("\n");

        for(int i = 0; i < output.size(); i++){
            QString temp = output.at(i);
            if(temp.contains("Server Report")){
                QStringList results = output.at(i + 2).split(' ', Qt::SkipEmptyParts);
                if(results.size() != 12)
                    break;

                QStringList lostTotal = results.at(10).split('/');
                qDebug() << "lostTotal" << lostTotal;
                QString temp = results.at(11);
                QJsonObject body{
                    {"uuid", getUuid()},
                    {"interval", results.at(2) + " " + results.at(3)},
                    {"transfer", results.at(4) + " " + results.at(5)},
                    {"bandwidth", results.at(6) + " " + results.at(7)},
                    {"jitter", results.at(8) + " " + results.at(9)},
                    {"lost", lostTotal.at(0).toInt()},
                    {"total", lostTotal.at(1).toInt()},
                    {"lost/total", temp.remove('(').remove(')')},
                };

                network->post("/iperf/postStats", body);
            }
        }

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

            network->post("/finished", body);
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
