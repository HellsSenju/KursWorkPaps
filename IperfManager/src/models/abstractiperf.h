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

//    virtual bool finished(int exitCode);


protected:
    QUuid uuid;
    ProcessState state;
    QProcess *process = nullptr;
    bool server;

    virtual void setState(ProcessState state){
        this->state = state;
        emit stateChanged(state);
    };

protected slots:
    virtual void onStandartOutput(){
        qDebug() << "Iperf : standartOutput: " <<  process->readAll();
    };

    virtual void onErrorOccurred(QProcess::ProcessError error){
        switch (error) {
        case QProcess::ProcessError::FailedToStart :
            qDebug("Iperf : errorOccurred : %s : FailedToStart", qPrintable(getUuid()));
            emit stateChanged(ProcessState::Running);
            break;

        case QProcess::ProcessError::Crashed:
            qDebug("Iperf : errorOccurred : %s : Crashed", qPrintable(getUuid()));
            emit stateChanged(ProcessState::Crashed);
            break;

        case QProcess::ProcessError::Timedout:
            qDebug("Iperf : errorOccurred : %s : Timedout", qPrintable(getUuid()));
            break;

        case QProcess::ProcessError::ReadError:
            qDebug("Iperf : errorOccurred : %s : ReadError", qPrintable(getUuid()));
            break;

        case QProcess::ProcessError::WriteError:
            qDebug("Iperf : errorOccurred : %s : WriteError", qPrintable(getUuid()));
            break;

        case QProcess::ProcessError::UnknownError:
            qDebug("Iperf : errorOccurred : %s : UnknownError", qPrintable(getUuid()));
            break;

        default:
            qDebug("Iperf : errorOccurred : %s : default", qPrintable(getUuid()));
            break;
        }
    };

    virtual void onStandartError(){
        qDebug("Iperf : standartError (%s): %s", qPrintable(getUuid()), qPrintable(process->readAllStandardError()));
    };

    virtual void onStarted(){
        qDebug("Iperf : Процесс запущен и готов к чтению и записи %s", qPrintable(uuid.toString()));
        setState(ProcessState::Running);
    };

signals:
    void stateChanged(ProcessState state);

};

#endif // ABSTRACTIPERF_H
