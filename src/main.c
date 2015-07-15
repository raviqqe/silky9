#include "comm.h"
#include "debug.h"
#include "err.h"
#include "message.h"
#include "node.h"
#include "type.h"
#include "program.h"
#include "inst.h"

#include <stdio.h>
#include <stdlib.h>


// functions

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


//static Err
//sendReal(NodeId dest, Real realNum)
//{
//  return sendWord(dest, Word_ofReal(realNum));
//}


static Err
executeInst(Node * const node, const Word operand)
{
  Err err = Err_OK;

  if (inst_getNumOfOperands(node->instId) == 2
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


static Err
processMessages(NodeMemory * const nodeMemory)
{
  assert(nodeMemory != NULL);

  Err err = Err_OK;

  while (true) {
    Message message;
    err = comm_receiveMessage(&message);
    if (err) {
      DEBUG_INFO("Failed to receive a message.");
      goto final;
    }

    switch (message.tag) {
    case MessageTag_TOKEN:
      {
        NodeId localNodeId = NodeId_DUMMY;

        err = comm_calcLocalNodeId(Message_getToken(message).dest,
                                   &localNodeId);
        if (err) {
          DEBUG_INFO("Failed to calculate a local node ID from a global "
                        "one of token's destination.");
          goto final;
        }

        Node *node = NULL;
        err = NodeMemory_getNodeOfId(nodeMemory, localNodeId, &node);
        if (err) {
          DEBUG_INFO("Failed to get an address of node from a node "
                        "memory.");
          goto final;
        }

        err = executeInst(node, Message_getToken(message).value);
        if (err) {
          DEBUG_INFO("Failed to execute an instruction.");
          goto final;
        }
      }
      break;
    case MessageTag_NODE_UPDATE:
      {
        NodeId localNodeId = NodeId_DUMMY;

        err = comm_calcLocalNodeId(Message_getNodeUpdate(message).nodeId,
                                   &localNodeId);
        if (err) {
          DEBUG_INFO("Failed to calculate a local node ID from a global"
                        "one of node update.");
          goto final;
        }

        err = NodeMemory_setNodeOfId(nodeMemory, localNodeId,
                                     Message_getNodeUpdate(message).node);
        if (err) {
          DEBUG_INFO("Failed to update a node in a node memory.");
          goto final;
        }
      }
      break;
    case MessageTag_SIGNAL:
      switch (Message_getSignal(message)) {
      case Signal_SHUTDOWN:
        goto final;
      default:
        DEBUG_INFO("Unknown signal detected. (signal: %d)",
                      Message_getSignal(message));
        err = Err_SIGNAL_UNKNOWN;
        goto final;
      }
      break;
    default:
      DEBUG_INFO("Unknown message tag detected. (message tag: %d)",
                    message.tag);
      err = Err_MESSAGE_TAG;
      goto final;
    } // switch (message.tag)
  } // while (true)

final:
  return err;
}



// main routine

int
main(int numOfArgs, char **args)
{
  Err err = Err_OK;

  char *programFileName = NULL;
  if (numOfArgs == 2) {
    programFileName = args[1];
  } else {
    USER_INFO("Usage: %s <program file>", args[0]);
    err = Err_COMMAND_ARGS;
    goto justExit;
  }

  err = comm_init();
  if (err) {
    DEBUG_INFO("Failed to initialize communication environment of "
                  "processors.");
    goto justExit;
  }

  bool answer;
  err = comm_amIMaster(&answer);
  if (err) {
    DEBUG_INFO("Failed to check if I am the master processor.");
    goto finalComm;
  }
  if (answer) {
    err = program_loadProgram(programFileName); // Signal_MEM_SIZE?
    if (err) {
      DEBUG_INFO("Failed to load a program to processors.");
      goto finalComm;
    }
  }

  NodeMemory nodeMemory = NodeMemory_DUMMY;
  err = NodeMemory_init(&nodeMemory);
  if (err) {
    DEBUG_INFO("Failed to initialize a node memory.");
    goto finalComm;
  }

  err = processMessages(&nodeMemory);
  if (err) {
    DEBUG_INFO("Failed to processs some messages.");
    goto finalNodeMemory;
  }

  Err errOnFinal = Err_OK;
finalNodeMemory:
  errOnFinal = NodeMemory_final(&nodeMemory);
  if (errOnFinal) {
    DEBUG_INFO("Failed to finalize a node memory. (error code: %d)",
                  errOnFinal);
    goto finalComm;
  }

finalComm:
  if (err) {
    errOnFinal = comm_sendMessage(Message_ofSignal(Signal_SHUTDOWN));
    if (errOnFinal) {
      DEBUG_INFO("Failed to send shutdown signal to processors. "
                    "(error code: %d)", errOnFinal);
    }
  }

  errOnFinal = comm_final();
  if (errOnFinal) {
    DEBUG_INFO("Failed to finalize communication environment of "
                  "processors. (error code: %d)", errOnFinal);
    goto justExit;
  }

justExit:
  if (err) {
    USER_INFO("Failed to run Silky 9 because of error of the code, "
              "%d.", err);
    return EXIT_FAILURE;
  } else {
    return EXIT_SUCCESS;
  }
}
