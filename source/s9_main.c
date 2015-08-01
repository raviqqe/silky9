#include <stdio.h>
#include <stdlib.h>

#include "s9_error.h"
#include "s9_instruction.h"
#include "s9_log.h"
#include "s9_node.h"
#include "s9_program.h"
#include "s9_protocol.h"
#include "s9_receive.h"
#include "s9_signal.h"
#include "s9_token.h"
#include "s9_type.h"


static s9_error_t
s9_process_tokens(s9_node_memory_t * const node_memory)
{
  s9_debug_log(S9_LOG_LEVEL_VERBOSE, "starting to process messages...");
  assert(node_memory != NULL);

  s9_error_t error = S9_OK;
  s9_receive_buffer_t buffer = S9_DUMMY_RECEIVE_BUFFER;

  while (true) {
    error = s9_receive_token_or_signal(&buffer);
    if (error) {
      s9_log(S9_LOG_LEVEL_ERROR, "failed to receive a token or signal.");
      return error;
    }

    if (s9_token_is_received(buffer)) {
      s9_debug_log(S9_LOG_LEVEL_VERBOSE, "a token is received.");

      s9_node_t *node = NULL;
      error = s9_memory_t_gets9_node_tOfId(node_memory, locals9_node_tId, &node);
      if (error) {
        s9_log(S9_LOG_LEVEL_DEBUG, "Failed to get an address of node from a node "
                      "memory.");
        return error;
      }

      error = inst_executeInst(node, Message_getToken(message).value);
      if (error) {
        s9_log(S9_LOG_LEVEL_DEBUG, "Failed to execute an instruction.");
        return error;
      }
    } else if (s9_signal_is_received(buffer)) {
      s9_debug_log(S9_LOG_LEVEL_VERBOSE, "a signal is received.");
      switch (Message_getSignal(message)) {
      case Signal_SHUTDOWN:
        goto final;
      default:
        s9_log(S9_LOG_LEVEL_DEBUG, "Unknown signal detected. (signal: %d)",
                      Message_getSignal(message));
        return S9_ERROR_SIGNAL_UNKNOWN;
      }
    } else


  } // while (true)

  return S9_OK;
}



// main routine

int
main(const int argc, const char * const * const argv)
{
  s9_error_t error = S9_OK;

  char *program_filename = NULL;
  if (argc == 3) {
    node_filename = argv[1];
    token_filename = argv[2];
  } else {
    s9_log(S9_LOG_LEVEL_NOTICE, "usage: %s <node file> <token file>", argv[0]);
    error = S9_ERROR_COMMAND_ARGS;
    goto just_exit;
  }

  s9_protocol_t protocol = S9_DUMMY_PROTOCOL;
  error = s9_initialize_network(&protocol);
  if (error) {
    s9_debug_log(S9_LOG_LEVEL_ERROR,
                 "failed to initialize the processor network.");
    goto just_exit;
  }

  s9_debug_log(S9_LOG_LEVEL_VERBOSE,
               "the processor #%lld launched!", protocol.processor_id);

  s9_node_memory_t node_memory = S9_DUMMY_NODE_MEMORY;
  error = s9_initilalize_node_memory(&node_memory);
  if (error) {
    s9_debug_log(S9_LOG_LEVEL_ERROR, "failed to initialize a node memory.");
    goto finalize_network;
  }

  error = s9_load_nodes(node_file, &node_memory, protocol);
  if (error) {
    s9_debug_log(S9_LOG_LEVEL_ERROR,
                 "failed to load nodes to a node memory.");
    goto finalize_node_memory;
  }

  s9_debug_log(S9_LOG_LEVEL_VERBOSE, "a node memory initialized!");

  error = s9_process_messages(&node_memory);
  if (error) {
    s9_log(S9_LOG_LEVEL_DEBUG, "Failed to processs some messages.");
    goto finalize_node_memory;
  }

  s9_error_t error_on_finalization = S9_OK;
finalize_node_memory:
  error_on_finalization = s9_finalize_node_memory(&node_memory);
  if (error_on_finalization) {
    s9_log(S9_LOG_LEVEL_DEBUG,
           "failed to finalize a node memory. "
           "(error code: %d)", error_on_finalization);
    goto finalize_network;
  }

finalize_network:
  if (error) {
    error_on_finalization = s9_send_message(
        s9_message_of_signal(S9_SIGNAL_SHUTDOWN));
    if (error_on_finalization) {
      s9_debug_log(S9_LOG_LEVEL_ERROR,
                   "failed to send shutdown signals to processors. "
                   "(error code: %d)", error_on_finalization);
    } else {
      s9_debug_log(S9_LOG_LEVEL_VERBOSE,
                   "shutdown signal sent to all processors.");
    }
  }

  error_on_finalization = s9_finalize_network();
  if (error_on_finalization) {
    s9_debug_log(S9_LOG_LEVEL_ERROR,
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
