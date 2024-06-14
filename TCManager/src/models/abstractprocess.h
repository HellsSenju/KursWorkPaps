#ifndef ABSTRACTPROCESS_H
#define ABSTRACTPROCESS_H

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QUuid>
#include <QString>

enum ProcessState {Starting, Running, Finished, Crashed, FailedToStart};
Q_DECLARE_METATYPE(ProcessState)

class AbstractProcess : public QObject
{
    Q_OBJECT
public:
    virtual ~AbstractProcess() = default;

    virtual void setParams(const QString &program, const QStringList &args) = 0;
    virtual void execute() = 0;
    virtual void stop() = 0;

    virtual QString getUuid(){
        return uuid.toString();
    };

    virtual const char* getUuidChar(){
        return qPrintable(uuid.toString());
    };

    virtual ProcessState getState(){
        return state;
    };

protected:
    QProcess *process = nullptr;
    QUuid uuid;
    ProcessState state;

    virtual void setState(ProcessState newState){
        state = newState;
    };

private slots:
    virtual void onStandartOutput() = 0;
    virtual void onErrorOccurred(QProcess::ProcessError error) = 0;
    virtual void onStandartError() = 0;


signals:
    void stateChanged(ProcessState state);
};

#endif // ABSTRACTPROCESS_H
