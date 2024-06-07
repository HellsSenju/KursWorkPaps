#ifndef ABSTRACTIPERF_H
#define ABSTRACTIPERF_H

#include <QObject>
#include <QProcess>

class AbstractIperf : public QObject
{
    Q_OBJECT
public:
    virtual void setParams(const QString &program, const QStringList &args);
    virtual void start();
    virtual void stop();

    virtual void output();
    virtual bool started();
    virtual bool finished(int exitCode, QProcess::ExitStatus exitStatus);


protected:
    QProcess *process = nullptr;



signals:

};

#endif // ABSTRACTIPERF_H
