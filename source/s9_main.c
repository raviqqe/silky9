#include <stdio.h>
#include <stdlib.h>

#include "s9_error.h"
#include "s9_instruction.h"
#include "s9_log.h"
#include "s9_network.h"
#include "s9_node.h"
#include "s9_memory.h"
#include "s9_program.h"
#include "s9_receive.h"
#include "s9_signal.h"
#include "s9_token.h"
#include "s9_type.h"


static s9_error_t
s9_process_tokens(s9_memory_t * const memory, const s9_protocol_t protocol)
{
  s9_debug_log(S9_LOG_LEVEL_VERBOSE, "starting to process messages...");
  assert(memory != NULL);

  while (true) {
    s9_receive_buffer_t buffer = S9_DUMMY_RECEIVE_BUFFER;
    s9_error_t error = s9_receive_token_or_signal(&buffer);
    if (error) {
      s9_log(S9_LOG_LEVEL_ERROR, "failed to receive a token or signal.");
      return error;
    }

    if (s9_token_is_received(buffer)) {
      s9_debug_log(S9_LOG_LEVEL_VERBOSE, "a token is received.");

      const s9_token_t token = s9_token_in_receive_buffer(buffer);

      s9_node_t *node = NULL;
      s9_error_t error = s9_get_node_in_memory(memory, token.dest, &node);
      if (error) {
        s9_debug_log(S9_LOG_LEVEL_ERROR,
                     "failed to get an address of node from a node memory.");
        return error;
      }

      error = s9_execute_instruction(node, token.value);
      if (error) {
        s9_debug_log(S9_LOG_LEVEL_ERROR, "failed to execute an instruction.");
        return error;
      }
    } else if (s9_signal_is_received(buffer)) {
      s9_debug_log(S9_LOG_LEVEL_VERBOSE, "a signal is received.");

      const s9_signal_t signal = s9_signal_in_receive_buffer(buffer);

      switch (signal) {
      case S9_SIGNAL_HALT:
        return S9_OK;
      default:
        s9_debug_log(S9_LOG_LEVEL_ERROR,
                     "unknown signal detected. (signal: %d)", signal);
        return S9_ERROR_SIGNAL_UNKNOWN;
      }
    } else {
      s9_debug_log(S9_LOG_LEVEL_ERROR, "data of unknown type is received.");
      return S9_ERROR_UNKNOWN_DATA;
    }
  } // while (true)

  return S9_OK;
}



// main routine

int
main(const int argc, const char * const * const argv)
{
  s9_error_t error = S9_OK;
  const char *node_file;
  const char *token_file;

  if (argc == 3) {
    node_file = argv[1];
    token_file = argv[2];
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

  s9_memory_t memory = S9_DUMMY_MEMORY;
  error = s9_load_nodes(node_file, &memory, protocol);
  if (error) {
    s9_debug_log(S9_LOG_LEVEL_ERROR,
                 "failed to load nodes to a node memory.");
    goto finalize_memory;
  }

  if (protocol.processor_id == 0) {
    error = s9_dispatch_tokens(token_file, protocol);
    if (error) {
      s9_debug_log(S9_LOG_LEVEL_ERROR,
                   "failed to load nodes to a node memory.");
      goto finalize_memory;
    }
  }

  s9_debug_log(S9_LOG_LEVEL_VERBOSE, "a node memory initialized!");

  error = s9_process_tokens(&memory, protocol);
  if (error) {
    s9_debug_log(S9_LOG_LEVEL_ERROR, "failed to processs some messages.");
    goto finalize_memory;
  }

  s9_error_t error_on_finalization = S9_OK;
finalize_memory:
  error_on_finalization = s9_finalize_memory(&memory);
  if (error_on_finalization) {
    s9_debug_log(S9_LOG_LEVEL_ERROR,
                 "failed to finalize a node memory. "
                 "(error code: %d)", error_on_finalization);
    goto finalize_network;
  }

finalize_network:
  if (error) {
    error_on_finalization = s9_send_signal(S9_SIGNAL_HALT);
    if (error_on_finalization) {
      s9_debug_log(S9_LOG_LEVEL_ERROR,
                   "failed to send shutdown signals to processors. "
                   "(error code: %d)", error_on_finalization);
    } else {
      s9_debug_log(S9_LOG_LEVEL_VERBOSE,
                   "shutdown signal sent to all processors.");
    }
  }

  error_on_finalization = s9_finalize_network(protocol);
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
