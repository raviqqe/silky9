#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "error.h"
#include "instruction.h"
#include "message.h"
#include "network.h"
#include "node.h"
#include "type.h"
#include "program.h"


// functions

static s9_error_t
s9_process_messages(s9_memory_t * const memory)
{
  s9_log(S9_LOG_LEVEL_DEBUG, "starting to process messages...");
  assert(memory != NULL);

  s9_error_t error = S9_OK;

  while (true) {
    s9_message_t message = S9_DUMMY_MESSAGE;
    error = s9_receive_message(&message);
    if (error) {
      s9_log(S9_LOG_LEVEL_DEBUG, "failed to receive a message.");
      goto final;
    }

    switch (message.tag) {
    case S9_MESSAGE_TAG_TOKEN:
      s9_log(S9_LOG_LEVEL_DEBUG, "message of S9_MESSAGE_TAG_TOKEN received.");
      {
        s9_node_id local_node_id = S9_DUMMY_NODE_ID;

        error = s9_calc_local_node_id(s9_token_in_message(message).dest,
                                      &local_node_id);
        if (error) {
          s9_log(S9_LOG_LEVEL_DEBUG,
                 "Failed to calculate a local node ID from a global one of "
                 "token's destination.");
          goto final;
        }

        s9_node_t *node = NULL;
        error = s9_memory_t_gets9_node_tOfId(memory, locals9_node_tId, &node);
        if (error) {
          s9_log(S9_LOG_LEVEL_DEBUG, "Failed to get an address of node from a node "
                        "memory.");
          goto final;
        }

        error = inst_executeInst(node, Message_getToken(message).value);
        if (error) {
          s9_log(S9_LOG_LEVEL_DEBUG, "Failed to execute an instruction.");
          goto final;
        }
      }
      break;
    case S9_MESSAGE_TAG_NODE_UPDATE:
      s9_log(S9_LOG_LEVEL_DEBUG, "Message of S9_MESSAGE_TAG_NODE_UPDATE received.");
      {
        s9_node_tId locals9_node_tId = s9_node_tId_DUMMY;

        error = comm_calcLocals9_node_tId(Message_gets9_node_tUpdate(message).nodeId,
                                   &locals9_node_tId);
        if (error) {
          s9_log(S9_LOG_LEVEL_DEBUG, "Failed to calculate a local node ID from a global"
                        "one of node update.");
          goto final;
        }

        error = s9_memory_t_sets9_node_tOfId(memory, locals9_node_tId,
                                     Message_gets9_node_tUpdate(message).node);
        if (error) {
          s9_log(S9_LOG_LEVEL_DEBUG, "Failed to update a node in a node memory.");
          goto final;
        }
      }
      break;
    case S9_MESSAGE_TAG_SIGNAL:
      s9_log(S9_LOG_LEVEL_DEBUG, "Message of S9_MESSAGE_TAG_SIGNAL received.");
      switch (Message_getSignal(message)) {
      case Signal_SHUTDOWN:
        goto final;
      default:
        s9_log(S9_LOG_LEVEL_DEBUG, "Unknown signal detected. (signal: %d)",
                      Message_getSignal(message));
        error = s9_error_t_SIGNAL_UNKNOWN;
        goto final;
      }
      break;
    default:
      s9_log(S9_LOG_LEVEL_DEBUG, "Unknown message tag detected. (message tag: %d)",
                    message.tag);
      error = s9_error_t_MESSAGE_TAG;
      goto final;
    } // switch (message.tag)
  } // while (true)

final:
  return err;
}



// main routine

int
main(const int argc, const char * const * const argv)
{
  s9_error_t error = S9_OK;

  char *program_filename = NULL;
  if (argc == 2) {
    program_filename = argv[1];
  } else {
    s9_log(S9_LOG_LEVEL_NOTICE, "Usage: %s <program file>", argv[0]);
    error = S9_ERROR_COMMAND_ARGS;
    goto just_exit;
  }

  error = s9_initialize_network();
  if (error) {
    s9_log(S9_LOG_LEVEL_DEBUG, "Failed to initialize communication environment of "
                  "processors.");
    goto just_exit;
  }

  s9_log(S9_LOG_LEVEL_DEBUG, "A processor launched!");

  if (s9_is_master_processor()) {
    error = s9_load_program(programFileName);
    if (error) {
      s9_log(S9_LOG_LEVEL_DEBUG, "Failed to load a program to processors.");
      goto finalize_network;
    }
  }

  s9_memory_t memory = S9_DUMMY_MEMORY;
  error = s9_initilalize_memory(&memory);
  if (error) {
    s9_log(S9_LOG_LEVEL_DEBUG, "Failed to initialize a node memory.");
    goto finalize_network;
  }
  s9_log(S9_LOG_LEVEL_DEBUG, "s9_memory_t initialized!");

  error = s9_process_messages(&memory);
  if (error) {
    s9_log(S9_LOG_LEVEL_DEBUG, "Failed to processs some messages.");
    goto finalize_memory;
  }

  s9_error_t error_on_finalization = S9_OK;
finalize_memory:
  error_on_finalization = s9_finalize_memory(&memory);
  if (error_on_finalization) {
    s9_log(S9_LOG_LEVEL_DEBUG,
           "Failed to finalize a memory. "
           "(error code: %d)", error_on_finalization);
    goto finalize_network;
  }

finalize_network:
  if (error) {
    error_on_finalization = s9_send_message(
        s9_message_of_signal(S9_SIGNAL_SHUTDOWN));
    if (error_on_finalization) {
      s9_log(S9_LOG_LEVEL_DEBUG,
             "failed to send shutdown signals to processors. "
             "(error code: %d)", error_on_finalization);
    } else {
      s9_log(S9_LOG_LEVEL_DEBUG, "shutdown signal sent to all processors.");
    }
  }

  error_on_finalizatoin = s9_finalize_network();
  if (error_on_finalization) {
    s9_log(S9_LOG_LEVEL_DEBUG,
           "failed to finalize the processor network. "
           "(error code: %d)", error_on_finalization);
    goto just_exit;
  }

just_exit:
  if (error) {
    s9_log(S9_LOG_LEVEL_NOTICE,
           "failed to run silky 9 because of error of the code, %d.", error);
    return EXIT_FAILURE;
  } else {
    return EXIT_SUCCESS;
  }
}
