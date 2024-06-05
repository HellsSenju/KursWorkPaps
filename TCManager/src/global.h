#ifndef GLOBAL_H
#define GLOBAL_H


#include "httpsessionstore.h"
#include "staticfilecontroller.h"
#include "filelogger.h"
#include "processespool.h"

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

extern ProcessesPool* pool;

#endif // GLOBAL_H
