#pragma once
#ifndef ABSTRACTIPERF_H
#define ABSTRACTIPERF_H

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QUuid>
#include <QString>

enum ProcessState {Starting, Running, Finished, Crashed, FaledToStart};
Q_DECLARE_METATYPE(ProcessState)

class AbstractIperf : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ProcessState state READ state WRITE setState NOTIFY stateChanged)

public:
    virtual ~AbstractIperf() = default;

    virtual void setParams(const QString &program, const QStringList &args) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void waitForFinished(int sec = 30) = 0; //-1 - бесконечное ожидание

    virtual QString getUuid(){
        return uuid.toString();
    };

//    virtual bool finished(int exitCode);


protected:
    QProcess *process = nullptr;
    QUuid uuid;
    bool server;

};

#endif // ABSTRACTIPERF_H
