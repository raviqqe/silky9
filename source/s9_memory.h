#ifndef S9_MEMORY_H_
#define S9_MEMORY_H_


#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "s9_config.h"
#include "s9_error.h"
#include "s9_instruction.h"
#include "s9_log.h"
#include "s9_type.h"


typedef struct {
  s9_int_t num_of_nodes_;
  s9_node_t *nodes;
} s9_memory_t;

#define S9_DUMMY_MEMORY (s9_memory_t){ \
  .num_of_nodes_ = 0, \
  .nodes = NULL, \
}


s9_error_t s9_initialize_memory(s9_memory_t * const memory,
                                const s9_int_t num_of_nodes);
s9_error_t s9_finalize_memory(s9_memory_t * const memory);
s9_error_t s9_get_node_in_memory(s9_memory_t * const memory,
                                 const s9_node_id_t node_id,
                                 s9_node_t ** const node);


#endif // S9_MEMORY_H_
