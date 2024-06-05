#ifndef TCPROCESS_H
#define TCPROCESS_H

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QUuid>

class TCProcess : public QObject
{
    Q_OBJECT
public:
    explicit TCProcess(QUuid processUuid, QObject *parent = nullptr);

    void setParams(const QString &program, const QStringList &args);
    void execute();

protected:
    QProcess *process = nullptr;
    QUuid uuid;

signals:

};

#endif // TCPROCESS_H
