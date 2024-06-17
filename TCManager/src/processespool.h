#ifndef PROCESSESPOOL_H
#define PROCESSESPOOL_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QMap>
#include <QUuid>
#include <QStringList>
#include <QString>

#include "./models/abstractprocess.h"
#include "./models/tcprocess.h"
#include "./models/process.h"

enum Programs {TC, NMCLI};
Q_DECLARE_METATYPE(Programs)

class ProcessesPool : public QObject
{
    Q_OBJECT
public:
    explicit ProcessesPool(QObject *parent = nullptr);
    ~ProcessesPool();

    void execute(const QString &uuid, Programs program, const QString &command);
    void deleteProcess(const QString &uuid);

    ProcessState getProcessState(const QString& uuid){
        return pool.value(uuid)->getState();
    };

    QString getProcessOutput(const QString& uuid){
        return pool.value(uuid)->getOutput();
    };

private:
    const QString tc = "tc";
    const QString nmcli = "nmcli";

    QMap<QString, AbstractProcess*> pool;

public slots:
    void onStateChanged(ProcessState state);

//    void onAddRule(const QString &uuid, const QString &command);
//    void onDeleteRule(const QString &uuid, const QString &command);
//    void onUpdateRule(const QString &uuid, const QString &command);
//    void onGetRules(const QString &uuid, const QString &command);

signals:
    void executed();
};

#endif // PROCESSESPOOL_H
