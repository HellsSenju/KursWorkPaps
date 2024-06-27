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

    /** запуск процесса */
    void execute(const QString &uuid, Programs program, const QString &command);

    /** удаление процесса из пула */
    void deleteProcess(const QString &uuid);

    /** получение состояния процесса */
    ProcessState getProcessState(const QString& uuid){
        return pool.value(uuid)->getState();
    };

    /** получение вывода процесса */
    QString getProcessOutput(const QString& uuid){
        return pool.value(uuid)->getOutput();
    };

    /** получение ошибок процесса */
    QString getProcessError(const QString& uuid){
        return pool.value(uuid)->getError();
    };

    /** проверка идентификатора */
    bool checkDublicates(const QString& uuid){
        return pool.contains(uuid);
    };

private:
    const QString tc = "tc";
    const QString nmcli = "nmcli";

    QMap<QString, AbstractProcess*> pool;

public slots:
    void onStateChanged(ProcessState state);

signals:
    void executed();
};

#endif // PROCESSESPOOL_H
