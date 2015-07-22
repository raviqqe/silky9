#include "inst.h"


// types

typedef InstId InstIdFieldMask;


// constants

const InstIdFieldMask kNumOfOperandsFieldMask = 0b001;
const InstIdFieldMask kCategoryIdFieldMask    = 0b110;


// functions

static Int
InstIdFieldMask_getPosition(const InstIdFieldMask mask)
{
  Int position = 0;
  InstIdFieldMask maskShifted = mask;

  while ((maskShifted & 1) == 0) {
    maskShifted >>= 1;
    position++;
    assert(position < sizeof(maskShifted) * 8);
  }

  return position;
}


static Int
InstId_getFieldValue(const InstId instId, const InstIdFieldMask mask)
{
  return (instId & mask) >> InstIdFieldMask_getPosition(mask);
}


static Int
InstId_getNumOfOperands(const InstId instId)
{
  return InstId_getFieldValue(instId, kNumOfOperandsFieldMask);
}


static Int
InstId_getCategoryId(const InstId instId) {
  return InstId_getFieldValue(instId, kCategoryIdFieldMask);
}


static Err
sendWord(const NodeId dest, const Word value)
{
  return comm_sendMessage(Message_ofToken(Token_of(dest, value)));
}


static Err
sendInt(NodeId dest, Int intNum)
{
  return sendWord(dest, Word_ofInt(intNum));
}


static Err
sendReal(NodeId dest, Real realNum)
{
  return sendWord(dest, Word_ofReal(realNum));
}


Err
inst_executeInst(Node * const node, const Word operand)
{
  Err err = Err_OK;

  if (InstId_getNumOfOperands(node->instId) == 2
      && !node->operandPresentBit) {
    Node_storeOperand(node, operand);
  } else {
    Word operandInNode;

    switch (node->instId) {
    case inst_COPY:
      sendWord(node->subDest, operand);
      sendWord(node->dest, operand);
      break;
    case inst_IO:
      //sendInt(node->dest, io(operand.intNum));
      break;
    case inst_SHUTDOWN:
      err = comm_sendMessage(Message_ofSignal(Signal_SHUTDOWN));
      break;
    case inst_BOOL_NOT:
      err = sendInt(node->dest, !operand.intNum);
      break;
    case inst_BOOL_AND:
      err = sendInt(node->dest,
                    operand.intNum && Node_takeOutOperand(node).intNum);
      break;
    case inst_BOOL_OR:
      err = sendInt(node->dest,
                    operand.intNum || Node_takeOutOperand(node).intNum);
      break;
    case inst_BOOL_XOR:
      operandInNode.intNum = Node_takeOutOperand(node).intNum;
      err = sendInt(node->dest,
                    !(operand.intNum && operandInNode.intNum)
                    && !(operand.intNum || operandInNode.intNum));
      break;
    default:
      DEBUG_INFO("Unknown instruction detected. (instruction id: %x)",
                 node->instId);
      return Err_INST_UNKNOWN;
    }

    if (err) {
      DEBUG_INFO("Failed to execute an instruction of code, %d.",
                 node->instId);
      return Err_INST_EXEC;
    }
  }

  return err;
}
