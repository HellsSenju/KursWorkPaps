#ifndef ABSTRACTPROCESS_H
#define ABSTRACTPROCESS_H

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QUuid>
#include <QString>
#include <QStringList>
#include <QJsonArray>
#include <QJsonDocument>


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

protected:
    QProcess *process = nullptr;
    QUuid uuid;
    ProcessState state;
    QString output;

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
        qDebug("standartError (%s): %s", getUuidChar(), qPrintable(process->readAllStandardError()));
    };

    virtual void onErrorOccurred(QProcess::ProcessError error){
        switch (error) {
        case 0:
            qDebug("errorOccurred : %s : FailedToStart", qPrintable(uuid.toString()));
            setState(ProcessState::FailedToStart);
            break;

        case 1:
            qDebug("errorOccurred : %s : Crashed", qPrintable(uuid.toString()));
            setState(ProcessState::Crashed);
            break;

        case 2:
            qDebug("errorOccurred : %s : Timedout", qPrintable(uuid.toString()));
            break;

        case 3:
            qDebug("errorOccurred : %s : ReadError", qPrintable(uuid.toString()));
            break;

        case 4:
            qDebug("errorOccurred : %s : WriteError", qPrintable(uuid.toString()));
            break;

        case 5:
            qDebug("errorOccurred : %s : UnknownError", qPrintable(uuid.toString()));
            break;

        default:
            qDebug("errorOccurred : %s : default", qPrintable(uuid.toString()));
            break;
        }
    };


signals:
    void stateChanged(ProcessState state);
};

#endif // ABSTRACTPROCESS_H
