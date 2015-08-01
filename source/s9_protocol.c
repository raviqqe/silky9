#include "s9_protocol.h"


s9_error_t
s9_initialize_network(s9_protocol_t * protocol)
{
  assert(!s9_network_is_initialized(*protocol));

  int mpi_error = MPI_Init(NULL, NULL);
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Init", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

#ifndef S9_CONFIG_MPI_ERRORS_ARE_FATAL
  mpi_error = MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Comm_set_errhandler", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }
#endif

  // get rank

  mpi_error = MPI_Comm_rank(MPI_COMM_WORLD, &protocol->processor_id);
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Comm_rank", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

  // get num of processors

  mpi_error = MPI_Comm_size(MPI_COMM_WORLD, &protocol->num_of_processsors);
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Comm_size", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

  // type declarations

  //// byte

  mpi_error = MPI_Type_dup(
      MPI_UINT8_T,
      &protocol->mpi_datatypes[S9_MPI_DATATYPE_INDEX_BYTE]);
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Type_dup", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

  mpi_error = MPI_Type_commit(
      &protocol->mpi_datatypes[S9_MPI_DATATYPE_INDEX_BYTE]);
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Type_commit", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

  //// word

  mpi_error = MPI_Type_dup(
      MPI_UINT64_T,
      &protocol->mpi_datatypes[S9_MPI_DATATYPE_INDEX_WORD]);
    // The s9_int_t and s9_real_t type (i.e. any types s9_word_t type can
    // express) must have the same endianness according to silky 9's design.
    // So, the code above should and must always work.
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Type_dup", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

  mpi_error = MPI_Type_commit(
      &protocol->mpi_datatypes[S9_MPI_DATATYPE_INDEX_WORD]);
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Type_commit", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

  //// signal

  mpi_error = MPI_Type_dup(
      protocol->mpi_datatypes[S9_MPI_DATATYPE_INDEX_BYTE],
      &protocol->mpi_datatypes[S9_MPI_DATATYPE_INDEX_SIGNAL]);
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Type_dup", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

  mpi_error = MPI_Type_commit(
      &protocol->mpi_datatypes[S9_MPI_DATATYPE_INDEX_SIGNAL]);
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Type_commit", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

  //// token

  static_assert(2 * sizeof(s9_word_t) == sizeof(s9_token_t));
  mpi_error = MPI_Type_contiguous(
      2, protocol->mpi_datatypes[S9_MPI_DATATYPE_INDEX_WORD],
      &protocol->mpi_datatypes[S9_MPI_DATATYPE_INDEX_TOKEN]);
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Type_contiguous", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

  mpi_error = MPI_Type_commit(
      &protocol->mpi_datatypes[S9_MPI_DATATYPE_INDEX_TOKEN]);
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Type_commit", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

  // finally set the initialization-completion flag

  protocol->network_is_initialized_ = true;

  return S9_OK;
}


s9_error_t
s9_finalize_network(const s9_protocol_t protocol)
{
  assert(s9_network_is_initialized(protocol));

  int mpi_error = MPI_Finalize();
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Finalize", mpi_error);
    s9_log(S9_LOG_LEVEL_ERROR, "failed to finalize the processor network.");
    return S9_ERROR_NETWORK_FINALIZE;
  }

  return S9_OK;
}


bool
s9_network_is_initialized(const s9_protocol_t protocol)
{
  return protocol.network_is_initialized_;
}


s9_processor_id_t
s9_calc_dest_processor_id(const s9_protocol_t protocol,
                          const s9_node_id_t global_node_id)
{
  assert(s9_network_is_initialized(protocol));

  return global_node_id % protocol.num_of_processors;
}


s9_node_id_t
s9_calc_local_node_id(const s9_protocol_t protocol,
                      const s9_node_id_t global_node_id)
{
  assert(s9_network_is_initialized(protocol));

  return global_node_id / num_of_processors;
}
