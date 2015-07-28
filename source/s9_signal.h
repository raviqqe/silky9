#ifndef S9_SIGNAL_H_
#define S9_SIGNAL_H_


#include <assert.h>

#include "s9_network_info.h"
#include "s9_type.h"


enum {
  S9_SIGNAL_SHUTDOWN,
};
typedef s9_byte_t s9_signal_t
#define S9_DUMMY_SIGNAL S9_DUMMY_BYTE


s9_error_t s9_send_signal(const s9_network_info_t network_info,
                          const s9_signal_t signal);


#endif // S9_SIGNAL_H_
