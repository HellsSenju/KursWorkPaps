#ifndef PROCESSESPOOL_H
#define PROCESSESPOOL_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QMap>
#include <QUuid>
#include <QStringList>

class ProcessesPool : public QObject
{
    Q_OBJECT
public:
    explicit ProcessesPool(QObject *parent = nullptr);
    ~ProcessesPool();

private:
//    QMap<QString, >

public slots:
    void onAddRule(const QString &uuid, const QString &command);
    void onDeleteRule(const QString &uuid, const QString &command);
    void onUpdateRule(const QString &uuid, const QString &command);
    void onGetRules(const QString &uuid, const QString &command);

};

#endif // PROCESSESPOOL_H
