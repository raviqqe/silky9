#include "comm.h"


// types

typedef Int ProcId; // This is not in design, but compatible with it.


// global variables

MPI_Datatype MPI_Word = MPI_DATATYPE_NULL;
MPI_Datatype MPI_Node = MPI_DATATYPE_NULL;
MPI_Datatype MPI_Token = MPI_DATATYPE_NULL; // also carries Signal type value


// functions

#define MPI_DEBUG_MESSAGE(message, mpiFuncName, mpiErr) \
    DEBUG_MESSAGE( \
        "MPI: %s (MPI function: %s(), MPI error code: %d)", \
        (message), \
        (mpiFuncName), \
        (mpiErr))

#define MPI_COMM_SIZE_DEBUG_MESSAGE(mpiErr) \
    MPI_DEBUG_MESSAGE( \
        "Failed to get the number of processors.", \
        "MPI_Comm_size", \
        (mpiErr))


static Err
calcDest(const NodeId globalNodeId, ProcId * const destProcId)
{
  Int numOfProcs = Int_DUMMY;

  int mpiErr = MPI_Comm_size(MPI_COMM_WORLD, &numOfProcs);
  if (mpiErr) {
    MPI_COMM_SIZE_DEBUG_MESSAGE(mpiErr);
    return Err_COMM_NUM_OF_PROCS;
  }

  *destProcId = globalNodeId % numOfProcs; 

  return Err_OK;
}


Err
comm_calcLocalNodeId(const NodeId globalNodeId, NodeId * const localNodeId)
{
  Int numOfProcs = Int_DUMMY;

  int mpiErr = MPI_Comm_size(MPI_COMM_WORLD, &numOfProcs);
  if (mpiErr) {
    MPI_COMM_SIZE_DEBUG_MESSAGE(mpiErr);
    return Err_COMM_NUM_OF_PROCS;
  }

  *localNodeId = globalNodeId / numOfProcs;

  return Err_OK;
}

#undef MPI_COMM_SIZE_DEBUG_MESSAGE


Err
comm_init()
{
  int mpiErr = MPI_Init(NULL, NULL);
  if (mpiErr) {
    MPI_DEBUG_MESSAGE("Failed to initialize MPI library.", "MPI_Init", mpiErr);
    return Err_COMM_INIT;
  }

  mpiErr = MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
  if (mpiErr) {
    MPI_DEBUG_MESSAGE(
        "Failed to set the error handler of MPI library.",
        "MPI_Comm_set_errhandler", mpiErr);
    return Err_COMM_INIT;
  }

  // Type declarations

  mpiErr = MPI_Type_dup(MPI_UINT64_T, &MPI_Word);
    // The Int and Real type (i.e. any types Word type can express) must have
    // the same endianness according to Silky 9's design. So, the code above
    // should and must always work.
  if (mpiErr) {
    MPI_DEBUG_MESSAGE(
        "Failed to define Word type for MPI library.",
        "MPI_Type_dup", mpiErr);
    return Err_COMM_INIT;
  }
  mpiErr = MPI_Type_commit(&MPI_Word);
  if (mpiErr) {
    MPI_DEBUG_MESSAGE(
        "Failed to commit Word type for MPI library.",
        "MPI_Type_commit", mpiErr);
    return Err_COMM_INIT;
  }

  //// Token

  mpiErr = MPI_Type_contiguous(2, MPI_Word, &MPI_Token);
  if (mpiErr) {
    MPI_DEBUG_MESSAGE(
        "Failed to define Token type for MPI library.",
        "MPI_Type_contiguous", mpiErr);
    return Err_COMM_INIT;
  }
  mpiErr = MPI_Type_commit(MPI_Token);
  if (mpiErr) {
    MPI_DEBUG_MESSAGE(
        "Failed to commit Token type for MPI library.",
        "MPI_Type_commit", mpiErr);
    return Err_COMM_INIT;
  }

  //// Node

  mpiErr = MPI_Type_contiguous(3, MPI_Word, &MPI_Node);
  if (mpiErr) {
    MPI_DEBUG_MESSAGE(
        "Failed to define Node type for MPI library.",
        "MPI_Type_contiguous", mpiErr);
    return Err_COMM_INIT;
  }
  mpiErr = MPI_Type_commit(&MPI_Node);
  if (mpiErr) {
    MPI_DEBUG_MESSAGE(
        "Failed to commit Node type for MPI library.",
        "MPI_Type_commit", mpiErr);
    return Err_COMM_INIT;
  }

  return Err_OK;
}


Err
comm_final()
{
  int mpiErr = MPI_Finalize();
  if (mpiErr) {
    MPI_DEBUG_MESSAGE(
        "Failed to finalize MPI library.",
        "MPI_Finalize", mpiErr);
    return Err_COMM_FINAL;
  }

  return Err_OK;
}


Err
comm_sendMessage(const Message message)
{
  switch (message.tag) {
  case MessageTag_TOKEN:
    MPI_Send(&message.token, 1, MPI_Token, comm_calcDest);
    break;
  case MessageTag_NODE:
    MPI_Send(&message.node, 1, MPI_Node, nodeId);
    break;
  case MessageTag_SIGNAL:
    MPI_Bcast(&message.signal, 1, MPI_Signal,);
    break;
  default:
    DEBUG_MESSAGE(
        "Unknown message tag detected. (message tag: %d)", message.tag);
    return Err_COMM_TAG;
  }
  
  return Err_OK;
}


Err
comm_receiveMessage(Message * const message)
{
  MPI_Status status;

  int mpiErr = MPI_Recv();
  if (mpiErr) {
    MPI_DEBUG_MESSAGE(
    return Err_COMM_RECEIVE;
  }

  return Err_OK;
}


Err
comm_amIMaster(bool *answer)
{
  Int myRank = Int_DUMMY;

  int mpiErr = MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  if (mpiErr) {
    MPI_DEBUG_MESSAGE(
        "Failed to get the rank of the current process.",
        "MPI_Comm_rank", mpiErr);
    return Err_COMM_PROC_ID;
  }

  *answer = (myRank == 0);

  return Err_OK;
}

#undef MPI_DEBUG_MESSAGE
