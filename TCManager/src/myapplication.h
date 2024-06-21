#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include <QCoreApplication>
#include <QDebug>

class MyApplication : public QCoreApplication
{
public:
    MyApplication(int& argc, char** argv);

    bool notify(QObject* receiver, QEvent* event);
};

#endif // MYAPPLICATION_H
