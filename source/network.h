#ifndef NETWORK_H_
#define NETWORK_H_


#include "Error.h"
#include "Log.h"
#include "Message.h"
#include "Node.h"
#include "Type.h"
#include "Word.h"

#include <mpi.h>
#include <stdbool.h>


void Network_SendMessage(const Message message);
Message Network_ReceiveMessage();
NodeId Network_CalcLocalNodeId(const NodeId global_node_id);
bool Network_AmIMasterProcessor();
void Exit(const Error error);


#endif
