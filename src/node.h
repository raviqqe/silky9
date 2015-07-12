#ifndef _NODE_H
#define _NODE_H


#include "type.h"
#include "inst.h"


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

#define NodeId_DUMMY Int_DUMMY
#define Node_DUMMY (Node){ \
  .header = Int_DUMMY, \
  .subDest = NodeId_DUMMY, \
  .dest = NodeId_DUMMY, \
}

void Node_storeOperand(Node * const node, Word operand);
Word Node_takeOutOperand(Node * const node);
Word Node_copyOperand(const Node * const node);


typedef struct {
  Int capacity;
  Node *nodes;
} NodeMemory;

#define NodeMemory_DUMMY (NodeMemory){ \
  .capacity = Int_DUMMY, \
  .nodes = NULL,
}

Err NodeMemory_init(NodeMemory *nodeMemory);
Err NodeMemory_final(NodeMemory *nodeMemory);
Err NodeMemory_getNodeOfId(NodeMemory * const nodeMemory, const NodeId nodeId,
                           Node ** const node);
Err NodeMemory_setNodeOfId(NodeMemory * const nodeMemory, const NodeId nodeId,
                           Node node);
//NodeMemory NodeMemory_of(const Int capacity, const Node * const nodes);


#endif
