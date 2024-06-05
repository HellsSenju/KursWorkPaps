#include "tcprocess.h"

TCProcess::TCProcess(QUuid processUuid, QObject *parent)
    : QObject{parent}
{
    uuid = processUuid;
}

void TCProcess::setParams(const QString &program, const QStringList &args)
{

}
