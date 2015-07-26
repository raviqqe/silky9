#include <assert.h>
#include <mpi.h>

#include "network_info.h"
#include "signal.h"
#include "token.h"


typedef struct {
  s9_mpi_tag_t mpi_tag_;
  union {
    s9_token_t token_;
    s9_signal_t signal_;
  };
} s9_receive_buffer_t;


s9_error_t s9_receive_token_or_signal(
    const s9_network_info_t network_info,
    s9_receive_buffer_t * const receive_buffer);
bool s9_token_is_received(const s9_receive_buffer_t receive_buffer);
bool s9_signal_is_received(const s9_receive_buffer_t receive_buffer);
s9_token_t s9_token_in_receive_buffer(
    const s9_receive_buffer_t receive_buffer);
s9_signal_t s9_signal_in_receive_buffer(
    const s9_receive_buffer_t receive_buffer);
