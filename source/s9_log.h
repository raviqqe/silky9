#ifndef LOG_H_
#define LOG_H_


#include <stdio.h>
#include <stdarg.h>

#include "config.h"


#define s9_log(log_level, ...) \
  s9_log_(log_level, __FILE__, __LINE__, __func__, __VA_ARGS__)


typedef enum {
  S9_LOG_LEVEL_DEBUG,
  S9_LOG_LEVEL_NOTICE,
  S9_LOG_LEVEL_ERROR,
  S9_LOG_LEVEL_WARNING,
} s9_log_level_t;


// don't use this function directly
s9_log_(const s9_log_level_t log_level,
        const char * const filename,
        const int line_num,
        const char * const function,
        ...);


#endif // LOG_H_
