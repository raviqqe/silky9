#include "s9_receive.h"


s9_error_t
s9_receive_token_or_signal(const s9_protocol_t protocol,
                           s9_receive_buffer_t * const receive_buffer)
{
  assert(s9_network_is_initialized(protocol));

  MPI_Status status;
  int mpi_error = s9_mpi_probe(&status);
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Probe", mpi_error);
    return S9_ERROR_RECEIVE;
  }

  receive_buffer->mpi_tag_ = status.MPI_TAG;

  switch (status.MPI_TAG) {
  case S9_MPI_TAG_TOKEN:
    mpi_error = s9_mpi_recv(
        &receive_buffer->token,
        protocol->mpi_datatypes[S9_MPI_DATATYPE_INDEX_TOKEN]);
    if (mpi_error) {
      s9_mpi_debug_log("MPI_Recv", mpi_error);
      return S9_ERROR_RECEIVE;
    }
    break;
  case S9_MPI_TAG_SIGNAL:
    mpi_error = s9_mpi_recv(
        &receive_buffer->signal,
        protocol->mpi_datatypes[S9_MPI_DATATYPE_INDEX_SIGNAL]);
    if (mpi_error) {
      s9_mpi_debug_log("MPI_Recv", mpi_error);
      return S9_ERROR_RECEIVE;
    }
    break;
  default:
    s9_debug_log(S9_DEBUG_LEVEL_ERROR,
           "unknown tag detected during receiving a token or signal"
           "(tag = %d)", status.MPI_TAG);
    return S9_ERROR_RECEIVE;
  }

  return S9_OK;
}


s9_token_t
s9_token_in_receive_buffer(const s9_receive_buffer_t receive_buffer)
{
  assert(receive_buffer.mpi_tag_ == S9_MPI_TAG_TOKEN);
  return receive_buffer.token_;
}


s9_signal_t
s9_signal_in_receive_buffer(const s9_receive_buffer_t receive_buffer)
{
  assert(receive_buffer.mpi_tag_ == S9_MPI_TAG_SIGNAL);
  return receive_buffer.signal_;
}


bool
s9_token_is_received(const s9_receive_buffer_t receive_buffer)
{
  return receive_buffer.mpi_tag_ == S9_MPI_TAG_TOKEN;
}


bool
s9_signal_is_received(const s9_receive_buffer_t receive_buffer)
{
  return receive_buffer.mpi_tag_ == S9_MPI_TAG_SIGNAL;
}
