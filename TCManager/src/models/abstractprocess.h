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

    virtual void setParams(const QString &program, const QStringList &args){
        process->setProgram(program);
        process->setArguments(args);
    };

    virtual void execute(){
        process->start();
    };

    virtual void stop(){
        stoped = true;
        process->terminate();
    };

    virtual QString getUuid(){
        return uuid.toString(QUuid::WithoutBraces);
    };

    virtual const char* getUuidChar(){
        return qPrintable(uuid.toString(QUuid::WithoutBraces));
    };

    virtual ProcessState getState(){
        return state;
    };

    virtual QString getOutput(){
        return output;
    };

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

    virtual void setState(ProcessState newState){
        state = newState;
        emit stateChanged(state);
    };

protected slots:
    virtual void onStandartOutput(){
        output = process->readAll();
        qDebug() << "standartOutput: " << output;
    };

    virtual void onStandartError(){
        QString temp = process->readAllStandardError();
        if(temp.contains("Error"))
            error = temp;
        qDebug("standartError (%s): %s", qPrintable(getUuid()), qPrintable(temp));
    };

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
    void stateChanged(ProcessState state);
    void deleteProcess(const QString &uuid);
};

#endif // ABSTRACTPROCESS_H
