#ifndef _MESSAGE_H
#define _MESSAGE_H


#include "debug.h"
#include "node.h"
#include "type.h"


typedef const enum {
  MessageTag_TOKEN = 0,
  MessageTag_NODE = 1,
  MessageTag_SIGNAL = 2,
} MessageTag; // Byte size in design


typedef struct {
  const NodeId dest; // Global node ID
  const Word value;
} Token;

Token Token_DUMMY = {
  .dest = NodeId_DUMMY,
  .value = Word_DUMMY,
};

Token Token_of(const NodeId dest, const Word value);


typedef const enum {
  Signal_END_OF_TOKENS = 0,
  Signal_END_OF_NODES = 1,
} Signal; // Int size in design (not VM's feature)


typedef const struct {
  const MessageTag tag;
  const union {
    Token token;
    Node node;
    Signal signal;
  }; // payload
} Message;

Message Message_ofToken(const Token token);
Message Message_ofNode(const Node node);
Message Message_ofSignal(const Signal signal);
Token Message_getToken(const Message message);
Node Message_getNode(const Message message);
Signal Message_getSignal(const Message message);


#endif
