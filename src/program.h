#ifndef _PROGRAM_H
#define _PROGRAM_H


#include "comm.h"
#include "err.h"
#include "message.h"
#include "type.h"
#include <string.h>


#define program_STDIN NULL

Err program_loadProgram(const char fileName[*]);


#endif
