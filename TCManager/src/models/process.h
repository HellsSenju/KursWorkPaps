#ifndef PROCESS_H
#define PROCESS_H

#include "abstractprocess.h"

class Process : public AbstractProcess
{
    Q_OBJECT

public:
    explicit Process(QUuid processUuid);

    QString getOutput() override{
        return QJsonDocument(interfaces).toJson(QJsonDocument::Compact);
    };

private:
    QJsonArray interfaces;

};

#endif // PROCESS_H
