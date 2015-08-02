#include "s9_opcode.h"


typedef s9_error_t (*s9_opcode_control_t)(const s9_node_t node,
                                          const s9_word_t operand);


static s9_error
s9_opcode_control_copy(s9_word_t word)
{
  s9_send_

}


s9_int_t
s9_opcode_int_equal(const s9_int_t int_num_1, const s9_int_t int_num_2)
{
  return int_num_1 == int_num_2;
}

s9_int_t
s9_opcode_int_greater_than(const s9_int_t int_num_1, const s9_int_t int_num_2)
{
  return int_num_1 > int_num_2;
}

s9_int_t
s9_opcode_int_less_than(const s9_int_t int_num_1, const s9_int_t int_num_2)
{
  return int_num_1 < int_num_2;
}

s9_int_t
s9_opcode_int_add(const s9_int_t int_num_1, const s9_int_t int_num_2)
{
  return int_num_1 + int_num_2;
}

s9_int_t
s9_opcode_int_multiply(s9_int_t int_num_1, s9_int_t int_num_2)
{
  return int_num_1 * int_num_2;
}

s9_error_t
s9_execute_cv_opcode()
{

}

s9_error_t
s9_execute_vv_opcode()
{

}
