#include "s9_instruction.h"


// PLEASE MOVE TO s9_opcode.c

static s9_error_t
s9_send_word(const s9_node_id_t dest, const s9_word_t value)
{
  return s9_send_token(s9_token_of(dest, value));
}


static s9_error_t
s9_send_int(s9_node_id_t dest, s9_int_t int_num)
{
  return s9_send_word(dest, s9_word_of_int(int_num));
}


static s9_error_t
s9_send_real(s9_node_id_t dest, s9_real_t real_num)
{
  return s9_send_word(dest, s9_word_of_real(real_num));
}


// opcodes

static s9_token_t
s9_opcode_control_copy()
{
  s9_send_word(node->int_num, operand);
  s9_send_word(node->dest, operand);
}


// s9_instruction.c

s9_error_t
s9_execute_instruction(s9_node_t * const node, const s9_word_t operand)
{
  s9_error_t error = S9_OK;

  if (s9_get_num_of_operands_of_opcode(node->opcode) == 2
      && s9_operand_is_stored_in_node(*node)) {
    s9_store_operand_in_node(node, operand);
  } else {
    s9_word_t operand_in_node;

    switch (node->opcode) {
    case inst_OUTPUT:
      break;
      //send_int(node->dest, io(operand.intNum));
      break;
    case inst_SHUTDOWN:
      error = comm_sendMessage(Message_ofSignal(Signal_SHUTDOWN));
      break;
    case inst_BOOL_NOT:
      error = sends9_int_t(node->dest, !operand.intNum);
      break;
    case inst_BOOL_AND:
      error = sends9_int_t(node->dest,
                    operand.intNum && s9_node_t_takeOutOperand(node).intNum);
      break;
    case inst_BOOL_OR:
      error = sends9_int_t(node->dest,
                    operand.intNum || s9_node_t_takeOutOperand(node).intNum);
      break;
    case inst_BOOL_XOR:
      operandIns9_node_t.intNum = s9_node_t_takeOutOperand(node).intNum;
      error = sends9_int_t(node->dest,
                    !(operand.intNum && operandIns9_node_t.intNum)
                    && !(operand.intNum || operandIns9_node_t.intNum));
      break;
    default:
      s9_debug_log(S9_LOG_LEVEL_ERROR,
                   "unknown opcode detected. (opcode = %x)", node->opcode);
      return S9_ERROR_OPCODE_UNKNOWN;
    }

    if (error) {
      s9_debug_log(S9_LOG_LEVEL_ERROR,
                   "failed to execute an instruction of opcode, %d.",
                   node->opcode);
      return error;
    }
  }

  return error;
}
