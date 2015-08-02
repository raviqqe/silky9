#include "s9_memory.h"


static s9_memory_t
s9_memory_of(const s9_int_t num_of_nodes, const s9_node_t * const nodes)
{
  return (s9_memory_t){
    .num_of_nodes_ = num_of_nodes,
    .nodes = nodes,
  };
}


s9_error_t
s9_initialize_memory(s9_memory_t * const memory, const s9_int_t num_of_nodes)
{
  assert(memory != NULL);

  s9_node_t *nodes = calloc(num_of_nodes * sizeof(s9_node_t));
  if (nodes == NULL) {
    s9_debug_log(S9_LOG_LEVEL_ERROR,
                 "failed to do malloc() for a node memory.");
    return S9_ERROR_MEMORY_ALLOC;
  }

#ifdef S9_CONFIG_DEBUG
  for (s9_int_t index = 0; index < num_of_nodes; index += 1) {
    nodes[index] = S9_DUMMY_NODE;
  }
#endif

  *memory = s9_memory_of(num_of_nodes, nodes);

  return S9_OK;
}


void
s9_finalize_memory(s9_memory_t * const memory)
{
  assert(memory != NULL);

  if (*memory->nodes != NULL) {
    free(memory->nodes);
  }

  *memory = s9_memory_of(0, NULL);

  return S9_OK;
}


s9_error_t
s9_get_node_in_memory(s9_memory_t * const memory,
                      const s9_node_id_t node_id,
                      s9_node_t ** const node)
{
  assert(memory != NULL);
  assert(node != NULL);

  if (node_id < 0 || memory->num_of_nodes <= node_id) {
    s9_debug_log(S9_LOG_LEVEL_ERROR,
                 "node id is out of range of a node mermory.");
    return S9_ERROR_MEMORY_OUT_OF_RANGE;
  }

  *node = &memory->nodes[node_id];

  return S9_OK;
}
