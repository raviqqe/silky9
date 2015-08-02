#include "s9_log.h"


void
s9_print_log_(const char * const format, ...)
{
  va_list variadic_args;
  va_start(variadic_args, format);

  fprintf(stderr, format, variadic_args);
  fprintf(stderr, "\n");

  va_end(variadic_args);
}
