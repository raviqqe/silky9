#include "s9_instruction.h"


s9_error_t
s9_execute_instruction(s9_node_t * const node, const s9_word_t operand)
{
  assert(node != NULL);

  s9_word_t operand_in_node = S9_DUMMY_NODE;

  if (s9_get_num_of_operands(node->header) == 2
      && !s9_operand_is_stored_in_node(*node)) {
    s9_store_operand_in_node(node, operand);
    return S9_OK;
  } else if (s9_get_num_of_operands(node->header) == 2) {
    operand_in_node = s9_take_out_operand_from_node(node);
  } else {
    operand_in_node = s9_copy_operand_in_node(*node);
  }

  switch (node->header.opcode_category) {
  case S9_OPCODE_CATEGORY_CONTROL:
    {
      s9_error_t error = s9_execute_control_opcode(node, operand);
      if (error) {
        s9_debug_log(S9_LOG_LEVEL_ERROR,
                     "failed to execute a control opcode. "
                     "(opcode id: %d)", node->header.opcode_stem);
        return error;
      }
    }
    break;
  case S9_OPCODE_CATEGORY_LOGIC:
    s9_send_token(s9_token_of(node->dest, s9_word_of_int(
        s9_execute_logic_opcode(operand_in_node, operand))));
    if (error) {
      s9_debug_log(S9_LOG_LEVEL_ERROR, "failed to send a token.");
      return error;
    }
    break;
  case S9_OPCODE_CATEGORY_INT:
    s9_send_token(s9_token_of(node->dest, s9_word_of_int(
        s9_execute_int_opcode(operand_in_node, operand))));
    if (error) {
      s9_debug_log(S9_LOG_LEVEL_ERROR, "failed to send a token.");
      return error;
    }
    break;
  case S9_OPCODE_CATEGORY_REAL:
    s9_send_token(s9_token_of(node->dest, s9_word_of_real(
        s9_execute_int_opcode(operand_in_node, operand))));
    if (error) {
      s9_debug_log(S9_LOG_LEVEL_ERROR, "failed to send a token.");
      return error;
    }
    break;
  default:
    s9_log_debug(S9_LOG_LEVEL_WARNING, "unknown opcode category detected.");
  }

  return S9_OK;
}
