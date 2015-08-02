#ifndef S9_PROGRAM_H_
#define S9_PROGRAM_H_


#include "s9_error.h"
#include "s9_token.h"
#include "s9_network.h"
#include "s9_node.h"
#include "s9_type.h"


s9_error_t s9_load_nodes(const char node_file[*],
                         s9_memory_t * const memory,
                         const s9_protocol_t protocol);
s9_error_t s9_dispatch_tokens(const char token_file[*],
                              const s9_protocol_t protocol);


#endif // S9_PROGRAM_H_
