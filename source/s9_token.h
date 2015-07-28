#ifndef S9_TOKEN_H_
#define S9_TOKEN_H_


#include <assert.h>

#include "s9_log.h"
#include "s9_node.h"
#include "s9_type.h"


typedef struct {
  const s9_node_id_t dest; // global node id
  const s9_word_t value;
} s9_token_t;

#define S9_DUMMY_TOKEN \
  (s9_token_t){.dest = S9_DUMMY_NODE_ID, .value = S9_DUMMY_WORD}


s9_token_t s9_token_of(const s9_node_id_t dest, const s9_word_t value);
s9_error_t s9_send_token(const s9_network_info_t network_info,
                         const s9_token_t token);


#endif // S9_TOKEN_H_
