#ifndef _ERR_H
#define _ERR_H


typedef enum {
  Err_OK = 0, // Err_OK vs Err_NIL
  Err_COMM_INIT,
  Err_COMM_FINAL,
  Err_COMM_TAG,
  Err_COMM_SEND,
  Err_COMM_RECEIVE,
  Err_COMM_PROC_ID,
  //Err_INST_SOMETHING,
  Err_MEMORY_ALLOC,
  Err_MEMORY_NODE_ID,
  Err_PROGRAM_FILE,
  Err_PROGRAM_SYMBOL,
  Err_PROGRAM_WORDSIZE,
  //Err_PROGRAM_ENDIANNESS,
} Err;


#endif
