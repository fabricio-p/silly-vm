#include "env.h"
#include "macros.h"
#include "internal.h"

SStatus AllocChain_alloc(AllocNode **head, Usize size,
                         voidptr *buffer_p)
{
  SStatus status = SILLY_E_OK;
  Usize total_size = offsetof(AllocNode, data) + size;
  LOG_EVAL(total_size, "lu");
  AllocNode *node = malloc(total_size);
  if (node != NULL)
  {
    memset(node, '\0', total_size);
    node->size = size;
    AllocNode *const old_head = *head;
    *head = node;
    node->next = old_head;
    node->prev = NULL;
    if (old_head != NULL)
    {
      old_head->prev = node;
    }
    if (buffer_p != NULL)
    {
      *buffer_p = node->data;
    }
  } else
  {
    status = SILLY_E_OOM;
  }
  return status;
}

void AllocChain_free(AllocNode **current_p)
{
  if (current_p != NULL && *current_p != NULL)
  {
    AllocNode *current = *current_p;
    if (current->prev != NULL)
    {
      current->prev->next = current->next;
    }
    if (current->next != NULL)
    {
      current->next->prev = current->prev;
    }
    free(current);
    *current_p = NULL;
  }
}

void SProcess_create(SEnv *env, SProcess *proc, SProcess *parent,
                     SProcInfo *info, SModule *mod)
{
  proc->pid = SProcesses_len(env->all_processes);
  proc->init_func = &(mod->functions[info->init_func_idx]);
  proc->finish_func = &(mod->functions[info->finish_func_idx]);
  proc->listener = &(mod->functions[info->listener_func_idx]);
  proc->children = NULL;
  proc->parent = parent;
#ifdef SILLY_PROCESS_TRACING
  proc->mod = mod;
  proc->info = info;
#endif

  proc->stack.current_frame = &(proc->stack.frames[0]);
  proc->memory = NULL;
  proc->memory_size = 0;
  proc->exit_code = 0;
}

SStatus SProcess_spawn(SEnv *env, SProcess *proc, SMessage const *message)
{
  SStatus status = SILLY_E_OK;
  

//catch:
  return status;
}
