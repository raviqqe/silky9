#include "s9_type.h"


s9_word_t
s9_word_of_int(const s9_int_t int_num)
{
  return (s9_word_t){.int_num = int_num};
}


s9_word_t
s9_word_of_real(const s9_real_t real_num)
{
  return (s9_word_t){.real_num = real_num};
}
