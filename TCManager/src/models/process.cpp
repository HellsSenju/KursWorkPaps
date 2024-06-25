#include "process.h"

Process::Process(QUuid processUuid)
{
    process = new QProcess();
    uuid = processUuid;

    connect(process, &QProcess::readyReadStandardError, this, &Process::onStandartError);
    connect(process, &QProcess::errorOccurred, this, &Process::onErrorOccurred);
    connect(process, &QProcess::stateChanged, this, [=](QProcess::ProcessState newState)
    {
        switch (newState) {
        case 0: //not running
            break;
        case 1: //starting
            break;
        case 2: // running
            qDebug("Process : Процесс запущен и готов к чтению и записи %s", qPrintable(getUuid()));
            setState(ProcessState::Running);
            break;
  }
    });

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](int exitCode, QProcess::ExitStatus exitStatus)
    {
        qDebug("Process : Процес выполнился %s. exitCode: %i. exitStatus: %i.",
               qPrintable(getUuid()),
               exitCode,
               exitStatus
        );

        QString res = process->readAllStandardOutput();
        QStringList list = res.split("\n");

        for(int i = 1; i < list.size(); i++){
            QString el = list.at(i).split(' ').first();
            if(!el.isEmpty())
                interfaces.append(el);
        }

        qDebug() << "interfaces : " << interfaces;

        setState(ProcessState::Finished);
    });
}


