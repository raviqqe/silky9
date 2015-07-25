#include "program.h"


#define s9_fread_log(read_size) \
    s9_log(S9_LOG_LEVEL_DEBUG, \
           "Failed to read enough size of data from program file. " \
           "(size of read data is %dB)", (read_size))


static s9_error_t
s9_check_symbol_(FILE *program_file)
{
  const char jack_symbol[] = "JACK";
  char symbol_in_file[sizeof(symbol)] = {'\0'};

  size_t read_size = fread(symbol_in_file, 1, strlen(symbol), program_file);
  if (read_size != strlen(symbol)) {
    s9_fread_log(read_size);
    return S9_ERROR_PROGRAM_SIZE;
  }

  int strncmps9_error_t = strncmp(symbolOfFile, kSymbol, strlen(kSymbol));
  if (strncmps9_error_t) {
    s9_log(S9_LOG_LEVEL_DEBUG, "Program file's symbol, \"%s\" doesn't match with the correct "
               "symbol, \"%s\".", symbolOfFile, kSymbol);
    return s9_error_t_PROGRAM_SYMBOL;
  }

  return S9_ERROR_OK;
}


static s9_error_t
checkWordSize(FILE *programFile)
{
  Byte sizeOfWord;

  size_t read_size = fread(&sizeOfWord, 1, sizeof(sizeOfWord), programFile);
  if (read_size != sizeof(sizeOfWord)) {
    s9_fread_log(read_size);
    return s9_error_t_PROGRAM_FILE;
  } else if (sizeOfWord != sizeof(Word)) {
    s9_log(S9_LOG_LEVEL_DEBUG, "Program file's word size doesn't match with virtual "
               "machine's. (machine: %d * 8 bits, file: %d * 8 bits)",
               sizeof(Word), sizeOfWord);
    return s9_error_t_PROGRAM_WORDSIZE;
  }

  return S9_ERROR_OK;
}


//static s9_error_t
//readByte(FILE *programFile, Byte *byteMem)
//{
//  size_t read_size = fread(byteMem, 1, sizeof(*byteMem), programFile);
//  if (read_size != sizeof(*byteMem)) {
//    s9_fread_log(read_size);
//    return s9_error_t_PROGRAM_FILE;
//  }
//
//  return S9_ERROR_OK;
//}


static s9_error_t
readInt(FILE *programFile, Int *intMem)
{
  size_t read_size = fread(intMem, 1, sizeof(*intMem), programFile);
  if (read_size != sizeof(*intMem)) {
    s9_fread_log(read_size);
    return s9_error_t_PROGRAM_FILE;
  }

  return S9_ERROR_OK;
}

#undef s9_fread_log


static s9_error_t
readWord(FILE *programFile, Word *wordMem)
{
  return readInt(programFile, (Int *)wordMem);
}


static s9_error_t
readNodeId(FILE *programFile, NodeId *nodeIdMem)
{
  return readInt(programFile, (Int *)nodeIdMem);
}


static s9_error_t
readNode(FILE *programFile, Node *node)
{
  *node = Node_DUMMY;

  Int header;
  s9_error_t error = readInt(programFile, &header);
  if (error) {
    s9_log(S9_LOG_LEVEL_DEBUG,
        "Failed to read the instruction field of a node from the program "
        "file.");
    return error;
  }

  // If the read node's first word field is a operand buffer,
  // this process makes no sense.
  NodeId subDest;
  error = readNodeId(programFile, &subDest);
  if (error) {
    s9_log(S9_LOG_LEVEL_DEBUG,
        "Failed to read the sub-destination (or the operand buffer of a "
        "unknown value) of a node from the program file.");
    return error;
  }

  NodeId dest;
  error = readNodeId(programFile, &dest);
  if (error) {
    s9_log(S9_LOG_LEVEL_DEBUG,
        "Failed to read the destination of a node from the program file.");
    return error;
  }

  *node = Node_of(header, Word_ofInt(subDest), dest);

  return S9_ERROR_OK;
}


static s9_error_t
dispatchNodes(FILE *programFile)
{
  Int numOfNodes = Int_DUMMY;

  s9_error_t error = readInt(programFile, &numOfNodes);
  if (error) {
    s9_log(S9_LOG_LEVEL_DEBUG, "Failed to read the number of nodes from the program file.");
    return error;
  }

  for (Int nodeId = 0; nodeId < numOfNodes; nodeId++) {
    Node node;
    error = readNode(programFile, &node);
    if (error) {
      s9_log(S9_LOG_LEVEL_DEBUG, "Failed to read a node from the program file.");
      return error;
    }

    error = comm_sendMessage(Message_ofNodeUpdate(NodeUpdate_of(nodeId, node)));
    if (error) {
      s9_log(S9_LOG_LEVEL_DEBUG, "Failed to send a node while reading the program file.");
      return error;
    }
  }

  return S9_ERROR_OK;
}


static s9_error_t
sendInitialTokens(FILE *programFile)
{
  Int numOfInitialTokens;

  s9_error_t error = readInt(programFile, &numOfInitialTokens);
  if (error) {
    s9_log(S9_LOG_LEVEL_DEBUG,
        "Failed to read number of initial tokens from program file.");
    return error;
  }

  for (Int i = 0; i < numOfInitialTokens; i++) {
    NodeId dest;
    error = readNodeId(programFile, &dest);
    if (error) {
      s9_log(S9_LOG_LEVEL_DEBUG,
          "Failed to read the destination of a initial token from the program "
          "file.");
      return error;
    }

    Word value;
    error = readWord(programFile, &value);
    if (error) {
      s9_log(S9_LOG_LEVEL_DEBUG,
             "Failed to read the value of a initial token from the program "
             "file.");
      return error;
    }

    error = comm_sendMessage(Message_ofToken(Token_of(dest, value)));
    if (error) {
      s9_log(S9_LOG_LEVEL_DEBUG,
             "Failed to send a token while reading the program file.");
      return error;
    }
  }

  return S9_ERROR_OK;
}


static s9_error_t
loadProgramFromFileStruct(FILE *programFile)
{
  s9_error_t error = checkSymbol(programFile);
  if (error) {
    s9_log(S9_LOG_LEVEL_DEBUG, "Check of program file's symbol failed.");
    return error;
  }

  error = checkWordSize(programFile);
  if (error) {
    s9_log(S9_LOG_LEVEL_DEBUG, "Check of program file's word size failed.");
    return error;
  }

  //
  // checkEndianess() is not implemented yet.
  //

  error = dispatchNodes(programFile);
  if (error) {
    s9_log(S9_LOG_LEVEL_DEBUG,
           "Failed to dispatch nodes of program to processors.");
    return error;
  }

  error = sendInitialTokens(programFile);
  if (error) {
    s9_log(S9_LOG_LEVEL_DEBUG,
           "Failed to send the initial tokens of the program.");
    return error;
  }

  return error;
}


s9_error_t
s9_load_program(const char program_filename[])
{
  FILE *program_file = fopen(program_filename, "rb");

  s9_error_t error = loadProgramFromFileStruct(programFile);
  if (error && programFile == stdin) {
    s9_log(S9_LOG_LEVEL_DEBUG, "failed to load the program from stdin.");
  } else if (error) {
    s9_log(
        "failed to load the program from the program file, %s.",
        programFileName);
  }

  if (program_filename != stdin) {
    fclose(program_file);
  }

  return error;
}
