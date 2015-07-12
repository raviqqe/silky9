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
#include <stdint.h>
#include <stderr.h>
#include <stdbool.h>


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


static Err
sendReal(NodeId dest, Real realNum)
{
  return sendWord(dest, Word_ofReal(realNum));
}


static Err
executeInst(Node * const node, const Word operand)
{
  Err err = Err_OK;

  if (inst_getNumOfOperands(node->instId) == 2
      && !node->inst.operandPresentBit) {
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
      DEBUG_MESSAGE("Unknown instruction detected. (instruction id: %x)",
                    node->instId);
      return Err_INST_UNKNOWN;
    }
  }

  return err;
}


static Err
processMessages(Node * const nodes)
{
  Err err = Err_OK;

  while (true) {
    Message message;
    err = comm_receiveMessage(&message);
    if (err) {
      DEBUG_MESSAGE("Failed to receive a message.");
      goto final;
    }

    switch (message.tag) {
    case MessageTag_TOKEN:
      {
        NodeId localNodeId = NodeId_DUMMY;

        err = comm_calcLocalNodeId(Message_getToken(message).dest,
                                   &localNodeId);
        if (err) {
          DEBUG_MESSAGE("Failed to calculate a local node ID from a global "
                        "one.");
          goto final;
        }

        err = executeInst(
            &nodes[localNodeId],
            Message_getToken(message).value);
        if (err) {
          DEBUG_MESSAGE("Failed to execute an instruction.");
          goto final;
        }
      }
      break;
    case MessageTag_NODE:
      {
        NodeId localNodeId = NodeId_DUMMY;

        err = comm_calcLocalNodeId(Message_getNodeUpdate(message).nodeId,
                                   localNodeId);
        if (err) {
          DEBUG_MESSAGE("Failed to calculate a local node ID from a global "
                        "one.");
          goto final;
        }

        nodes[localNodeId] = Message_getNodeUpdate(message).node;
      }
      break;
    case MessageTag_SIGNAL:
      switch (Message_getSignal(message)) {
      case Signal_SHUTDOWN:
        goto final;
      default:
        DEBUG_MESSAGE("Unknown signal detected. (signal: %d)",
                      Message_getSignal(message));
        err = Err_SIGNAL_UNKNOWN;
        goto final;
      }
      break;
    default:
      DEBUG_MESSAGE("Unknown message tag detected. (message tag: %d)",
                    message.tag);
      err = Err_MESSAGE_TAG;
      goto final;
    } // switch (message.tag)
  } // while (true)

final:
  freeNodeMemory(nodes);

  return err;
}


// main routine

int
main(int numOfArgs, char **args)
{
  Err err = comm_init();
  if (err) {
    DEBUG_MESSAGE("Failed to initialize communication environment of "
                  "processors.");
    goto justExit;
  }

  //
  // process command line options and set programFileName
  //
  char *programFileName = args[numOfArgs - 1];
  //
  // not implemented yet
  //

  bool answer;
  err = comm_amIMaster(&answer);
  if (err) {
    DEBUG_MESSAGE("Failed to check if I am the master processor.");
    goto final;
  }
  if (answer) {
    DEBUG_MESSAGE("I am the master processor!");
    Int numOfNodes = 0; // dummy value
    err = program_loadProgram(programFilename); // Signal_MEM_SIZE?
    if (err) {
      DEBUG_MESSAGE("Failed to load a program to processors.");
      goto final;
    }
  }

  Node *nodes = NULL;
  err = allocNodeMemory(&nodes);
  if (err) {
    DEBUG_MESSAGE("Failed to allocate initial memory for nodes.");
    goto final;
  }

  err = processMessages(nodes); // resposible to free up node memory
  if (err) {
    DEBUG_MESSAGE("Failed to processs messages.");
    goto final;
  }

final:
  err = comm_final();
  if (err) {
    DEBUG_MESSAGE("Failed to finalize communication environment of "
                  "processors.");
    goto exit;
  }

justExit:
  if (err) {
    DEBUG_MESSAGE("Failed to run Silky 9 because of the error of the code, "
                  "%d.", err);
    return EXIT_FAILURE;
  } else {
    return EXIT_SUCCESS;
  }
}
