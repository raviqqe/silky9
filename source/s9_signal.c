#include "s9_signal.h"


s9_error_t
s9_send_signal(const s9_protocol_t protocol, const s9_signal_t signal)
{
  assert(s9_network_is_initialized(protocol));

  for (s9_processor_id_t dest_processor_id = 0;
      dest_processor_id < protocol.num_of_processors;
      dest_processor_id += 1) {
    int mpi_error = s9_mpi_send(
        &signal, protocol.mpi_datatypes[S9_MPI_DATATYPE_INDEX_SIGNAL],
        dest_processor_id, S9_MPI_TAG_SIGNAL);
    if (mpi_error) {
      s9_mpi_debug_log("s9_mpi_send", mpi_error);
      return S9_ERROR_SEND_SIGNAL;
    }
  }

  return S9_OK;
}
