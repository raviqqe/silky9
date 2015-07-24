#include "Comm.h"


// Macros

#define MPI_DEBUG_LOG(message, mpi_func_name, mpi_err) \
    DEBUG_LOG("MPI function, %s() failed. (MPI error code: %d)", \
              (message), (mpi_func_name), (mpi_err))


// Types

typedef Int ProcessorID; // This is not in design, but compatible with it.
#define ProcessorID_DUMMY Int_DUMMY


// Global variables

MPI_Datatype g_MPI_Word = MPI_DATATYPE_NULL;
MPI_Datatype g_MPI_NodeUpdate = MPI_DATATYPE_NULL;
MPI_Datatype g_MPI_Token = MPI_DATATYPE_NULL;
MPI_Datatype g_MPI_Signal = MPI_DATATYPE_NULL;


// Functions

static void
EnsureInitialized()
{

}


static Int
GetNumOfProcessors()
{
  EnsureInitialized();

  int size = int_DUMMY;
  int mpi_error = MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (mpi_error) {
    MPI_DEBUG_LOG("MPI_Comm_size", mpi_error);
    Log(LogType_Error, "Failed to get number of processors.");
    Exit(Error_COMM_NUM_OF_PROCESSORS);
  }

  return size;
}


static ProcessorID
CalcDestProcessorID(const NodeID global_node_id)
{
  EnsureInitialized();
  return global_node_id % GetNumOfProcessors();
}


NodeID
Comm_CalcLocalNodeID(const NodeID global_node_id)
{
  EnsureInitialized();
  return global_node_id / GetNumOfProcessors;
}


Err
Comm_init()
{
  int mpi_error = MPI_Init(NULL, NULL);
  if (mpi_error) {
    MPI_DEBUG_LOG("Failed to initialize MPI library.", "MPI_Init", mpi_error);
    return Err_COMM_INIT;
  }

  mpi_error = MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
  if (mpi_error) {
    MPI_DEBUG_LOG(
        "Failed to set the error handler of MPI library.",
        "MPI_Comm_set_errhandler", mpi_error);
    return Err_COMM_INIT;
  }

  // Type declarations

  //// Word

  mpi_error = MPI_Type_dup(MPI_UINT64_T, &MPI_Word);
    // The Int and Real type (i.e. any types Word type can express) must have
    // the same endianness according to Silky 9's design. So, the code above
    // should and must always work.
  if (mpi_error) {
    MPI_TYPE_DEFINE_DEBUG_INFO("Word", "MPI_Type_dup", mpi_error);
    return Err_COMM_INIT;
  }
  mpi_error = MPI_Type_Commit(&MPI_Word);
  if (mpi_error) {
    MPI_TYPE_COMMIT_DEBUG_INFO("Word", mpi_error);
    return Err_COMM_INIT;
  }

  //// Signal

  mpi_error = MPI_Type_dup(MPI_UINT64_T, &MPI_Signal);
  if (mpi_error) {
    MPI_TYPE_DEFINE_DEBUG_INFO("Signal", "MPI_Type_dup", mpi_error);
    return Err_COMM_INIT;
  }
  mpi_error = MPI_Type_commit(&MPI_Signal);
  if (mpi_error) {
    MPI_TYPE_COMMIT_DEBUG_INFO("Signal", mpi_error);
    return Err_COMM_INIT;
  }

  //// Token

  assert(2 * sizeof(Word) == sizeof(Token));
  mpi_error = MPI_Type_contiguous(2, MPI_Word, &MPI_Token);
  if (mpi_error) {
    MPI_TYPE_DEFINE_DEBUG_INFO("Token", "MPI_Type_contiguous", mpi_error);
    return Err_COMM_INIT;
  }
  mpi_error = MPI_Type_commit(&MPI_Token);
  if (mpi_error) {
    MPI_TYPE_COMMIT_DEBUG_INFO("Token", mpi_error);
    return Err_COMM_INIT;
  }

  //// NodeUpdate

  assert(4 * sizeof(Word) == sizeof(NodeUpdate));
  mpi_error = MPI_Type_contiguous(4, MPI_Word, &MPI_NodeUpdate);
  if (mpi_error) {
    MPI_TYPE_DEFINE_DEBUG_INFO("NodeUpdate", "MPI_Type_contiguous", mpi_error);
    return Err_COMM_INIT;
  }
  mpi_error = MPI_Type_commit(&MPI_NodeUpdate);
  if (mpi_error) {
    MPI_TYPE_COMMIT_DEBUG_INFO("NodeUpdate", mpi_error);
    return Err_COMM_INIT;
  }

  return Err_OK;
}

  return Err_OK;
}

#undef MPI_TYPE_DEFINE_DEBUG_INFO
#undef MPI_TYPE_COMMIT_DEBUG_INFO


#define MPI_SEND_DEBUG_INFO(type, mpi_error) \
    MPI_DEBUG_LOG( \
        "Failed to send " type " type value.", \
        "MPI_Send", \
        (mpi_error))


Err
Comm_sendMessage(const Message message)
{
  EnsureInitialized();

  int mpi_error = MPI_SUCCESS;

  switch (message.tag) {
  case MessageTag_TOKEN:
    {
      ProcessorID destProcessorID = ProcessorID_DUMMY;
      Err err = calcDestProcessorID(Message_getToken(message).dest, &destProcessorID);
      if (err) {
        DEBUG_INFO("Failed to calculate a destination processor ID from "
                      "a token's destination node ID.");
        return Err_COMM_SEND;
      }
      mpi_error = MPI_Send(&message.token, 1, MPI_Token, destProcessorID,
                        MessageTag_TOKEN, MPI_COMM_WORLD);
      if (mpi_error) {
        MPI_SEND_DEBUG_INFO("Token", mpi_error);
        return Err_COMM_SEND;
      }
    }
    break;
  case MessageTag_NODE_UPDATE:
    {
      ProcessorID destProcessorID = ProcessorID_DUMMY;
      Err err = calcDestProcessorID(Message_getNodeUpdate(message).nodeID,
                               &destProcessorID);
      if (err) {
        DEBUG_INFO("Failed to calculate a destination processor ID from "
                      "a node's node ID.");
        return Err_COMM_SEND;
      }
      mpi_error = MPI_Send(&message.nodeUpdate, 1, MPI_NodeUpdate, destProcessorID,
                        MessageTag_NODE_UPDATE, MPI_COMM_WORLD);
      if (mpi_error) {
        MPI_SEND_DEBUG_INFO("NodeUpdate", mpi_error);
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

      for (Int destProcessorID = 0; destProcessorID < numOfProcs; destProcessorID++) {
        mpi_error = MPI_Send(&message.signal, 1, MPI_Signal, destProcessorID,
                          MessageTag_SIGNAL, MPI_COMM_WORLD);
        if (mpi_error) {
          MPI_SEND_DEBUG_INFO("Signal", mpi_error);
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


#define MPI_RECV_DEBUG_INFO(type, mpi_error) \
    MPI_DEBUG_LOG( \
        "Failed to receive " type " type value.", \
        "MPI_Recv", \
        (mpi_error))

int
WrapperOf_MPI_Recv(void * const buff, MPI_Datatype datatype,
           MPI_Status * const status)
{
  EnsureInitialized();

  return MPI_Recv(buff, 1, datatype, MPI_ANY_SOURCE, MPI_ANY_TAG,
                  MPI_COMM_WORLD, status);
}


Error
Comm_ReceiveMessage(Message * const message)
{
  EnsureInitialized();

  MPI_Status status;

  int mpi_error = MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
                            &status);
  if (mpi_error) {
    MPI_DEBUG_LOG("MPI_Probe", mpi_error);
    Exit(Err_COMM_RECEIVE);
  }

  switch (status.MPI_TAG) {
  case MessageTag_TOKEN:
    {
      Token token = Token_DUMMY;
      mpi_error = WrapperOf_MPI_Recv(&token, MPI_Token, &status);
      if (mpi_error) {
        MPI_DEBUG_INFO("Token", mpi_error);
        return Err_COMM_RECEIVE;
      }
      *message = Message_ofToken(token);
    }
    break;
  case MessageTag_NODE_UPDATE:
    {
      NodeUpdate nodeUpdate = NodeUpdate_DUMMY;
      mpi_error = WrapperOf_MPI_Recv(&nodeUpdate, MPI_NodeUpdate, &status);
      if (mpi_error) {
        MPI_DEBUG_INFO("NodeUpdate", mpi_error);
        return Err_COMM_RECEIVE;
      }
      *message = Message_ofNodeUpdate(nodeUpdate);
    }
    break;
  case MessageTag_SIGNAL:
    {
      Int signal = Int_DUMMY;
      mpi_error = WrapperOf_MPI_Recv(&signal, MPI_Signal, &status);
      if (mpi_error) {
        MPI_DEBUG_INFO("Signal", mpi_error);
        return Err_COMM_RECEIVE;
      }
      *message = Message_ofSignal(signal);
    }
    break;
  default:
    Log(LogType_ERROR,
        "Unknown message tag detected in a received message. "
        "(message tag: %d)", status.MPI_TAG);
    Exit(Error_COMM_UNKNOWN
  }

  return Error_OK;
}


bool
Comm_AmIMaster()
{
  EnsureInitialized();

  int my_rank = int_DUMMY;
  int mpi_error = MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  if (mpi_error) {
    MPI_DEBUG_LOG("MPI_Comm_rank", mpi_error);
    Log(LogType_ERROR, "Failed to get a processor ID.");
    Exit(Error_COMM_PROCESSOR_ID);
  }

  return my_rank == 0; // the first process's rank on MPI library
}


void
Exit(const Error error)
{
  EnsureInitialized();

  Error error_on_exit = Comm_SendMessage(Message_Of(Signal_SHUTDOWN));
  if (error_on_exit) {
    Log(LogType_ERROR,
        "Failed to shut down in place. You may need to kill other processes "
        "manually.");
    goto ExitWithError;
  }

  int mpi_error = MPI_Finalize();
  if (mpi_error) {
    MPI_DEBUG_LOG("MPI_Finalize", mpi_error);
    Log(LogType_ERROR, "Failed to finalize the processor network.");
    goto ExitWithError;
  }

  if (error == Error_OK) {
    exit(EXIT_SUCCESS);
  }

ExitWithError:
  Log(LogType_ERROR,
      "Shutting down because of error. (error code, %d)", error);
  exit(EXIT_FAILURE);
}


#undef MPI_DEBUG_LOG
