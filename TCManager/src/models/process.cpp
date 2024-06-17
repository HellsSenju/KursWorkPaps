#include "process.h"

Process::Process(QUuid processUuid)
{
    process = new QProcess();
    uuid = processUuid;

    connect(process, &QProcess::readyReadStandardOutput, this, &Process::onStandartOutput);
    connect(process, &QProcess::readyReadStandardError, this, &Process::onStandartError);
    connect(process, &QProcess::errorOccurred, this, &Process::onErrorOccurred);
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
        qDebug("Процес выполнился %s. exitCode: %i. exitStatus: %i.",
               qPrintable(getUuid()),
               exitCode,
               exitStatus
        );
        setState(ProcessState::Finished);
    });
}

void Process::onStandartOutput()
{
    QString str = process->readAll();
    QStringList list = str.split("\n");

    for(int i = 1; i < list.size(); i++){
        QString el = list.at(i).split(' ').first();
        if(!el.isEmpty())
            interfaces.append(el);
    }

//    qDebug() << "standartOutput: " << interfaces;
}

