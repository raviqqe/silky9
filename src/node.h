#ifndef _NODE_H
#define _NODE_H


#include "type.h"
#include "inst.h"
#include <stdbool.h>


typedef Int NodeId; // this is not in design, but conforms to it.

typedef struct {
  union {
    Int header;
        // used to write operandPresentBit and instId at once
        // from program files
    struct {
      bool operandPresentBit : sizeof(Byte);
      const InstId instId : sizeof(Int) - sizeof(Byte);
    };
  };
  union {
    Word operandBuff;
    const NodeId subDest;
  };
  const NodeId dest;
} Node;


// dummy values

const NodeId NodeId_DUMMY = Int_DUMMY;
Node Node_DUMMY = {
  .header = Int_DUMMY,
  .subDest = NodeId_DUMMY,
  .dest = NodeId_DUMMY,
};


void Node_storeOperand(Node * const node, Word operand);
Word Node_takeOutOperand(Node * const node);
Word Node_copyOperand(const Node * const node);


#endif
