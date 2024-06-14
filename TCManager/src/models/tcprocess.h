#ifndef TCPROCESS_H
#define TCPROCESS_H

#include "abstractprocess.h"

class TCProcess : public AbstractProcess
{
    Q_OBJECT
public:
    explicit TCProcess(QUuid processUuid);
    ~TCProcess();

    void setParams(const QString &program, const QStringList &args);
    void execute();
    void stop();

private slots:
    void onStandartOutput();
    void onErrorOccurred(QProcess::ProcessError error);
    void onStandartError();

};

#endif // TCPROCESS_H
