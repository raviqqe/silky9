#ifndef S9_NETWORK_INFO_H_
#define S9_NETWORK_INFO_H_


#include <mpi.h>
#include <stdbool.h>

#include "s9_error.h"
#include "s9_log.h"
#include "s9_mpi.h"
#include "s9_signal.h"
#include "s9_token.h"
#include "s9_type.h"


typedef s9_int_t s9_processor_id_t;
#define S9_DUMMY_PROCESSOR_ID S9_DUMMY_INT


typedef struct {
  const s9_processor_id_t processor_id;
  const s9_int_t num_of_processors;
  s9_mpi_datatype_list_t mpi_datatypes;
  bool network_is_initialized_;
} s9_protocol_t;

#define S9_DUMMY_NETWORK_INFO \
  (s9_protocol_t){ \
    .processor_id = S9_DUMMY_PROCESSOR_ID, \
    .num_of_processors = S9_DUMMY_INT, \
    .mpi_datatypes = S9_DUMMY_MPI_DATATYPE_LIST, \
    .network_is_initialized_ = false, \
  }


s9_error_t s9_initialize_network(s9_protocol_t * const protocol);
s9_error_t s9_finalize_network(const s9_protocol_t protocol);
bool s9_network_is_initialized(const s9_protocol_t protocol);
s9_processor_id_t s9_calc_dest_processor_id(
    const s9_protocol_t protocol,
    const s9_node_id_t global_node_id);
s9_node_id_t s9_calc_local_node_id(const s9_protocol_t protocol,
                                   const s9_node_id_t global_node_id);


#endif // S9_NETWORK_INFO_H_
