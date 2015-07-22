#include "instSet.h"


// types

typedef Int InstId;
typedef Word (*)(const Node, const Word) OneOperandInst;
typedef Word (*)(const Node *, const Word) OneOperandInst;


Word
OneOperandInst_ControlShutdown(const Node, const Word)
{

}

OneOperandInst OneOperandInstTable[4][16] = {
  {
    OneOperandInst_ControlShutdown,
    OneOperandInst_ControlCopy,
    OneOperandInst_ControlInput,
    OneOperandInst_ControlOutput,
  },
  {
    OneOperandInst_LogicBitAnd,
    OneOperandInst_LogicBitOr,
    OneOperandInst_LogicBitXor,
    OneOperandInst_LogicBoolAnd,
    OneOperandInst_LogicBoolOr,
    OneOperandInst_LogicBoolXor,
  },
  {
    OneOperandInst_IntEqual,
    OneOperandInst_IntGreaterThan,
    OneOperandInst_IntLessThan,
    OneOperandInst_IntAdd,
    OneOperandInst_IntMultiply,
    OneOperandInst_IntSubstByVar,
    OneOperandInst_IntSubstByConst,
    OneOperandInst_IntDivideByVar,
    OneOperandInst_IntDivideByConst,
    OneOperandInst_IntPowerOfVar,
    OneOperandInst_IntPowerOfConst,
    OneOperandInst_IntPowerByConst,
    OneOperandInst_IntPowerOfConst,
  },
  {
  },
};

TwoOperandInst TwoOperandInstTable[] = {
};
