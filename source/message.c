#include "message.h"


s9_token_t
s9_token_of(const s9_node_id_t dest, const s9_word_t value)
{
  return (s9_token_t){
    .dest = dest,
    .value = value,
  };
}


s9_node_update_t
s9_node_update_of(const s9_node_id_t node_id, const s9_node_t node)
{
  return (s9_node_update_t){
    .node_id = node_id,
    .node = node,
  };
}


s9_message_t
s9_message_of_token(const s9_token_t token)
{
  return (s9_message_t){
    .tag = S9_MESSAGE_TAG_TOKEN,
    .token = token,
  };
}


s9_message_t
s9_message_of_node_update(const s9_node_update_t node_update)
{
  return (s9_message_t){
    .tag = S9_MESSAGE_TAG_NODE_UPDATE,
    .node_update_ = node_update,
  };
}


s9_message_t
s9_message_of_signal(const s9_signal_t signal)
{
  return (s9_message_t){
    .tag = S9_MESSAGE_TAG_SIGNAL,
    .signal_ = signal,
  };
}


s9_token_t
s9_token_in_message(const s9_message_t message)
{
  assert(message.tag == S9_MESSAGE_TAG_TOKEN);
  return message.token_;
}


s9_node_update_t
s9_node_update_in_message(const s9_message_t message)
{
  assert(message.tag == S9_MESSAGE_TAG_NODE_UPDATE);
  return message.node_update_;
}


s9_signal_t
s9_signal_in_message(const s9_message_t message)
{
  assert(message.tag == S9_MESSAGE_TAG_SIGNAL);
  return message.signal_;
}
