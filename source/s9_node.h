#ifndef S9_NODE_H_
#define S9_NODE_H_


#include <assert.h>
#include <stdbool.h>

#include "s9_opcode.h"
#include "s9_type.h"


typedef s9_int_t s9_node_id_t; // conforming to design
#define S9_DUMMY_NODE_ID S9_DUMMY_INT


typedef union {
  s9_int_t whole;
  struct {
    bool            operand_present_bit_ : 1;
    const bool      num_of_operands_ : 1;
    const s9_int_t  opcode_category : 2;
    const s9_int_t  opcode_stem : 4;
  };
} s9_node_header_t;

#define S9_DUMMY_NODE_HEADER S9_DUMMY_BYTE

static_assert(sizeof(s9_node_header_t) == sizeof(s9_int_t),
              "s9_node_header_t's size is not compatible with s9_int_t.");


typedef struct {
  s9_node_header_t header;
  s9_word_t operand_;
  const s9_node_id_t dest;
} s9_node_t;

#define S9_DUMMY_NODE (s9_node_t){ \
  .header.whole = S9_DUMMY_NODE_HEADER, \
  .operand_ = S9_DUMMY_WORD, \
  .dest = S9_DUMMY_NODE_ID, \
}

static_assert(sizeof(s9_node_t) == 3 * sizeof(s9_word_t),
              "s9_node_t's size is not compatible with 3 contiguous "
              "s9_word_t.");

s9_node_t s9_node_of(const s9_opcode_t header,
                     const s9_word_t operand,
                     const s9_node_id_t dest);
void s9_store_operand_in_node(s9_node_t * const node,
                              const s9_word_t operand);
bool s9_operand_is_stored_in_node(const s9_node_t node);
s9_word_t s9_take_out_operand_from_node(s9_node_t * const node);
s9_word_t s9_copy_operand_in_node(const s9_node_t node);
s9_int_t s9_get_num_of_operands(const s9_header_t header);


#endif // S9_NODE_H_
