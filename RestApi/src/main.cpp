#include <QCoreApplication>
#include <QDir>
#include <QSettings>
#include <QUuid>

#include "httplistener.h"
#include "global.h"
#include "globalnetwork.h"
#include "controllers/requestmapper.h"

QString searchConfigFile()
{
    QString binDir=QCoreApplication::applicationDirPath();
    QString appName=QCoreApplication::applicationName();
    QString fileName("RestApi.ini");

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

QString searchHelperFile()
{
    QString binDir=QCoreApplication::applicationDirPath();
    QString appName=QCoreApplication::applicationName();
    QString fileName("helpers.json");

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
            qDebug("Using helper file %s",qPrintable(fileName));
            return fileName;
        }
    }

    // not found
    foreach (QString dir, searchList)
    {
        qWarning("%s/%s not found",qPrintable(dir),qPrintable(fileName));
    }
    qFatal("Cannot find helper file %s",qPrintable(fileName));

    return nullptr;
}


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("RestApi");

    // поиск конфигурационного файла
    QString configFileName = searchConfigFile();
    QString helperFileName = searchHelperFile();

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

    // настройка DataBaseConnection
    QSettings* dbSettings = new QSettings(configFileName, QSettings::IniFormat, &app);
    dbSettings->beginGroup("db");
    db = new DataBaseConnection(helperFileName, dbSettings, &app);
    db->connect();

    // настройка и старт TCP listener
    QSettings* listenerSettings = new QSettings(configFileName,QSettings::IniFormat, &app);
    listenerSettings->beginGroup("listener");
    new HttpListener(listenerSettings, new RequestMapper(&app), &app);


    // настройка Network
    QSettings* apiSettings = new QSettings(configFileName,QSettings::IniFormat, &app);
    apiSettings->beginGroup("api");
    network = new Network(apiSettings);

    qWarning("Application has started");
    return app.exec();
    qWarning("Application has stopped");
}
