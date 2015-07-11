#ifndef _INST_H
#define _INST_H


#include "type.h"


#define inst_ID_PREFIX_LEN 8
#define inst_NUM_OF_OPERANDS_LEN (sizeof(Byte))
#define inst_ID(prefix, id, ops) \
    (((id) << (inst_ID_PREFIX_LEN + inst_NUM_OF_OPERANDS_LEN)) \
    + ((prefix) << (inst_NUM_OF_OPERANDS_LEN)) \
    + (ops))

typedef Int InstId; // its size is sizeof(Int) - sizeof(Byte)

const InstId inst_COPY  = inst_ID(0, 0, 1);
const InstId inst_IO    = inst_ID(0, 1, 1);

const InstId inst_BIT_NOT = inst_ID(1, 0, 1);
const InstId inst_BIT_AND = inst_ID(1, 1, 2);
const InstId inst_BIT_OR  = inst_ID(1, 2, 2);
const InstId inst_BIT_XOR = inst_ID(1, 3, 2);

const InstId inst_BOOL_NOT  = inst_ID(1, 4, 1);
const InstId inst_BOOL_AND  = inst_ID(1, 5, 2);
const InstId inst_BOOL_OR   = inst_ID(1, 6, 2);
const InstId inst_BOOL_XOR  = inst_ID(1, 7, 2);

const InstId inst_INT_EQUAL     = inst_ID(2, 0, 2);
const InstId inst_INT_LESSTHAN0 = inst_ID(2, 1, 1);
const InstId inst_INT_ADD       = inst_ID(2, 2, 2);
const InstId inst_INT_MULT      = inst_ID(2, 3, 2);
const InstId inst_INT_ADDINV    = inst_ID(2, 4, 1);
const InstId inst_INT_POW2      = inst_ID(2, 5, 1);
const InstId inst_INT_LOG2      = inst_ID(2, 6, 1);

const InstId inst_REAL_EQUAL      = inst_ID(3, 0, 2);
const InstId inst_REAL_LESSTHAN0  = inst_ID(3, 1, 1);
const InstId inst_REAL_ADD        = inst_ID(3, 2, 2);
const InstId inst_REAL_MULT       = inst_ID(3, 3, 2);
const InstId inst_REAL_ADDINV     = inst_ID(3, 4, 1);
const InstId inst_REAL_POW2       = inst_ID(3, 5, 1);
const InstId inst_REAL_LOG2       = inst_ID(3, 6, 1);
const InstId inst_REAL_MULTINV    = inst_ID(3, 7, 1);

Byte inst_getNumOfOperands(InstId instId);


#endif
