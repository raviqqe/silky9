#ifndef S9_TYPE_H_
#define S9_TYPE_H_


#include <stdint.h>


//typedef uint8_t s9_byte_t;
//#define S9_DUMMY_BYTE 0xEF
typedef uint64_t s9_int_t;
#define S9_DUMMY_INT 0xDEADBEEF
typedef double s9_real_t;
#define S9_DUMMY_REAL 42424242
#define DUMMY_INT 0xDEADBEEF // dummy value for built-in int

typedef union {
  const s9_int_t int_num;
  const s9_real_t real_num;
} s9_word_t;
#define S9_DUMMY_WORD (s9_word_t){.int_num = 0xDEADBEEF}


s9_word_t s9_word_of_int(const s9_int_t int_num);
s9_word_t s9_word_of_real(const s9_real_t real_num);


#endif // S9_TYPE_H_
