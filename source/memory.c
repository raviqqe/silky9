#include "memory.h"


static s9_memory_t
s9_memory_of(const s9_int_t capacity, const s9_node_t * const nodes)
{
  return (s9_memory_t){
    .capacity = capacity,
    .nodes = nodes,
  };
}


s9_error_t
s9_initialize_memory(s9_memory_t * const memory)
{
  assert(memory != NULL);

  *memory = s9_memory_of(CONFIG_INITIAL_MEMORY_CAPACITY,
                         malloc(CONFIG_INITIAL_MEMORY_CAPACITY
                                * sizeof(s9_node_t)));

  if (memory->nodes == NULL) {
    s9_log(S9_LOG_LEVEL_ERROR, "Failed to do malloc() for a node memory.");
    return S9_ERROR_MEMORY_ALLOC;
  }

#ifdef S9_CONFIG_DEBUG
  for (s9_int_t index = 0; index < memory->capacity; index += 1) {
    memory->nodes[index] = S9_DUMMY_NODE;
  }
#endif

  return S9_ERROR_OK;
}


s9_error_t
s9_memory_t_final(s9_memory_t * const memory)
{
  assert(memory != NULL);

  free(memory->nodes);

  return S9_ERROR_OK;
}


s9_error_t
s9_node_in_memory(s9_memory_t * const memory, const s9_node_tId node_id,
                       s9_node_t ** const node)
{
  if (node_id >= memory->capacity) {
    DEBUG_INFO("s9_node_t id is out of range of a node mermory.");
    return s9_error_t_MEMORY_OUT_OF_RANGE;
  }

  *node = &memory->nodes[node_id];

  return S9_ERROR_OK;
}


static s9_int_t
s9_calc_new_capacity(const s9_int_t old_capacity)
{
  return 2 * old_capacity;
}


s9_error_t
s9_append_node_to_memory(s9_memory_t * const memory,
                         const s9_node_t node)
{
  if ((node_id > memory->capacity - 1)
      && (s9_calc_new_capacity(memory->capacity)
      <= CONFIG_MAX_MEMORY_CAPACITY)) {
    s9_int_t old_capacity = memory->capacity;

    *memory = (s9_memory_t){
      .capacity = s9_calc_new_capacity(memory->capacity),
      .nodes = realloc(memory->nodes, sizeof(s9_node_t)
                       * s9_calc_new_capacity(memory->capacity)),
    };
    if (memory->nodes == NULL) {
      DEBUG_INFO("Failed to do realloc() for a node memory.");
      return s9_error_t_MEMORY_ALLOC;
    }

    for (int index = old_capacity; index < memory->capacity; index += 1) {
      memory->nodes[index] = S9_DUMMY_NODE;
    }
  }

  memory->nodes[node_id] = node;

  return ;
}
