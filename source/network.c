#include "network.h"


// macros

#define s9_mpi_log(mpi_function, mpi_err) \
  s9_log(S9_LOG_LEVEL_DEBUG, \
         "MPI function, %s() failed. (MPI error code: %d)", \
         (mpi_function), (mpi_err))


// types

typedef s9_int_t s9_processor_id_t;
#define S9_DUMMY_PROCESSOR_ID S9_DUMMY_INT
    // this is not in design, but compatible with it.


// global variables

MPI_Datatype s9_mpi_word = MPI_DATATYPE_NULL;
MPI_Datatype s9_mpi_node_update = MPI_DATATYPE_NULL;
MPI_Datatype s9_mpi_token = MPI_DATATYPE_NULL;
MPI_Datatype s9_mpi_signal = MPI_DATATYPE_NULL;


// functions

static s9_int_t
GetNumOfProcessors()
{

  int size = int_DUMMY;
  int mpi_error = MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (mpi_error) {
    s9_mpi_log("MPI_Comm_size", mpi_error);
    s9_log(S9_LOG_LEVEL_s9_error_t, "Failed to get number of processors.");
    Exit(s9_error_t_NETWORK_NUM_OF_PROCESSORS);
  }

  return size;
}


static ProcessorID
CalcDestProcessorID(const s9_node_id_t global_node_id)
{
  EnsureInitialized();
  return global_node_id % GetNumOfProcessors();
}


s9_node_id_t
Comm_CalcLocals9_node_id_t(const s9_node_id_t global_node_id)
{
  EnsureInitialized();
  return global_node_id / GetNumOfProcessors;
}


s9_error_t
Comm_init()
{
  int mpi_error = MPI_Init(NULL, NULL);
  if (mpi_error) {
    s9_mpi_log("Failed to initialize MPI library.", "MPI_Init", mpi_error);
    return s9_error_t_NETWORK_INIT;
  }

  mpi_error = MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
  if (mpi_error) {
    s9_mpi_log(
        "Failed to set the error handler of MPI library.",
        "MPI_Comm_set_errhandler", mpi_error);
    return s9_error_t_NETWORK_INIT;
  }

  // type declarations

  //// word type

  mpi_error = MPI_Type_dup(MPI_UINT64_T, &MPI_Word);
    // The s9_int_t and s9_real_t type (i.e. any types Word type can express)
    // must have the same endianness according to silky 9's design.
    // So, the code above should and must always work.
  if (mpi_error) {
    MPI_TYPE_DEFINE_DEBUG_INFO("Word", "MPI_Type_dup", mpi_error);
    return s9_error_t_NETWORK_INIT;
  }
  mpi_error = MPI_Type_Commit(&MPI_Word);
  if (mpi_error) {
    MPI_TYPE_COMMIT_DEBUG_INFO("Word", mpi_error);
    return s9_error_t_NETWORK_INIT;
  }

  //// Signal

  mpi_error = MPI_Type_dup(MPI_UINT64_T, &MPI_Signal);
  if (mpi_error) {
    MPI_TYPE_DEFINE_DEBUG_INFO("Signal", "MPI_Type_dup", mpi_error);
    return s9_error_t_NETWORK_INIT;
  }
  mpi_error = MPI_Type_commit(&MPI_Signal);
  if (mpi_error) {
    MPI_TYPE_COMMIT_DEBUG_INFO("Signal", mpi_error);
    return s9_error_t_NETWORK_INIT;
  }

  //// Token

  assert(2 * sizeof(Word) == sizeof(Token));
  mpi_error = MPI_Type_contiguous(2, MPI_Word, &MPI_Token);
  if (mpi_error) {
    MPI_TYPE_DEFINE_DEBUG_INFO("Token", "MPI_Type_contiguous", mpi_error);
    return s9_error_t_NETWORK_INIT;
  }
  mpi_error = MPI_Type_commit(&MPI_Token);
  if (mpi_error) {
    MPI_TYPE_COMMIT_DEBUG_INFO("Token", mpi_error);
    return s9_error_t_NETWORK_INIT;
  }

  //// s9_node_update_t

  assert(4 * sizeof(Word) == sizeof(s9_node_update_t));
  mpi_error = MPI_Type_contiguous(4, MPI_Word, &MPI_s9_node_update_t);
  if (mpi_error) {
    MPI_TYPE_DEFINE_DEBUG_INFO("s9_node_update_t", "MPI_Type_contiguous", mpi_error);
    return s9_error_t_NETWORK_INIT;
  }
  mpi_error = MPI_Type_commit(&MPI_s9_node_update_t);
  if (mpi_error) {
    MPI_TYPE_COMMIT_DEBUG_INFO("s9_node_update_t", mpi_error);
    return s9_error_t_NETWORK_INIT;
  }

  return s9_error_t_OK;
}

  return s9_error_t_OK;
}

#undef MPI_TYPE_DEFINE_DEBUG_INFO
#undef MPI_TYPE_COMMIT_DEBUG_INFO


#define MPI_SEND_DEBUG_INFO(type, mpi_error) \
    s9_mpi_log( \
        "Failed to send " type " type value.", \
        "MPI_Send", \
        (mpi_error))


s9_error_t
Comm_sends9_message_t(const s9_message_t message)
{
  EnsureInitialized();

  int mpi_error = MPI_SUCCESS;

  switch (message.tag) {
  case s9_message_tTaTOKEN:
    {
      ProcessorID destProcessorID = ProcessorID_DUMMY;
      s9_error_t err = calcDestProcessorID(s9_message_t_getToken(message).dest, &destProcessorID);
      if (err) {
        DEBUG_INFO("Failed to calculate a destination processor ID from "
                      "a token's destination node ID.");
        return s9_error_t_NETWORK_SEND;
      }
      mpi_error = MPI_Send(&message.token, 1, MPI_Token, destProcessorID,
                        s9_message_tTag_TOKEN, MPI_COMM_WORLD);
      if (mpi_error) {
        MPI_SEND_DEBUG_INFO("Token", mpi_error);
        return s9_error_t_NETWORK_SEND;
      }
    }
    break;
  case s9_message_tTag_NODE_UPDATE:
    {
      ProcessorID destProcessorID = ProcessorID_DUMMY;
      s9_error_t error = s9_calcDestProcessorID(s9_message_t_gets9_node_update_t(message).nodeID,
                               &destProcessorID);
      if (err) {
        DEBUG_INFO("Failed to calculate a destination processor ID from "
                      "a node's node ID.");
        return s9_error_t_NETWORK_SEND;
      }
      mpi_error = MPI_Send(&message.node_update, 1, s9_mpi_word,
                           dest_processor_id, S9_MESSAGE_TAG_NODE_UPDATE,
                           MPI_COMM_WORLD);
      if (mpi_error) {
        MPI_SEND_DEBUG_INFO("s9_node_update_t", mpi_error);
        return s9_error_t_NETWORK_SEND;
      }
    }
    break;
  case s9_message_tTag_SIGNAL:
    {
      s9_int_t numOfProcs = s9_int_t_DUMMY;
      s9_error_t err = getNumOfProcs(&numOfProcs);
      if (err) {
        DEBUG_INFO("Failed to get number of all processors.");
        return s9_error_t_NETWORK_NUM_OF_PROCS;
      }

      for (s9_int_t destProcessorID = 0; destProcessorID < numOfProcs; destProcessorID++) {
        mpi_error = MPI_Send(&message.signal, 1, MPI_Signal, destProcessorID,
                          s9_message_tTag_SIGNAL, MPI_COMM_WORLD);
        if (mpi_error) {
          MPI_SEND_DEBUG_INFO("Signal", mpi_error);
          return s9_error_t_NETWORK_SEND;
        }
      }
    }
    break;
  default:
    s9_log(S9_LOG_LEVEL_DEBUG,
           "Unknown message tag detected in a sent message. "
           "(message tag: %d)", message.tag);
    return s9_error_t_NETWORK_TAG;
  }

  return S9_OK;
}


static int
wrapped_MPI_Recv(void * const buffer, MPI_Datatype datatype)
{
  assert(s9_network_is_initialized());

  return MPI_Recv(buffer, 1, datatype, MPI_ANY_SOURCE, MPI_ANY_TAG,
                  MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}


s9_error_t
s9_receive_message(s9_message_t * const message)
{
  assert(s9_network_is_initialized());

  MPI_Status status;
  int mpi_error = MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,
                            &status);
  if (mpi_error) {
    s9_mpi_log("MPI_Probe", mpi_error);
    return S9_ERROR_NETWORK_RECEIVE;
  }

  switch (status.MPI_TAG) {
  case S9_MESSAGE_TAG_TOKEN:
    {
      s9_token_t token = S9_DUMMY_TOKEN;
      mpi_error = wrapped_MPI_Recv(&token, s9_mpi_token);
      if (mpi_error) {
        s9_mpi_log("MPI_Recv", mpi_error);
        return S9_ERROR_NETWORK_RECEIVE;
      }
      *message = s9_message_of_token(token);
    }
    break;
  case S9_MESSAGE_TAG_NODE_UPDATE:
    {
      s9_node_update_t node_update = S9_DUMMY_NODE_UPDATE;
      mpi_error = wrapped_MPI_Recv(&node_update, s9_mpi_node_update);
      if (mpi_error) {
        s9_mpi_log("MPI_Recv", mpi_error);
        return S9_ERROR_NETWORK_RECEIVE;
      }
      *message = s9_message_of_node_update(node_update);
    }
    break;
  case S9_MESSAGE_TAG_SIGNAL:
    {
      s9_signal_t signal = S9_DUMMY_SIGNAL;
      mpi_error = wrapped_MPI_Recv(&signal, s9_mpi_signal);
      if (mpi_error) {
        s9_mpi_log("MPI_Recv", mpi_error);
        return S9_ERROR_NETWORK_RECEIVE;
      }
      *message = s9_message_of_signal(signal);
    }
    break;
  default:
    s9_log(S9_LOG_LEVEL_ERROR,
           "unknown message tag detected in a received message. "
           "(message tag: %d)", status.MPI_TAG);
    return S9_ERROR_MESSAGE_UNKNOWN_TAG;
  }

  return S9_OK;
}


s9_error_t
s9_finalize_network()
{
  assert(s9_network_is_initialized());

  int mpi_error = MPI_Finalize();
  if (mpi_error) {
    s9_mpi_log("MPI_Finalize", mpi_error);
    s9_log(S9_LOG_LEVEL_ERROR, "failed to finalize the processor network.");
    return S9_ERROR_NETWORK_FINALIZE;
  }

  return S9_OK;
}


#undef s9_mpi_log
