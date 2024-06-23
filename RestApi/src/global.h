#ifndef GLOBAL_H
#define GLOBAL_H

#include "httpsessionstore.h"
#include "staticfilecontroller.h"
#include "filelogger.h"
#include "databaseconnection.h"
#include "network.h"

using namespace stefanfrings;

/**
  Global objects that are shared by multiple source files
  of this project.
*/


/** Storage for session cookies */
extern  HttpSessionStore* sessionStore;

/** Controller for static files */
extern  StaticFileController* staticFileController;

/** Redirects log messages to a file */
extern  FileLogger* logger;

/** manager for db*/
extern DataBaseConnection* db;


#endif // GLOBAL_H
