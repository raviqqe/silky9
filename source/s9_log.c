#include "s9_log.h"


void
s9_log_(const s9_log_level_t log_level,
        const char * const filename,
        const int line_num,
        const char * const function,
        const char * const format,
        ...)
{
  va_list variadic_args;
  va_start(variadic_args, format);

  switch (log_level) {
  case S9_LOG_LEVEL_DEBUG:
    fprintf(stderr, "s9:%s:%d:%s(): ", filename, line_num, function);
    break;
  case S9_LOG_LEVEL_NOTICE:
  case S9_LOG_LEVEL_WARNING:
  case S9_LOG_LEVEL_ERROR:
    break;
  default:
    fprintf(stderr, "Unknown log level detected.\n");
    break;
  }

  fprintf(stderr, format, variadic_args);
  fprintf(stderr, "\n");

  va_end(variadic_args);
}
