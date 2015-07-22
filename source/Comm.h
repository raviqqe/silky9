#ifndef _COMM_H
#define _COMM_H


#include "debug.h"
#include "err.h"
#include "message.h"
#include "node.h"
#include "type.h"
#include <mpi.h>
#include <stdbool.h>


// functions

Err comm_init();
Err comm_final();
Err comm_sendMessage(const Message message);
Err comm_receiveMessage(Message * const message);
Err comm_amIMaster(bool *answer);
Err comm_calcLocalNodeId(const NodeId globalNodeId,
                         NodeId * const localNodeId);


#endif
