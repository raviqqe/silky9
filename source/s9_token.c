#include "s9_token.h"


s9_token_t
s9_token_of(const s9_node_id_t dest, const s9_word_t value)
{
  return (s9_token_t){
    .dest = dest,
    .value = value,
  };
}


s9_error_t
s9_send_token(const s9_protocol_t protocol, const s9_token_t token)
{
  assert(s9_network_is_initialized(protocol));

  int mpi_error = s9_mpi_send(
      &token, protocol.mpi_datatypes[S9_MPI_DATATYPE_INDEX_TOKEN],
      s9_calc_dest_processor_id(protocol, token.dest), S9_MPI_TAG_TOKEN);
  if (mpi_error) {
    s9_mpi_debug_log("s9_mpi_send", mpi_error);
    return S9_ERROR_SEND_TOKEN;
  }

  return S9_OK;
}
