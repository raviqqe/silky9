#include "node.h"


s9_node_t
s9_node_of(const s9_int_t header,
           const s9_word_t operand,
           const s9_int_t dest)
{
  return (s9_node_t){
    .header = header,
    .operand_ = operand,
    .dest = dest,
  };
}


void
s9_store_operand_in_node(s9_node_t * const node, const s9_word_t operand)
{
  assert(node->operand_present_bit_ == false);

  *node = (s9_node_t){
    .operand_present_bit_ = true;
    .operand_ = operand;
  };
}


s9_word_t
s9_take_out_operand_from_node(s9_node_t * const node)
{
  assert(node->operand_present_bit_ == true);

  s9_word_t returned_operand = node->operand_;
  *node = (s9_node_t){
    .operand_present_bit_ = false;
    .operand_ = S9_DUMMY_WORD;
  };

  return returned_operand;
}


s9_word_t
s9_copy_operand_in_node(const s9_node_t node)
{
  return node.operand_;
}
