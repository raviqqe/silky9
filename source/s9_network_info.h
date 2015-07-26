#ifndef S9_NETWORK_INFO_H_
#define S9_NETWORK_INFO_H_


#include <mpi.h>

#include "s9_error.h"
#include "s9_log.h"
#include "s9_mpi.h"
#include "s9_node.h"
#include "s9_token.h"
#include "s9_type.h"


typedef s9_int_t s9_processor_id_t;
#define S9_DUMMY_PROCESSOR_ID S9_DUMMY_INT
    // this is not in design, but conforms to it.


typedef struct {
  const s9_processor_id_t processor_id;
  const s9_int_t num_of_processors;
  MPI_Datatype mpi_datatypes[5];
} s9_network_info_t;

#define S9_DUMMY_NETWORK_INFO \
  (s9_network_info_t){ \
    .processor_id = S9_DUMMY_PROCESSOR_ID, \
    .num_of_processors = S9_DUMMY_INT, \
    .mpi_datatypes = (MPI_Datatype[]){ \
      MPI_DATATYPE_NULL, \
      MPI_DATATYPE_NULL, \
      MPI_DATATYPE_NULL, \
      MPI_DATATYPE_NULL, \
      MPI_DATATYPE_NULL, \
    }, \
  }


s9_error_t s9_initialize_network(s9_network_info_t * const network_info);
s9_error_t s9_finalize_network();
s9_processor_id_t s9_calc_dest_processor_id(
    const s9_network_info_t network_info,
    const s9_node_id_t global_node_id);
s9_node_id_t s9_calc_local_node_id(const s9_network_info_t network_info,
                                   const s9_node_id_t global_node_id);


#endif // S9_NETWORK_INFO_H_
