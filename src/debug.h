#ifndef _DEBUG_H
#define _DEBUG_H


#include "config.h"

#include <stdio.h>


#define USER_INFO(...) do { \
      fprintf(stderr, __VA_ARGS__); \
      fprintf(stderr, "\n"); \
    } while (0)


#define DEBUG_INFO(...) do { \
      if (config_DEBUG) { \
        fprintf(stderr, "SILKY9:%s:%d:%s(): ", __FILE__, __LINE__, __func__); \
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\n"); \
      } \
    } while (0)


#endif
