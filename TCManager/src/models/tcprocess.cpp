#include "tcprocess.h"

TCProcess::TCProcess(QUuid processUuid)
{
    process = new QProcess();
    uuid = processUuid;

    connect(process, &QProcess::readyReadStandardError, this, &TCProcess::onStandartError);
    connect(process, &QProcess::errorOccurred, this, &TCProcess::onErrorOccurred);
    connect(process, &QProcess::stateChanged, this, [=](QProcess::ProcessState newState)
    {
        switch (newState) {
        case 0: //not running
            break;
        case 1: //starting
            break;
        case 2: // running
            qDebug("TCProcess : Процесс запущен и готов к чтению и записи %s", qPrintable(getUuid()));
            setState(ProcessState::Running);
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

        output = process->readAllStandardOutput();

        QJsonObject body;
        body["uuid"] = getUuid();
        body["from"] = "TCManager";
        body["command"] = process->arguments().join(" ");
        body["output"] = output;
        body["exitStatus"] = exitStatus;
        body["exitCode"] = exitCode;

        if(!error.isEmpty())
            body["error"] = error;

        network->post("/finished", body);

        if(exitStatus == 0){
            setState(ProcessState::Finished);
        }
        else{
            setState(ProcessState::Crashed);
        }
    });
}
