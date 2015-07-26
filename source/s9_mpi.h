#ifndef S9_MPI_H_
#define S9_MPI_H_


#include <mpi.h>

#include "log.h"


#define s9_mpi_debug_log(mpi_function, mpi_error) \
  s9_debug_log(S9_DEBUG_LEVEL_ERROR, \
               "MPI function, %s() failed. (MPI error code: %d)", \
               (mpi_function), (mpi_error))


typedef enum {
  S9_MPI_DATATYPE_BYTE,
  S9_MPI_DATATYPE_NODE,
  S9_MPI_DATATYPE_SIGNAL,
  S9_MPI_DATATYPE_TOKEN,
  S9_MPI_DATATYPE_WORD,
} s9_mpi_datatype_t; // used as indeces in s9_network_info_t.mpi_datatypes


typedef enum {
  S9_MPI_TAG_SIGNAL,
  S9_MPI_TAG_TOKEN,
} s9_mpi_tag_t; // used by MPI_Send and MPI_Recv


#endif // S9_MPI_H_
