#include "network.h"


s9_error_t
s9_initialize_network(s9_network_info_t * network_info)
{
  assert(!s9_network_is_initialized(*network_info));

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

  /////////////////////////////////////////////////////////////////////////////
  ////  PLEASE IMPLEMENT ME  //////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////

  // get num of processors

  /////////////////////////////////////////////////////////////////////////////
  ////  PLEASE IMPLEMENT ME  //////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////

  // type declarations

  //// byte

  mpi_error = MPI_Type_dup(
      MPI_UINT8_T,
      &network_info->mpi_datatypes[S9_MPI_DATATYPE_INDEX_BYTE]);
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Type_dup", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

  mpi_error = MPI_Type_commit(
      &network_info->mpi_datatypes[S9_MPI_DATATYPE_INDEX_BYTE]);
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Type_commit", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

  //// word

  mpi_error = MPI_Type_dup(
      MPI_UINT64_T,
      &network_info->mpi_datatypes[S9_MPI_DATATYPE_INDEX_WORD]);
    // The s9_int_t and s9_real_t type (i.e. any types s9_word_t type can express)
    // must have the same endianness according to silky 9's design.
    // So, the code above should and must always work.
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Type_dup", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

  mpi_error = MPI_Type_commit(
      &network_info->mpi_datatypes[S9_MPI_DATATYPE_INDEX_WORD]);
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Type_commit", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

  //// signal

  mpi_error = MPI_Type_dup(
      network_info->mpi_datatypes[S9_MPI_DATATYPE_INDEX_BYTE],
      &network_info->mpi_datatypes[S9_MPI_DATATYPE_INDEX_SIGNAL]);
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Type_dup", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

  mpi_error = MPI_Type_commit(
      &network_info->mpi_datatypes[S9_MPI_DATATYPE_INDEX_SIGNAL]);
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Type_commit", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

  //// token

  static_assert(2 * sizeof(s9_word_t) == sizeof(s9_token_t));
  mpi_error = MPI_Type_contiguous(
      2, network_info->mpi_datatypes[S9_MPI_DATATYPE_INDEX_WORD],
      &network_info->mpi_datatypes[S9_MPI_DATATYPE_INDEX_TOKEN]);
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Type_contiguous", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

  mpi_error = MPI_Type_commit(
      &network_info->mpi_datatypes[S9_MPI_DATATYPE_INDEX_TOKEN]);
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Type_commit", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

  // finally set the initialization-completion flag

  network_info->network_is_initialized_ = true;

  return S9_OK;
}


s9_error_t
s9_finalize_network(const s9_network_info_t network_info)
{
  assert(s9_network_is_initialized(network_info));

  int mpi_error = MPI_Finalize();
  if (mpi_error) {
    s9_mpi_debug_log("MPI_Finalize", mpi_error);
    s9_log(S9_LOG_LEVEL_ERROR, "failed to finalize the processor network.");
    return S9_ERROR_NETWORK_FINALIZE;
  }

  return S9_OK;
}


bool
s9_network_is_initialized(const s9_network_info_t network_info)
{
  return network_info.network_is_initialized_;
}


s9_processor_id_t
s9_calc_dest_processor_id(const s9_network_info_t network_info,
                          const s9_node_id_t global_node_id)
{
  assert(s9_network_is_initialized(network_info));

  return global_node_id % network_info.num_of_processors;
}


s9_node_id_t
s9_calc_local_node_id(const s9_network_info_t network_info,
                      const s9_node_id_t global_node_id)
{
  assert(s9_network_is_initialized(network_info));

  return global_node_id / num_of_processors;
}
