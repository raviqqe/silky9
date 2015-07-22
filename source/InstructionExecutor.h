#ifndef _INST_H
#define _INST_H


#include "comm.h"
#include "type.h"


typedef Int InstId;


Int inst_executeInst(Node * const node, const Word operand);


#endif
