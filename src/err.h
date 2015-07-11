#ifndef _ERR_H
#define _ERR_H


typedef enum {
  Err_OK = 0, // Err_OK vs Err_NIL
  Err_PROGRAM_FILE,
  Err_PROGRAM_SYMBOL,
  Err_PROGRAM_WORDSIZE,
  //Err_PROGRAM_ENDIANNESS,
  //Err_INST_,
  Err_COMM_INIT,
  Err_COMM_FINAL,
  Err_COMM_TAG,
} Err;


#endif
