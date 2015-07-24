#ifndef _TYPE_H
#define _TYPE_H


#include <stdint.h>


typedef uint8_t Byte;
typedef uint64_t Int;
typedef double Real;

typedef union {
  Int intNum;
  Real realNum;
} Word;

Word Word_ofInt(const Int intNum);
Word Word_ofReal(const Real realNum);


// dummy values

#define Byte_DUMMY 0xEF
#define Int_DUMMY 0xDEADBEEF
#define Word_DUMMY (Word){.intNum = 0xDEADBEEF}
#define Real_DUMMY (Word){.intNum = 0xDEADBEEF}.realNum


#define int_DUMMY 0xDEADBEEF


#endif
