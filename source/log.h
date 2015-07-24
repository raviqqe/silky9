#ifndef _LOG_H
#define _LOG_H


#include "Config.h"

#include <stdio.h>
#include <stdarg.h>


#ifdef Config_DEBUG
#define DEBUG_LOG(...) do { \
      fprintf(stderr, "Silky9:%s:%d:%s(): ", __FILE__, __LINE__, __func__); \
      Log(__VA_ARGS__); \
    } while (0)
#else
#define LOG_DEBUG(...)
#endif


typedef enum {
  LogType_FIRST_,
  LogType_WARNING,
  LogType_ERROR,
  LogType_LAST_,
} LogType;


void Log(LogType log_type, const char * const kFormat, ...);


#endif
