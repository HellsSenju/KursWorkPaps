#include <QCoreApplication>
#include <QCoreApplication>
#include <QDir>
#include <QSettings>
#include <QUuid>

#include "myapplication.h"
#include "processespool.h"
#include "httplistener.h"
#include "global.h"
#include "controllers/requestmapper.h"
#include "../IperfManager/src/globalnetwork.h"

QString searchConfigFile()
{
    QString binDir=QCoreApplication::applicationDirPath();
    QString appName=QCoreApplication::applicationName();
    QString fileName("TCManager.ini");

    QStringList searchList;
    searchList.append(binDir);
    searchList.append(binDir+"/etc");
    searchList.append(binDir+"/../etc");
    searchList.append(binDir+"/../"+appName+"/etc");     // for development with shadow build (Linux)
    searchList.append(binDir+"/../../"+appName+"/etc");  // for development with shadow build (Windows)
    searchList.append(QDir::rootPath()+"etc/opt");
    searchList.append(QDir::rootPath()+"etc");

    foreach (QString dir, searchList)
    {
        QFile file(dir+"/"+fileName);
        if (file.exists())
        {
            fileName=QDir(file.fileName()).canonicalPath();
            qDebug("Using config file %s",qPrintable(fileName));
            return fileName;
        }
    }

    // not found
    foreach (QString dir, searchList)
    {
        qWarning("%s/%s not found",qPrintable(dir),qPrintable(fileName));
    }
    qFatal("Cannot find config file %s",qPrintable(fileName));
    return nullptr;
}

int main(int argc, char *argv[])
{
//    QCoreApplication app(argc, argv);
    MyApplication app(argc, argv);
    app.setApplicationName("TCManager");

    // регистрация пользовательского типа
    qRegisterMetaType<ProcessState>();

    // поиск конфигурационного файла
    QString configFileName=searchConfigFile();

    // настройка логирования в файл
    QSettings* logSettings=new QSettings(configFileName,QSettings::IniFormat, &app);
    logSettings->beginGroup("logging");
    FileLogger* logger = new FileLogger(logSettings, 10000, &app);
    logger->installMsgHandler();

    // настройка session store
    QSettings* sessionSettings = new QSettings(configFileName,QSettings::IniFormat, &app);
    sessionSettings->beginGroup("sessions");
    sessionStore=new HttpSessionStore(sessionSettings, &app);

    // настройка static file controller
    QSettings* fileSettings = new QSettings(configFileName,QSettings::IniFormat, &app);
    fileSettings->beginGroup("docroot");
    staticFileController = new StaticFileController(fileSettings, &app);

    // настройка и старт TCP listener
    QSettings* listenerSettings = new QSettings(configFileName,QSettings::IniFormat, &app);
    listenerSettings->beginGroup("listener");
    new HttpListener(listenerSettings, new RequestMapper(&app), &app);

    // создание пула процессов
    pool = new ProcessesPool();

    // настройка Network
    QSettings* apiSettings = new QSettings(configFileName,QSettings::IniFormat, &app);
    apiSettings->beginGroup("api");
    network = new Network(apiSettings);

//    qDebug() << QUuid::createUuid().toString();
//    qDebug() << QUuid::createUuid().toString();
//    qDebug() << QUuid::createUuid().toString();
//    qDebug() << QUuid::createUuid().toString();
//    qDebug() << QUuid::createUuid().toString();

    qWarning("Application has started");
    app.exec();
    qWarning("Application has stopped");

//    return app.exec();
}
