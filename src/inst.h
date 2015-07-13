#ifndef _INST_H
#define _INST_H


#include "type.h"


#define inst_ID_PREFIX_LEN sizeof(Byte) * 8
#define inst_NUM_OF_OPERANDS_LEN (sizeof(Byte) * 8)
#define inst_ID(prefix, id, numOfOperands) \
    (((id) << (inst_ID_PREFIX_LEN + inst_NUM_OF_OPERANDS_LEN)) \
    + ((prefix) << (inst_NUM_OF_OPERANDS_LEN)) \
    + (numOfOperands))

typedef Int InstId; // its size is sizeof(Int) - sizeof(Byte)

#define inst_COPY     inst_ID(0, 0, 1)
#define inst_IO       inst_ID(0, 1, 1)
#define inst_SHUTDOWN inst_ID(0, 2, 1)

#define inst_BIT_NOT  inst_ID(1, 0, 1)
#define inst_BIT_AND  inst_ID(1, 1, 2)
#define inst_BIT_OR   inst_ID(1, 2, 2)
#define inst_BIT_XOR  inst_ID(1, 3, 2)

#define inst_BOOL_NOT   inst_ID(1, 4, 1)
#define inst_BOOL_AND   inst_ID(1, 5, 2)
#define inst_BOOL_OR    inst_ID(1, 6, 2)
#define inst_BOOL_XOR   inst_ID(1, 7, 2)

#define inst_INT_EQUAL      inst_ID(2, 0, 2)
#define inst_INT_LESSTHAN0  inst_ID(2, 1, 1)
#define inst_INT_ADD        inst_ID(2, 2, 2)
#define inst_INT_MULT       inst_ID(2, 3, 2)
#define inst_INT_ADDINV     inst_ID(2, 4, 1)
#define inst_INT_POW2       inst_ID(2, 5, 1)
#define inst_INT_LOG2       inst_ID(2, 6, 1)

#define inst_REAL_EQUAL     inst_ID(3, 0, 2)
#define inst_REAL_LESSTHAN0 inst_ID(3, 1, 1)
#define inst_REAL_ADD       inst_ID(3, 2, 2)
#define inst_REAL_MULT      inst_ID(3, 3, 2)
#define inst_REAL_ADDINV    inst_ID(3, 4, 1)
#define inst_REAL_POW2      inst_ID(3, 5, 1)
#define inst_REAL_LOG2      inst_ID(3, 6, 1)
#define inst_REAL_MULTINV   inst_ID(3, 7, 1)


Int inst_getNumOfOperands(const InstId instId);


#endif
