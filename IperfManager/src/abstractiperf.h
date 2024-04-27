#pragma once
#ifndef ABSTRACTIPERF_H
#define ABSTRACTIPERF_H

#include <QObject>
#include <QProcess>
#include <QDebug>

class AbstractIperf : public QObject
{
    Q_OBJECT
public:
    virtual ~AbstractIperf() = default;

    virtual void setParams(const QString &program, const QStringList &args) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void waitForFinished(int sec = 30) = 0; //-1 - бесконечное ожидание

//    virtual bool finished(int exitCode);


protected:
    QProcess *process = nullptr;


signals:

};

#endif // ABSTRACTIPERF_H
