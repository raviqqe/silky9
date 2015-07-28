#include "s9_mpi.h"


int
s9_mpi_send(const void *buffer, MPI_Datatype datatype, const int dest,
            const int tag)
{
  return MPI_Send(buffer, 1, datatype, dest, tag, MPI_COMM_WORLD);
}


int
s9_mpi_recv(void *buffer, MPI_Datatype datatype)
{
  return MPI_Recv(buffer, 1, datatype, MPI_ANY_SOURCE, MPI_ANY_TAG,
                  MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}


int
s9_mpi_probe(MPI_Status *status)
{
  return MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
}
