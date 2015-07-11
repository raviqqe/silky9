#ifndef _COMM_H
#define _COMM_H


#include "debug.h"
#include "node.h"
#include "type.h"
#include "message.h"


// functions

Err comm_init();
Err comm_final();
Err comm_sendMessage(const Message message);
Err comm_receiveMessage(Message * const message);
bool comm_amIMaster();
NodeId comm_calcLocalNodeId(const NodeId globalNodeId);


#endif
