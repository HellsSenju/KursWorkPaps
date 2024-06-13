#pragma once
#ifndef ABSTRACTIPERF_H
#define ABSTRACTIPERF_H

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QUuid>
#include <QString>

enum ProcessState {Starting, Running, Finished, Crashed, FailedToStart};
Q_DECLARE_METATYPE(ProcessState)

class AbstractIperf : public QObject
{
    Q_OBJECT

public:
    virtual ~AbstractIperf() = default;

    virtual void setParams(const QString &program, const QStringList &args) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void waitForFinished(int sec = 30) = 0; //-1 - бесконечное ожидание

    virtual QString getUuid(){
        return uuid.toString();
    };

    virtual const char* getUuidChar(){
        return qPrintable(uuid.toString());
    };

    virtual bool isStarted(){
        return started;
    }

    virtual bool isServer(){
        return server;
    }

//    virtual bool finished(int exitCode);


protected:
    QProcess *process = nullptr;
    QUuid uuid;
    bool server;
    bool started = false;

signals:
    void stateChanged(ProcessState state);

};

#endif // ABSTRACTIPERF_H
