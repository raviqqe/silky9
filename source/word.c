#include "word.h"


sq_word_t
sq_word_of_int(const sq_int_t int_num)
{
  return (sq_word_t){.int_ = int_num};
}


sq_word_t
sq_word_of_real(const sq_real_t real_num)
{
  return (wq_word_t){.real_ = real_num};
}
