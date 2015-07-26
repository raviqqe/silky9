#ifndef NETWORK_H_
#define NETWORK_H_


#include <mpi.h>

#include "error.h"
#include "log.h"
#include "message.h"
#include "node.h"
#include "type.h"


s9_error_t s9_initialize_network();
s9_error_t s9_finalize_network();
s9_error_t s9_send_message(const s9_message_t message);
s9_error_t s9_receive_message(s9_message_t * const message);
s9_error_t s9_get_num_of_processors(s9_int_t * const num_of_processors);
s9_error_t s9_calc_local_node_id(const s9_node_id_t global_node_id,
                                 s9_node_id_t * const local_node_id);


#endif // NETWORK_H_
