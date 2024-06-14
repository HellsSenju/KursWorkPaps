#ifndef PROCESS_H
#define PROCESS_H

#include "abstractprocess.h"

class Process : public AbstractProcess
{
    Q_OBJECT

public:
    explicit Process(QUuid processUuid);
    ~ Process();

public:
    void setParams(const QString &program, const QStringList &args);
    void execute();
    void stop();

private slots:
    void onStandartOutput();
    void onErrorOccurred(QProcess::ProcessError error);
    void onStandartError();
};

#endif // PROCESS_H
