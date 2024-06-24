#pragma once
#ifndef ABSTRACTIPERF_H
#define ABSTRACTIPERF_H

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QUuid>
#include <QString>

#include "../globalnetwork.h"

enum ProcessState {Starting, Running, Finished, Crashed, FailedToStart};
Q_DECLARE_METATYPE(ProcessState)

class AbstractIperf : public QObject
{
    Q_OBJECT

public:
    virtual ~AbstractIperf(){
        process->deleteLater();
    };

    virtual void setParams(const QString &program, const QStringList &args){
        process->setProgram(program);
        process->setArguments(args);
    };

    virtual void start(){
        process->start();
    };

    virtual void stop(){
        process->terminate();
        stoped = true;
    };

    virtual void waitForFinished(int sec = 30){
        process->waitForFinished(sec);
    }; //-1 - бесконечное ожидание

    virtual QString getUuid(){
        return uuid.toString(QUuid::WithoutBraces);
    };

    virtual const char& getUuidChar(){
        return *qPrintable(uuid.toString(QUuid::WithoutBraces));
    };

    virtual ProcessState getState(){
        return state;
    };

    virtual bool isServer(){
        return server;
    }

protected:
    QUuid uuid;
    ProcessState state;
    QProcess *process = nullptr;
    QString error = "";
    bool server;
    bool stoped = false;

    virtual void setState(ProcessState state){
        this->state = state;
        emit stateChanged(state);
    };

protected slots:
    virtual void onStandartOutput(){
//        qDebug() << "Iperf : standartOutput: " <<  process->readAll();
    };

    virtual void onStandartError(){
        QString temp = process->readAllStandardError();
        error.append(temp + " ");
        qDebug("standartError (%s): %s", qPrintable(getUuid()), qPrintable(temp));
    };

    virtual void onStarted(){
        qDebug("Iperf : Процесс запущен и готов к чтению и записи %s", qPrintable(uuid.toString()));
        setState(ProcessState::Running);
    };

    virtual void onErrorOccurred(QProcess::ProcessError error){
        switch (error) {
        case QProcess::ProcessError::FailedToStart :
            qDebug("Iperf : errorOccurred : %s : FailedToStart", qPrintable(getUuid()));
            emit stateChanged(ProcessState::FailedToStart);
            break;

        case QProcess::ProcessError::Crashed:
            qDebug("Iperf : errorOccurred : %s : Crashed", qPrintable(getUuid()));
            break;

        case QProcess::ProcessError::Timedout:
            qDebug("Iperf : errorOccurred : %s : Timedout", qPrintable(getUuid()));
            break;

        case QProcess::ProcessError::UnknownError:
            qDebug("Iperf : errorOccurred : %s : UnknownError", qPrintable(getUuid()));
            break;

        default:
            qDebug("Iperf : errorOccurred : %s : default", qPrintable(getUuid()));
            break;
        }
    };

signals:
    void stateChanged(ProcessState state);
    void deleteProcess(const QString &uuid);

};

#endif // ABSTRACTIPERF_H
