#ifndef _NODE_H
#define _NODE_H


#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "config.h"
#include "log.h"
#include "error.h"
#include "instruction.h"
#include "type.h"


typedef sq_int_t sq_node_id_t; // this is not in design, but conforms to it.
#define SQ_DUMMY_NODE_ID SQ_DUMMY_INT

typedef struct {
  union {
    sq_byte_t header;
    struct {
      bool operand_present_bit : 1;
      const sq_instruction_id instruction_id : 8 * sizeof(header) - 1;
    };
  };
  union {
    sq_word_t stock_operand;
  };
  const sq_node_id_t dest;
} sq_node_t;

#define SQ_DUMMY_NODE (sq_node_t){ \
  .header = SQ_DUMMY_BYTE, \
  .stock_operand = SQ_DUMMY_WORD, \
  .dest = SQ_DUMMY_NODE_ID, \
}


sq_node_t sq_node_of(const sq_byte_t header,
                     const sq_word_t operand,
                     const sq_node_id_t dest);
void sq_store_operand_in_node(sq_node_t * const node, sq_word_t operand);
sq_word_t sq_take_out_operand_from_node(sq_node_t * const node);
sq_word_t sq_copy_operand(const sq_node_t node);


typedef struct {
  sq_int_t capacity;
  sq_node_t *nodes;
} sq_memory_t;

#define SQ_DUMMY_MEMORY (sq_memory_t){ \
  .capacity = SQ_DUMMY_INT, \
  .nodes = NULL, \
}

sq_error_t sq_initialize_memory(sq_memory_t * const memory);
sq_error_t sq_finalize_memory(sq_memory_t *memory);
sq_error_t sq_node_in_memory(sq_memory_t * const nodeMemory, const sq_node_tId nodeId,
                           sq_node_t ** const node);
sq_error_t sq_append_node_to_memory(sq_memory_t * const nodeMemory,
                                    const sq_node_t node);


#endif // NODE_H_
