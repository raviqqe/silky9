#ifndef SQ_WORD_H_
#define SQ_WORD_H_


#include "word.h"

#include "type.h"


typedef union {
  const sq_int_t int_;
  const sq_real_t real_;
} sq_word_t;
#define SQ_DUMMY_WORD (sq_word_t){.int_ = 0xDEADBEEF}


sq_word_t sq_word_of_int(const sq_int_t int_num);
sq_word_t sq_word_of_real(const sq_real_t real_num);


#endif // SQ_WORD_H_
