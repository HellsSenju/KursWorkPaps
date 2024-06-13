#ifndef TCPROCESS_H
#define TCPROCESS_H

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QUuid>

enum ProcessState {Starting, Running, Finished, Crashed, FailedToStart};
Q_DECLARE_METATYPE(ProcessState)

class TCProcess : public QObject
{
    Q_OBJECT
public:
    explicit TCProcess(QUuid processUuid, QObject *parent = nullptr);
    ~TCProcess();

    void setParams(const QString &program, const QStringList &args);
    void execute();
    void stop();

    QString getUuid(){
        return uuid.toString();
    };

    const char* getUuidChar(){
        return qPrintable(uuid.toString());
    };

    ProcessState getState(){
        return state;
    };

private:
    QProcess *process = nullptr;
    QUuid uuid;
    ProcessState state;

    void setState(ProcessState newState){
        state = newState;
    };

private slots:
    void onStandartOutput();
    void onErrorOccurred(QProcess::ProcessError error);
    void onStandartError();

signals:
    void stateChanged(ProcessState state);
};

#endif // TCPROCESS_H
