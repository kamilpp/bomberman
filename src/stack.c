#include "stack.h"

void push(stack **stos, int val_x, int val_y)
{
    stack *tmp = (stack *) malloc(sizeof(stack));
    tmp->val_x = val_x;
    tmp->val_y = val_y;
    tmp->next = *stos;
    *stos = tmp;
}

stack top(stack *stos)
{
    return *stos;
}

int empty(stack *stos)
{
    if (stos) return 0;
    return 1;
}

void pop(stack **stos)
{
    *stos = (*stos)->next;
}

