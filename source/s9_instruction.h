#ifndef S9_INSTRUCTION_H_
#define S9_INSTRUCTION_H_


#include "s9_log.h"
#include "s9_token.h"
#include "s9_type.h"
#include "s9_opcode.h"


s9_error_t s9_execute_instruction(s9_node_t * const node,
                                  const s9_word_t operand);


#endif // S9_INSTRUCTION_H_
