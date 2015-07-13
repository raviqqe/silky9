#ifndef _MESSAGE_H
#define _MESSAGE_H


#include "debug.h"
#include "node.h"
#include "type.h"


typedef enum {
  MessageTag_TOKEN = 0,
  MessageTag_NODE_UPDATE = 1,
  MessageTag_SIGNAL = 2,
} MessageTag; // Byte size in design


typedef struct {
  const NodeId dest; // Global node ID
  const Word value;
} Token;

#define Token_DUMMY (Token){.dest = NodeId_DUMMY, .value = Word_DUMMY}

Token Token_of(const NodeId dest, const Word value);


typedef struct {
  const NodeId nodeId; // Global node ID
  const Node node;
} NodeUpdate;

#define NodeUpdate_DUMMY \
    (NodeUpdate){.nodeId = NodeId_DUMMY, .node = Node_DUMMY}

NodeUpdate NodeUpdate_of(const NodeId nodeId, const Node node);


typedef enum {
  Signal_SHUTDOWN = 0,
} Signal; // Int size in design (not VM's feature)


typedef struct {
  const MessageTag tag;
  const union {
    Token token;
    NodeUpdate nodeUpdate;
    Int signal; // Signal type but Int type to allocate enough space
  }; // payload
} Message;

Message Message_ofToken(const Token token);
Message Message_ofNodeUpdate(const NodeUpdate nodeUpdate);
Message Message_ofSignal(const Signal signal);
Token Message_getToken(const Message message);
NodeUpdate Message_getNodeUpdate(const Message message);
Signal Message_getSignal(const Message message);


#endif
