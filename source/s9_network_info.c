#include "network.h"


// functions

s9_error_t
s9_get_num_of_processors(s9_int_t * const num_of_processors)
{
  assert(s9_network_is_initialized());

  int size = DUMMY_INT;
  int mpi_error = MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (mpi_error) {
    s9_mpi_log("MPI_Comm_size", mpi_error);
    *num_of_processors = S9_DUMMY_INT;
    return S9_ERROR_NETWORK_NUM_OF_PROCESSORS;
  }

  *num_of_processors = size;

  return S9_OK;
}


static s9_error_t
s9_calc_dest_processor_id(const s9_node_id_t global_node_id,
                          s9_processor_id_t * const dest_processor_id)
{
  assert(s9_network_is_initialized());

  s9_int_t num_of_processors = S9_DUMMY_INT;
  s9_error_t error = s9_get_num_of_processors(&num_of_processors);
  if (error) {
    s9_log(S9_LOG_LEVEL_ERROR, "failed to get number of processors.");
    *dest_processor_id = S9_DUMMY_PROCESSOR_ID;
    return error;
  }

  *dest_processor_id = global_node_id % num_of_processors;

  return S9_OK;
}


s9_error_t
s9_calc_local_node_id(const s9_node_id_t global_node_id,
                      s9_node_id_t * const local_node_id)
{
  assert(s9_network_is_initialized());

  s9_int_t num_of_processors = S9_DUMMY_INT;
  s9_error_t error = s9_get_num_of_processors(&num_of_processors);
  if (error) {
    s9_log(S9_LOG_LEVEL_ERROR, "failed to get number of processors.");
    *local_node_id = S9_DUMMY_NODE_ID;
    return error;
  }

  *local_node_id = global_node_id / num_of_processors;

  return S9_OK;
}


s9_error_t
s9_initialize_network()
{
  assert(!s9_network_is_initialized());

  int mpi_error = MPI_Init(NULL, NULL);
  if (mpi_error) {
    s9_mpi_log("MPI_Init", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

#ifndef S9_CONFIG_MPI_ERRORS_ARE_FATAL
  mpi_error = MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
  if (mpi_error) {
    s9_mpi_log("MPI_Comm_set_errhandler", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }
#endif

  // type declarations

  //// word type

  mpi_error = MPI_Type_dup(MPI_UINT64_T, &s9_mpi_word);
    // The s9_int_t and s9_real_t type (i.e. any types Word type can express)
    // must have the same endianness according to silky 9's design.
    // So, the code above should and must always work.
  if (mpi_error) {
    s9_mpi_log("MPI_Type_dup", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }
  mpi_error = MPI_Type_commit(&s9_mpi_word);
  if (mpi_error) {
    s9_mpi_log("MPI_Type_commit", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

  //// signal

  mpi_error = MPI_Type_dup(MPI_UINT64_T, &MPI_Signal);
  if (mpi_error) {
    s9_mpi_log("MPI_Type_dup", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }
  mpi_error = MPI_Type_commit(&MPI_Signal);
  if (mpi_error) {
    s9_mpi_log("MPI_Type_commit", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

  //// token

  assert(2 * sizeof(Word) == sizeof(Token));
  mpi_error = MPI_Type_contiguous(2, MPI_Word, &MPI_Token);
  if (mpi_error) {
    s9_mpi_log("MPI_Type_contiguous", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }
  mpi_error = MPI_Type_commit(&MPI_Token);
  if (mpi_error) {
    s9_mpi_log("MPI_Type_commit", mpi_error);
    return S9_ERROR_NETWORK_INITIALIZE;
  }

  return S9_OK;
}


s9_error_t
s9_finalize_network()
{
  assert(s9_network_is_initialized());

  int mpi_error = MPI_Finalize();
  if (mpi_error) {
    s9_mpi_log("MPI_Finalize", mpi_error);
    s9_log(S9_LOG_LEVEL_ERROR, "failed to finalize the processor network.");
    return S9_ERROR_NETWORK_FINALIZE;
  }

  return S9_OK;
}


#undef s9_mpi_log
