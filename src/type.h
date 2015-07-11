#ifndef _TYPE_H
#define _TYPE_H


typedef uint8_t Byte;
typedef uint64_t Int;
typedef double Real;

typedef union {
  Int intNum;
  Real realNum;
} Word;

Word Word_ofInt(intNum);
Word Word_ofReal(realNum);


// dummy values

const Byte Byte_DUMMY = 0xEF;
const Int Int_DUMMY = 0xDEADBEEF;
const Word Word_DUMMY = {.intNum = 0xDEADBEEF};
const Real Real_DUMMY = (Word){.intNum = 0xDEADBEEF}.realNum;


#endif
