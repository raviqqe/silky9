#ifndef S9_LOG_H_
#define S9_LOG_H_


#include <stdio.h>
#include <stdarg.h>

#include "s9_config.h"


#ifdef S9_CONFIG_DEBUG_LOG
#define s9_debug_log(log_level, ...) do { \
    if (log_level >= S9_CONFIG_DEBUG_LOG_LEVEL) { \
      fprintf(stderr, "s9:%s:%d:%s(): ", __FILE__, __LINE__, __func__); \
      s9_print_log_(__VA_ARGS__); \
    } \
  } while (0)
#else
#define s9_debug_log(debug_level, ...)
#endif

#define s9_log(log_level, ...) do { \
    if (log_level >= S9_CONFIG_LOG_LEVEL) { \
      s9_print_log_(__VA_ARGS__); \
    } \
  } while (0)


typedef enum {
  S9_LOG_LEVEL_VERBOSE,
  S9_LOG_LEVEL_NOTICE,
  S9_LOG_LEVEL_WARNING,
  S9_LOG_LEVEL_ERROR,
} s9_log_level_t;


void s9_print_log_(const char * const format, ...);


#endif // S9_LOG_H_
