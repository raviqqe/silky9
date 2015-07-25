#ifndef NETWORK_H_
#define NETWORK_H_


#include <mpi.h>
#include <stdbool.h>

#include "error.h"
#include "log.h"
#include "message.h"
#include "mpi_wrapper.h"
#include "node.h"
#include "type.h"
#include "word.h"


s9_error_t s9_initialize_network();
s9_error_t s9_finalize_network();
s9_error_t s9_send_message(const s9_message_t message);
s9_error_t s9_receive_message(s9_message_t * const message);
s9_node_id_t s9_calc_local_node_id(const s9_node_id_t global_node_id);


#endif // NETWORK_H_
