#ifndef ABSTRACTPROCESS_H
#define ABSTRACTPROCESS_H

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QUuid>
#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "../globalnetwork.h"

enum ProcessState {Starting, Running, Finished, Crashed, FailedToStart};
Q_DECLARE_METATYPE(ProcessState)

class AbstractProcess : public QObject
{
    Q_OBJECT
public:
    virtual ~AbstractProcess(){
        process->deleteLater();
    };

    /** Установка параметров для процесса */
    virtual void setParams(const QString &program, const QStringList &args){
        process->setProgram(program);
        process->setArguments(args);
    };

    /** Запуск процесса*/
    virtual void execute(){
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

    /** Получение стандартного вывода процесса */
    virtual QString getOutput(){
        output.remove("\n");
        return output;
    };

    /** Получение ошибок от процесса */
    virtual QString getError(){
        error.remove("\n");
        return error;
    };

protected:
    QProcess *process = nullptr;
    QUuid uuid;
    ProcessState state;
    QString output = "";
    QString error = "";
    bool stoped = false;

    /** сеттер для состояния процесса */
    virtual void setState(ProcessState newState){
        state = newState;
        emit stateChanged(state);
    };

protected slots:
    /** слот для вывода */
    virtual void onStandartOutput(){
        output = process->readAll();
        qDebug() << "standartOutput: " << output;
    };

    /** слот для ошибок от процесса */
    virtual void onStandartError(){
        QString temp = process->readAllStandardError();
        error.append(temp);
        qDebug("standartError (%s): %s", qPrintable(getUuid()), qPrintable(temp));
    };

    /** слот на сигнал о получении стандартных ошибок */
    virtual void onErrorOccurred(QProcess::ProcessError error){
        switch (error) {
        case 0:
            qDebug("errorOccurred : %s : FailedToStart", qPrintable(getUuid()));
            setState(ProcessState::FailedToStart);
            break;

        case 1:
            qDebug("errorOccurred : %s : Crashed", qPrintable(getUuid()));
            break;

        case 2:
            qDebug("errorOccurred : %s : Timedout", qPrintable(getUuid()));
            break;

        case 3:
            qDebug("errorOccurred : %s : ReadError", qPrintable(getUuid()));
            break;

        case 4:
            qDebug("errorOccurred : %s : WriteError", qPrintable(getUuid()));
            break;

        case 5:
            qDebug("errorOccurred : %s : UnknownError", qPrintable(getUuid()));
            break;

        default:
            qDebug("errorOccurred : %s : default", qPrintable(getUuid()));
            break;
        }
    };


signals:
    /** сигнал изменения статуса*/
    void stateChanged(ProcessState state);
    /** сигнал для удаления процесса из менеджера */
    void deleteProcess(const QString &uuid);
};

#endif // ABSTRACTPROCESS_H
