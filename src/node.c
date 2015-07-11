#include "node.h"


void
Node_storeOperand(Node * const node, Word operand)
{
  assert(node.operandPresentBit == false);
  assert(node.operandBuff == Word_DUMMY);

  node.operandPresentBit = true;
  node.operandBuff = operand;
}


Word
Node_takeOutOperand(Node * const node)
{
  assert(node.operandPresentBit == true);

  Word returnedOperand = node.operandBuff;
  node.operandPresentBit = false;
  node.operandBuff = Word_DUMMY;

  return returnedOperand;
}


Word
Node_copyOperand(const Node * const node)
{
  return node.operandBuff;
}
