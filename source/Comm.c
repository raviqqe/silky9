#include "comm.h"


// types

typedef Int ProcId; // This is not in design, but compatible with it.
#define ProcId_DUMMY Int_DUMMY


// global variables

MPI_Datatype MPI_Word = MPI_DATATYPE_NULL;
MPI_Datatype MPI_NodeUpdate = MPI_DATATYPE_NULL;
MPI_Datatype MPI_Token = MPI_DATATYPE_NULL;
MPI_Datatype MPI_Signal = MPI_DATATYPE_NULL;


// functions

#define MPI_DEBUG_INFO(message, mpiFuncName, mpiErr) \
    DEBUG_INFO( \
        "MPI: %s (MPI function: %s(), MPI error code: %d)", \
        (message), \
        (mpiFuncName), \
        (mpiErr))

#define MPI_COMM_SIZE_DEBUG_INFO(mpiErr) \
    MPI_DEBUG_INFO( \
        "Failed to get the number of processors.", \
        "MPI_Comm_size", \
        (mpiErr))

static Err
getNumOfProcs(Int * const numOfProcs)
{
  int size = int_DUMMY;
  int mpiErr = MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (mpiErr) {
    MPI_COMM_SIZE_DEBUG_INFO(mpiErr);
    return Err_COMM_NUM_OF_PROCS;
  }

  *numOfProcs = size;

  return Err_OK;
}


static Err
calcDestProcId(const NodeId globalNodeId, ProcId * const destProcId)
{
  Int numOfProcs = Int_DUMMY;

  Err err = getNumOfProcs(&numOfProcs);
  if (err) {
    DEBUG_INFO("Failed to get number of processors.");
    return err;
  }

  *destProcId = globalNodeId % numOfProcs; 

  return err;
}


Err
comm_calcLocalNodeId(const NodeId globalNodeId, NodeId * const localNodeId)
{
  Int numOfProcs = Int_DUMMY;

  Err err = getNumOfProcs(&numOfProcs);
  if (err) {
    DEBUG_INFO("Failed to get number of processors.");
    return err;
  }

  *localNodeId = globalNodeId / numOfProcs;

  return err;
}

#undef MPI_COMM_SIZE_DEBUG_INFO


#define MPI_TYPE_DEFINE_DEBUG_INFO(type, mpiFuncName, mpiErr) \
    MPI_DEBUG_INFO( \
        "Failed to define " type " type for MPI library.", \
        (mpiFuncName), \
        (mpiErr))

#define MPI_TYPE_COMMIT_DEBUG_INFO(type, mpiErr) \
    MPI_DEBUG_INFO( \
        "Failed to commit " type " type for MPI library.", \
        "MPI_Type_commit", \
        (mpiErr))

Err
comm_init()
{
  int mpiErr = MPI_Init(NULL, NULL);
  if (mpiErr) {
    MPI_DEBUG_INFO("Failed to initialize MPI library.", "MPI_Init", mpiErr);
    return Err_COMM_INIT;
  }

  mpiErr = MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
  if (mpiErr) {
    MPI_DEBUG_INFO(
        "Failed to set the error handler of MPI library.",
        "MPI_Comm_set_errhandler", mpiErr);
    return Err_COMM_INIT;
  }

  // Type declarations

  //// Word

  mpiErr = MPI_Type_dup(MPI_UINT64_T, &MPI_Word);
    // The Int and Real type (i.e. any types Word type can express) must have
    // the same endianness according to Silky 9's design. So, the code above
    // should and must always work.
  if (mpiErr) {
    MPI_TYPE_DEFINE_DEBUG_INFO("Word", "MPI_Type_dup", mpiErr);
    return Err_COMM_INIT;
  }
  mpiErr = MPI_Type_commit(&MPI_Word);
  if (mpiErr) {
    MPI_TYPE_COMMIT_DEBUG_INFO("Word", mpiErr);
    return Err_COMM_INIT;
  }

  //// Signal

  mpiErr = MPI_Type_dup(MPI_UINT64_T, &MPI_Signal);
  if (mpiErr) {
    MPI_TYPE_DEFINE_DEBUG_INFO("Signal", "MPI_Type_dup", mpiErr);
    return Err_COMM_INIT;
  }
  mpiErr = MPI_Type_commit(&MPI_Signal);
  if (mpiErr) {
    MPI_TYPE_COMMIT_DEBUG_INFO("Signal", mpiErr);
    return Err_COMM_INIT;
  }

  //// Token

  assert(2 * sizeof(Word) == sizeof(Token));
  mpiErr = MPI_Type_contiguous(2, MPI_Word, &MPI_Token);
  if (mpiErr) {
    MPI_TYPE_DEFINE_DEBUG_INFO("Token", "MPI_Type_contiguous", mpiErr);
    return Err_COMM_INIT;
  }
  mpiErr = MPI_Type_commit(&MPI_Token);
  if (mpiErr) {
    MPI_TYPE_COMMIT_DEBUG_INFO("Token", mpiErr);
    return Err_COMM_INIT;
  }

  //// NodeUpdate

  assert(4 * sizeof(Word) == sizeof(NodeUpdate));
  mpiErr = MPI_Type_contiguous(4, MPI_Word, &MPI_NodeUpdate);
  if (mpiErr) {
    MPI_TYPE_DEFINE_DEBUG_INFO("NodeUpdate", "MPI_Type_contiguous", mpiErr);
    return Err_COMM_INIT;
  }
  mpiErr = MPI_Type_commit(&MPI_NodeUpdate);
  if (mpiErr) {
    MPI_TYPE_COMMIT_DEBUG_INFO("NodeUpdate", mpiErr);
    return Err_COMM_INIT;
  }

  return Err_OK;
}


Err
comm_final()
{
  int mpiErr = MPI_Finalize();
  if (mpiErr) {
    MPI_DEBUG_INFO(
        "Failed to finalize MPI library.",
        "MPI_Finalize", mpiErr);
    return Err_COMM_FINAL;
  }

  return Err_OK;
}

#undef MPI_TYPE_DEFINE_DEBUG_INFO
#undef MPI_TYPE_COMMIT_DEBUG_INFO


#define MPI_SEND_DEBUG_INFO(type, mpiErr) \
    MPI_DEBUG_INFO( \
        "Failed to send " type " type value.", \
        "MPI_Send", \
        (mpiErr))


Err
comm_sendMessage(const Message message)
{
  int mpiErr = MPI_SUCCESS;

  switch (message.tag) {
  case MessageTag_TOKEN:
    {
      ProcId destProcId = ProcId_DUMMY;
      Err err = calcDestProcId(Message_getToken(message).dest, &destProcId);
      if (err) {
        DEBUG_INFO("Failed to calculate a destination processor ID from "
                      "a token's destination node ID.");
        return Err_COMM_SEND;
      }
      mpiErr = MPI_Send(&message.token, 1, MPI_Token, destProcId,
                        MessageTag_TOKEN, MPI_COMM_WORLD);
      if (mpiErr) {
        MPI_SEND_DEBUG_INFO("Token", mpiErr);
        return Err_COMM_SEND;
      }
    }
    break;
  case MessageTag_NODE_UPDATE:
    {
      ProcId destProcId = ProcId_DUMMY;
      Err err = calcDestProcId(Message_getNodeUpdate(message).nodeId,
                               &destProcId);
      if (err) {
        DEBUG_INFO("Failed to calculate a destination processor ID from "
                      "a node's node ID.");
        return Err_COMM_SEND;
      }
      mpiErr = MPI_Send(&message.nodeUpdate, 1, MPI_NodeUpdate, destProcId,
                        MessageTag_NODE_UPDATE, MPI_COMM_WORLD);
      if (mpiErr) {
        MPI_SEND_DEBUG_INFO("NodeUpdate", mpiErr);
        return Err_COMM_SEND;
      }
    }
    break;
  case MessageTag_SIGNAL:
    {
      Int numOfProcs = Int_DUMMY;
      Err err = getNumOfProcs(&numOfProcs);
      if (err) {
        DEBUG_INFO("Failed to get number of all processors.");
        return Err_COMM_NUM_OF_PROCS;
      }

      for (Int destProcId = 0; destProcId < numOfProcs; destProcId++) {
        mpiErr = MPI_Send(&message.signal, 1, MPI_Signal, destProcId,
                          MessageTag_SIGNAL, MPI_COMM_WORLD);
        if (mpiErr) {
          MPI_SEND_DEBUG_INFO("Signal", mpiErr);
          return Err_COMM_SEND;
        }
      }
    }
    break;
  default:
    DEBUG_INFO("Unknown message tag detected in a sent message. "
                  "(message tag: %d)", message.tag);
    return Err_COMM_TAG;
  }

  return Err_OK;
}

#undef MPI_SEND_DEBUG_INFO


#define MPI_RECV_DEBUG_INFO(type, mpiErr) \
    MPI_DEBUG_INFO( \
        "Failed to receive " type " type value.", \
        "MPI_Recv", \
        (mpiErr))

int
wrapperOf_MPI_Recv(void * const buff, MPI_Datatype datatype,
           MPI_Status * const status)
{
  return MPI_Recv(buff, 1, datatype, MPI_ANY_SOURCE, MPI_ANY_TAG,
                  MPI_COMM_WORLD, status);
}


Err
comm_receiveMessage(Message * const message)
{
  MPI_Status status;

  int mpiErr = MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  if (mpiErr) {
    MPI_DEBUG_INFO("Failed to get status of the top message.",
                      "MPI_Probe", mpiErr);
    return Err_COMM_RECEIVE;
  }

  switch (status.MPI_TAG) {
  case MessageTag_TOKEN:
    {
      Token token = Token_DUMMY;
      mpiErr = wrapperOf_MPI_Recv(&token, MPI_Token, &status);
      if (mpiErr) {
        MPI_RECV_DEBUG_INFO("Token", mpiErr);
        return Err_COMM_RECEIVE;
      }
      *message = Message_ofToken(token);
    }
    break;
  case MessageTag_NODE_UPDATE:
    {
      NodeUpdate nodeUpdate = NodeUpdate_DUMMY;
      mpiErr = wrapperOf_MPI_Recv(&nodeUpdate, MPI_NodeUpdate, &status);
      if (mpiErr) {
        MPI_RECV_DEBUG_INFO("NodeUpdate", mpiErr);
        return Err_COMM_RECEIVE;
      }
      *message = Message_ofNodeUpdate(nodeUpdate);
    }
    break;
  case MessageTag_SIGNAL:
    {
      Int signal = Int_DUMMY;
      mpiErr = wrapperOf_MPI_Recv(&signal, MPI_Signal, &status);
      if (mpiErr) {
        MPI_RECV_DEBUG_INFO("Signal", mpiErr);
        return Err_COMM_RECEIVE;
      }
      *message = Message_ofSignal(signal);
    }
    break;
  default:
    DEBUG_INFO("Unknown message tag detected in a received message. "
                  "(message tag: %d)", status.MPI_TAG);
    return Err_COMM_TAG;
  }

  return Err_OK;
}

#undef MPI_RECV_DEBUG_INFO


Err
comm_amIMaster(bool *answer)
{
  int myRank = int_DUMMY;

  int mpiErr = MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  if (mpiErr) {
    MPI_DEBUG_INFO("Failed to get the rank of the current process.",
                      "MPI_Comm_rank", mpiErr);
    return Err_COMM_PROC_ID;
  }

  *answer = (myRank == 0);

  return Err_OK;
}

#undef MPI_DEBUG_INFO
