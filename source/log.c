#include "Log.h"


void
Log(LogType log_type, const char * const kFormat, ...)
{
  va_list variadic_args;
  va_start(variadic_args, kFormat);
  fprintf(stderr, kFormat, variadic_args);
  fprintf(stderr, "\n");
  va_end(variadic_args);
}
