#include "signal.h"


s9_error_t
s9_send_signal(const s9_network_info_t network_info, const s9_signal_t signal)
{
  assert(s9_network_is_initialized(network_info));

  for (s9_processor_id_t dest_processor_id = 0;
      dest_processor_id < network_info.num_of_processors;
      dest_processor_id += 1) {
    int mpi_error = MPI_Send(
        &signal, 1, network_info.mpi_datatypes[S9_MPI_DATATYPE_SIGNAL],
        dest_processor_id, S9_MPI_TAG_SIGNAL, MPI_COMM_WORLD);
    if (mpi_error) {
      s9_mpi_debug_log("MPI_Send", mpi_error);
      return S9_ERROR_SEND_SIGNAL;
    }
  }

  return S9_OK;
}
