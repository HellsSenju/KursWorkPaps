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

    /** Установка параметров для процесса */
    virtual void setParams(const QString &program, const QStringList &args){
        process->setProgram(program);
        process->setArguments(args);
    };

    /** Запуск процесса*/
    virtual void start(){
        process->start();
    };

    /** Остановка процесса*/
    virtual void stop(){
        stoped = true;
        process->terminate();
    };

    /** Получени идентификатора процесса */
    virtual QString getUuid(){
        return uuid.toString(QUuid::WithoutBraces);
    };

    /** Получение состояния процесса */
    virtual ProcessState getState(){
        return state;
    };


protected:
    QUuid uuid;
    ProcessState state;
    QProcess *process = nullptr;
    QString error = "";
    bool server;
    bool stoped = false;

    /** сеттер для состояния процесса */
    virtual void setState(ProcessState state){
        this->state = state;
        emit stateChanged(state);
    };

protected slots:

    /** слот для ошибок от процесса */
    virtual void onStandartError(){
        QString temp = process->readAllStandardError();
        error.append(temp + " ");
        qDebug("standartError (%s): %s", qPrintable(getUuid()), qPrintable(temp));
    };

    /** слот на сигнал о старте процесса */
    virtual void onStarted(){
        qDebug("Iperf : Процесс запущен и готов к чтению и записи %s", qPrintable(uuid.toString()));
        setState(ProcessState::Running);
    };

    /** слот на сигнал о получении стандартных ошибок */
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
    /** сигнал изменения статуса*/
    void stateChanged(ProcessState state);
    /** сигнал для удаления процесса из менеджера */
    void deleteProcess(const QString &uuid);

};

#endif // ABSTRACTIPERF_H
