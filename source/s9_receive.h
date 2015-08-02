#ifndef S9_RECEIVE_H_
#define S9_RECEIVE_H_


#include <assert.h>
#include <mpi.h>

#include "s9_network.h"
#include "s9_signal.h"
#include "s9_token.h"


typedef struct {
  s9_mpi_tag_t mpi_tag_;
  union {
    s9_token_t token_;
    s9_signal_t signal_;
  };
} s9_receive_buffer_t;


s9_error_t s9_receive_token_or_signal(
    const s9_protocol_t protocol,
    s9_receive_buffer_t * const receive_buffer);
s9_token_t s9_token_in_receive_buffer(
    const s9_receive_buffer_t receive_buffer);
s9_signal_t s9_signal_in_receive_buffer(
    const s9_receive_buffer_t receive_buffer);
bool s9_token_is_received(const s9_receive_buffer_t receive_buffer);
bool s9_signal_is_received(const s9_receive_buffer_t receive_buffer);


#endif // S9_RECEIVE_H_
