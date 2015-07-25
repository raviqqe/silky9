#ifndef MEMORY_H_
#define MEMORY_H_


#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "config.h"
#include "log.h"
#include "error.h"
#include "instruction.h"
#include "type.h"


typedef struct {
  s9_int_t capacity;
  s9_node_t *nodes;
} s9_memory_t;

#define S9_DUMMY_MEMORY (s9_memory_t){ \
  .capacity = S9_DUMMY_INT, \
  .nodes = NULL, \
}


s9_error_t s9_initialize_memory(s9_memory_t * const memory);
s9_error_t s9_finalize_memory(s9_memory_t * const memory);
s9_error_t s9_node_in_memory(s9_memory_t * const memory,
                             const s9_node_id_t node_id,
                             s9_node_t ** const node);
s9_error_t s9_append_node_to_memory(s9_memory_t * const memory,
                                    const s9_node_t node);


#endif // MEMORY_H_
