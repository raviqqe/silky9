#include "program.h"


#define FREAD_DEBUG_MESSAGE(readSize) \
    DEBUG_MESSAGE( \
        "Failed to read enough size of data from program file. " \
        "(size of read data is %dB)", (readSize))


static Err
checkSymbol(FILE *programFile)
{
  const char kSymbol[] = "JACK";
  char symbolOfFile[sizeof(kSymbol)] = {'\0'};

  size_t readSize = fread(symbolOfFile, 1, strlen(kSymbol), programFile);
  if (readSize != strlen(kSymbol)) {
    FREAD_DEBUG_MESSAGE(readSize);
    return Err_PROGRAM_FILE;
  }

  int strncmpErr = strncmp(symbolOfFile, kSymbol, strlen(kSymbol));
  if (strncmpErr) {
    DEBUG_MESSAGE(
        "Program file's symbol, %s doesn't match with the correct symbol, "
        "\"%s\".", symbolOfFile, kSymbol);
    return Err_PROGRAM_SYMBOL;
  }

  return Err_OK;
}


static Err
checkWordSize(FILE *programFile)
{
  Byte sizeOfWord;

  size_t readSize = fread(&sizeOfWord, 1, sizeof(sizeOfWord), programFile);
  if (readSize != sizeof(sizeOfWord)) {
    FREAD_DEBUG_MESSAGE(readSize);
    return Err_PROGRAM_FILE;
  } else if (sizeOfWord != sizeof(Word)) {
    DEBUG_MESSAGE(
        "Program file's word size doesn't match with virtual machine's. "
        "(machine: %d * 8 bits, file: %d * 8 bits)", sizeof(Word), sizeOfWord);
    return Err_PROGRAM_WORDSIZE;
  }

  return Err_OK;
}


//static Err
//readByte(FILE *programFile, Byte *byteMem)
//{
//  size_t readSize = fread(byteMem, 1, sizeof(*byteMem), programFile);
//  if (readSize != sizeof(*byteMem)) {
//    FREAD_DEBUG_MESSAGE(readSize);
//    return Err_PROGRAM_FILE;
//  }
//
//  return Err_OK;
//}


static Err
readInt(FILE *programFile, Int *intMem)
{
  size_t readSize = fread(intMem, 1, sizeof(*intMem), programFile);
  if (readSize != sizeof(*intMem)) {
    FREAD_DEBUG_MESSAGE(readSize);
    return Err_PROGRAM_FILE;
  }

  return Err_OK;
}

#undef FREAD_DEBUG_MESSAGE


static Err
readWord(FILE *programFile, Word *wordMem)
{
  return readInt(programFile, (Int *)wordMem);
}


static Err
readNodeId(FILE *programFile, NodeId *nodeIdMem)
{
  return readInt(programFile, (Int *)nodeIdMem);
}


static Err
readNode(FILE *programFile, Node *node)
{
  *node = Node_DUMMY;

  Int header;
  Err err = readInt(programFile, &header);
  if (err) {
    DEBUG_MESSAGE(
        "Failed to read the instruction field of a node from the program "
        "file.");
    return err;
  }

  // If the read node's first word field is a operand buffer,
  // this process makes no sense.
  NodeId subDest;
  err = readNodeId(programFile, &subDest);
  if (err) {
    DEBUG_MESSAGE(
        "Failed to read the sub-destination (or the operand buffer of a "
        "unknown value) of a node from the program file.");
    return err;
  }

  NodeId dest;
  err = readNodeId(programFile, &dest);
  if (err) {
    DEBUG_MESSAGE(
        "Failed to read the destination of a node from the program file.");
    return err;
  }

  *node = Node_of(header, Word_ofInt(subDest), dest);

  return Err_OK;
}


static Err
dispatchNodes(FILE *programFile)
{
  Int numOfNodes = Int_DUMMY;

  Err err = readInt(programFile, &numOfNodes);
  if (err) {
    DEBUG_MESSAGE("Failed to read the number of nodes from the program file.");
    return err;
  }

  for (Int nodeId = 0; nodeId < numOfNodes; nodeId++) {
    Node node;
    err = readNode(programFile, &node);
    if (err) {
      DEBUG_MESSAGE("Failed to read a node from the program file.");
      return err;
    }

    err = comm_sendMessage(Message_ofNodeUpdate(NodeUpdate_of(nodeId, node)));
    if (err) {
      DEBUG_MESSAGE("Failed to send a node while reading the program file.");
      return err;
    }
  }

  return Err_OK;
}


static Err
sendInitialTokens(FILE *programFile)
{
  Int numOfInitialTokens;

  Err err = readInt(programFile, &numOfInitialTokens);
  if (err) {
    DEBUG_MESSAGE(
        "Failed to read number of initial tokens from program file.");
    return err;
  }

  for (Int i = 0; i < numOfInitialTokens; i++) {
    NodeId dest;
    err = readNodeId(programFile, &dest);
    if (err) {
      DEBUG_MESSAGE(
          "Failed to read the destination of a initial token from the program "
          "file.");
      return err;
    }

    Word value;
    err = readWord(programFile, &value);
    if (err) {
      DEBUG_MESSAGE(
          "Failed to read the value of a initial token from the program "
          "file.");
      return err;
    }

    err = comm_sendMessage(Message_ofToken(Token_of(dest, value)));
    if (err) {
      DEBUG_MESSAGE("Failed to send a token while reading the program file.");
      return err;
    }
  }

  return Err_OK;
}


static Err
loadProgramFromFileStruct(FILE *programFile)
{
  Err err = checkSymbol(programFile);
  if (err) {
    DEBUG_MESSAGE("Check of program file's symbol failed.");
    return err;
  }

  err = checkWordSize(programFile);
  if (err) {
    DEBUG_MESSAGE("Check of program file's word size failed.");
    return err;
  }

  //
  // checkEndianess() is not implemented yet.
  //

  err = dispatchNodes(programFile);
  if (err) {
    DEBUG_MESSAGE("Failed to dispatch nodes of program to processors.");
    return err;
  }

  err = sendInitialTokens(programFile);
  if (err) {
    DEBUG_MESSAGE("Failed to send the initial tokens of the program.");
    return err;
  }

  return err;
}


Err
program_loadProgram(const char programFileName[])
{
  FILE *programFile = (programFileName == program_STDIN) ?
      stdin : fopen(programFileName, "rb");

  Err err = loadProgramFromFileStruct(programFile);
  if (err && programFile == stdin) {
    DEBUG_MESSAGE("Failed to load the program from stdin.");
  } else if (err) {
    DEBUG_MESSAGE(
        "Failed to load the program from the program file, %s.",
        programFileName);
  }

  if (programFile != stdin) {
    fclose(programFile);
  }
  
  return err;
}
