#include "env.h"
#include "macros.h"

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
