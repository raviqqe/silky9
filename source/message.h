#ifndef MESSAGE_H_
#define MESSAGE_H_


#include <assert.h>

#include "node.h"
#include "type.h"
#include "word.h"


// message tag

enum {
  S9_MESSAGE_TAG_TOKEN,
  S9_MESSAGE_TAG_NODE_UPDATE,
  S9_MESSAGE_TAG_SIGNAL,
};
typedef s9_int_t s9_message_tag_t;


// token

typedef struct {
  const s9_node_id_t dest; // global node id
  const s9_word_t value;
} s9_token_t;

#define S9_DUMMY_TOKEN \
  (s9_token_t){.dest = S9_DUMMY_NODE_ID, .value = S9_DUMMY_WORD}

s9_token_t s9_token_of(const s9_node_id_t dest, const s9_word_t value);


// node update

typedef struct {
  const s9_node_id_t node_id; // global node id
  const s9_node_t node;
} s9_node_update_t;

#define S9_NODE_UPDATE_DUMMY \
  (s9_node_update_t){.nodeId = NodeId_DUMMY, .node = Node_DUMMY}

s9_node_update_t s9_node_update_of(const s9_node_id_t nodeId,
                                   const s9_node_t node);


// signal

enum {
  S9_SIGNAL_SHUTDOWN,
};
typedef s9_int_t s9_signal_t


// message

typedef struct {
  const s9_message_tag_t tag;
  const union {
    s9_token_t token_;
    s9_node_update_t node_update_;
    s9_signal_t signal_;
  }; // payload
} s9_message_t;


s9_message_t s9_message_of_token(const s9_token_t token);
s9_message_t s9_message_of_node_update(const s9_node_update_t node_update);
s9_message_t s9_message_of_signal(const s9_signal_t signal);
s9_token_t s9_token_in_message(const s9_message_t message);
s9_node_update_t s9_node_update_in_message(const s9_message_t message);
s9_signal_t s9_signal_in_message(const s9_message_t message);


#endif // MESSAGE_H_
