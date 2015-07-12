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


Err
NodeMemory_init(NodeMemory * const nodeMemory)
{
  assert(nodeMemory != NULL);

  nodeMemory->capacity = config_INITIAL_CAPACITY_OF_NODE_MEMORY;
  nodeMemory->nodes = malloc(sizeof(Node) * nodeMemory->capacity);
  if (nodeMemory->nodes == NULL) {
    DEBUG_MESSAGE("Failed to do malloc() for a node memory.");
    return Err_MEMORY_ALLOC;
  }

  for (int index = 0; index < nodeMemory->capacity; index++) {
    nodeMemory->nodes[index] = Node_DUMMY;
  }

  return Err_OK;
}


Err
NodeMemory_final(NodeMemory * const nodeMemory)
{
  free(nodeMemory->nodes);

  return Err_OK;
}


Err
NodeMemory_getNodeOfId(NodeMemory * const nodeMemory, const NodeId nodeId,
                       Node ** const node)
{
  if (NodeId >= nodeMemory->capacity) {
    DEBUG_MESSAGE("Node id is out of range of a node mermory.");
    return Err_MEMORY_OUT_OF_RANGE;
  }

  *node = &nodeMemory->nodes[nodeId];

  return Err_OK;
}


static Int
calcNewCapacity(Int oldCapacity)
{
  return oldCapacity * 2;
}


Err
NodeMemory_setNodeOfId(NodeMemory * const nodeMemory, const NodeId nodeId,
                       const Node node)
{
  if (NodeId >= nodeMemory->capacity
      && (calcNewCapacity(nodeMemory->capacity)
      <= config_MAX_NODE_MEMORY_CAPACITY)) {
    Int oldCapacity = nodeMemory->capacity;

    *nodeMemory = (NodeMemory){
      .capacity = calcNewCapacity(nodeMemory->capacity),
      .nodes = realloc(nodeMemory->nodes,
                       calcNewCapacity(nodeMemory->capacity)),
    };
    if (nodeMemory->nodes == NULL) {
      DEBUG_MESSAGE("Failed to do realloc() for a node memory.");
      return Err_MEMORY_ALLOC;
    }

    for (int index = oldCapacity; index < nodeMemory->capacity; index++) {
      nodeMemory->nodes[index] = Node_DUMMY;
    }
  }

  nodeMemory->nodes[nodeId] = node;

  return Err_OK;
}
