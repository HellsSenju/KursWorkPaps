#ifndef PROCESS_H
#define PROCESS_H

#include "abstractprocess.h"

class Process : public AbstractProcess
{
    Q_OBJECT

public:
    explicit Process(QUuid processUuid);

    QString getOutput() override{
//        QJsonDocument doc;
//        doc.setArray(interfaces);

        return QJsonDocument(interfaces).toJson(QJsonDocument::Compact);
    };

private:
    QJsonArray interfaces;

private slots:
    void onStandartOutput() override;
};

#endif // PROCESS_H
