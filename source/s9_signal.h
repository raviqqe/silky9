#ifndef S9_SIGNAL_H_
#define S9_SIGNAL_H_


#include <assert.h>

#include "s9_network.h"
#include "s9_type.h"


enum {
  S9_SIGNAL_HALT,
};
typedef s9_int_t s9_signal_t;
#define S9_DUMMY_SIGNAL S9_DUMMY_INT


s9_error_t s9_send_signal(const s9_protocol_t protocol,
                          const s9_signal_t signal);


#endif // S9_SIGNAL_H_
