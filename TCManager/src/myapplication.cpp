#include "myapplication.h"

MyApplication::MyApplication(int& argc, char** argv) :
    QCoreApplication(argc, argv) {}

bool MyApplication::notify(QObject *receiver, QEvent *event)
{
    bool done = true;
    try {
        done = QCoreApplication::notify(receiver, event);
    }
    catch (const std::exception& ex) {
        qDebug() << &ex;
    }
    catch (...) {
    }
    return done;
}


