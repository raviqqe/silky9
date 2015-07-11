#include "inst.h"


Inst
inst_getNumOfOperands(InstId instId)
{
  // lowest bit of instId specifies its num of operands - 1.
  // i.e. 0 for 1 operands, 1 for 2 operands.
  return (instId & 1) + 1;
}
