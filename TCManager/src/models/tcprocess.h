#ifndef TCPROCESS_H
#define TCPROCESS_H

#include "abstractprocess.h"

class TCProcess : public AbstractProcess
{
    Q_OBJECT
public:
    explicit TCProcess(QUuid processUuid);


};

#endif // TCPROCESS_H
