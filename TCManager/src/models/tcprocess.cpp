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
            qDebug("stateChanged : Процесс not running %s", qPrintable(getUuid()));
            break;
        case 1: //starting
            qDebug("stateChanged : Процесс запускается, но программа еще не была вызвана %s", qPrintable(getUuid()));
//            setState(ProcessState::Starting);
            break;
        case 2: // running
            qDebug("stateChanged : Процесс запущен и готов к чтению и записи %s", qPrintable(getUuid()));
//            setState(ProcessState::Running);
            break;
  }
    });

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](int exitCode, QProcess::ExitStatus exitStatus)
    {
        qDebug("TCProcess : Процес выполнился %s. exitCode: %i. exitStatus: %i.",
               qPrintable(getUuid()),
               exitCode,
               exitStatus
        );
        if(exitStatus == 0)
            setState(ProcessState::Finished);
        else
            setState(ProcessState::Crashed);

        if(!stoped){

            QJsonObject body;
            body["uuid"] = getUuid();
            body["status"] = "вававава";

            error = process->readAllStandardError();
            if(!error.isEmpty())
                body["error"] = error;

            network->post("/iperf/finished", body);
            emit deleteProcess(getUuid());
        }
    });
}
