#include "token.h"


s9_token_t
s9_token_of(const s9_node_id_t dest, const s9_word_t value)
{
  return (s9_token_t){
    .dest = dest,
    .value = value,
  };
}


s9_error_t
s9_send_token(const s9_network_info_t network_info, const s9_token_t token)
{
  assert(s9_network_is_initialized(network_info));

  int mpi_error = MPI_Send(&token, 1,
                           network_info.mpi_datatypes[S9_MPI_DATATYPE_TOKEN],
                           s9_calc_dest_processor_id(network_info, token.dest),
                           S9_MPI_TAG_TOKEN, MPI_COMM_WORLD);
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Send", mpi_error);
    return S9_ERROR_SEND_TOKEN;
  }

  return S9_OK;
}


