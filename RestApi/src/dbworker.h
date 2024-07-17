#ifndef DBWORKER_H
#define DBWORKER_H

#include <QObject>

class DbWorker : public QObject
{
    Q_OBJECT
public:
    explicit DbWorker(QObject *parent = nullptr);

signals:

};

#endif // DBWORKER_H
