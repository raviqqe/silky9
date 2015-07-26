#include "receive.h"


static int
s9_wrapped_MPI_Recv(void *buffer, MPI_Datatype datatype)
{
  return MPI_Recv(buffer, 1, datatype, MPI_ANY_SOURCE, MPI_ANY_TAG,
                  MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}


s9_error_t
s9_receive_token_or_signal(const s9_network_info_t network_info,
                           s9_receive_buffer_t * const receive_buffer)
{
  assert(s9_network_is_initialized(network_info));

  MPI_Status status;
  int mpi_error = MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
                            &status);
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Probe", mpi_error);
    return S9_ERROR_RECEIVE;
  }

  receive_buffer->datatype_ = status.MPI_TAG;

  switch (status.MPI_TAG) {
  case S9_MPI_DATATYPE_TOKEN:
    mpi_error = s9_wrapped_MPI_Recv(
        &receive_buffer->token,
        network_info->mpi_datatypes[S9_MPI_DATATYPE_TOKEN]);
    if (mpi_error) {
      s9_mpi_debug_log("MPI_Recv", mpi_error);
      return S9_ERROR_RECEIVE;
    }
    break;
  case S9_MPI_DATATYPE_SIGNAL:
    mpi_error = s9_wrapped_MPI_Recv(
        &receive_buffer->signal,
        network_info->mpi_datatypes[S9_MPI_DATATYPE_SIGNAL]);
    if (mpi_error) {
      s9_mpi_debug_log("MPI_Recv", mpi_error);
      return S9_ERROR_RECEIVE;
    }
    break;
  default:
    s9_log(S9_LOG_LEVEL_WARNING,
           "unknown tag detected during receiving a token or signal"
           "(tag = %d)", status.MPI_TAG);
    break;
  }

  return S9_OK;
}


s9_token_t
s9_token_in_receive_buffer(const s9_receive_buffer_t receive_buffer)
{
  assert(receive_buffer.datatype_ == S9_MPI_DATATYPE_TOKEN);
  return receive_buffer.token_;
}


s9_signal_t
s9_signal_in_receive_buffer(const s9_receive_buffer_t receive_buffer)
{
  assert(receive_buffer.datatype_ == S9_MPI_DATATYPE_SIGNAL);
  return receive_buffer.signal_;
}
