#ifndef S9_MPI_H_
#define S9_MPI_H_


#include <mpi.h>

#include "s9_log.h"


#define s9_mpi_debug_log(mpi_function, mpi_error) \
  s9_debug_log(S9_DEBUG_LEVEL_ERROR, \
               "MPI: function, %s() failed. (MPI error code: %d)", \
               (mpi_function), (mpi_error))


typedef MPI_Datatype s9_mpi_datatype_list[5];


typedef enum {
  S9_MPI_DATATYPE_INDEX_BYTE,
  S9_MPI_DATATYPE_INDEX_NODE,
  S9_MPI_DATATYPE_INDEX_SIGNAL,
  S9_MPI_DATATYPE_INDEX_TOKEN,
  S9_MPI_DATATYPE_INDEX_WORD,
} s9_mpi_datatype_index_t;
  // used as indeces of s9_mpi_datatype_array_t


typedef enum {
  S9_MPI_TAG_SIGNAL,
  S9_MPI_TAG_TOKEN,
} s9_mpi_tag_t; // used by s9_mpi_send and s9_mpi_recv


int s9_mpi_send(const void *buffer, MPI_Datatype datatype, const int dest,
                const int tag);
int s9_mpi_recv(void *buffer, MPI_Datatype datatype);
int s9_mpi_probe(MPI_Status *status);


#endif // S9_MPI_H_
