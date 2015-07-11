#include "message.h"


Message
Message_ofToken(const Token token)
{
  return (Message){
        .tag = MessageTag_TOKEN,
        .token = token,
      };
}


Message
Message_ofNodeUpdate(const NodeUpdate nodeUpdate)
{
  return (Message){
        .tag = MessageTag_NODE_UPDATE,
        .nodeUpdate = nodeUpdate,
      };
}


Message
Message_ofSignal(const Signal signal)
{
  return (Message){
        .tag = MessageTag_SIGNAL,
        .signal = signal,
      };
}


// These accessors of Message type are used by users who already know tags of
// messages. So, they can return their values theirselves and just use assert()
// to check programming errors.

Token
Message_getToken(const Message message)
{
  assert(message.tag == MessageTag_TOKEN);
  return message.token;
}


NodeUpdate
Message_getNodeUpdate(const Message message)
{
  assert(message.tag == MessageTag_NODE_UPDATE);
  return message.nodeUpdate;
}


Signal
Message_getSignal(const Message message)
{
  assert(message.tag == MessageTag_SIGNAL);
  return message.signal;
}
