#ifndef TOKEN_H_
#define TOKEN_H_


#include <assert.h>

#include "node.h"
#include "type.h"


typedef struct {
  const s9_node_id_t dest; // global node id
  const s9_word_t value;
} s9_token_t;

#define S9_DUMMY_TOKEN \
  (s9_token_t){.dest = S9_DUMMY_NODE_ID, .value = S9_DUMMY_WORD}

s9_token_t s9_token_of(const s9_node_id_t dest, const s9_word_t value);

enum {
  S9_SIGNAL_NULL,
  S9_SIGNAL_SHUTDOWN,
};
typedef s9_byte_t s9_signal_t


s9_error_t s9_send_token(const s9_network_info_t * const network_info,
                         const s9_token_t token);
s9_error_t s9_receive_token(const s9_network_info_t * const network_info,
                            s9_token_t * const token);


#endif // TOKEN_H_
