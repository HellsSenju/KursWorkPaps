#ifndef PROCESSESPOOL_H
#define PROCESSESPOOL_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QMap>
#include <QUuid>
#include <QStringList>

#include "./models/tcprocess.h"

class ProcessesPool : public QObject
{
    Q_OBJECT
public:
    explicit ProcessesPool(QObject *parent = nullptr);
    ~ProcessesPool();

    void execute(const QString &uuid, const QString &command);
    ProcessState getProcessState(const QString& uuid){
        return pool.value(uuid)->getState();
    };

private:
    QMap<QString, TCProcess*> pool;

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
