//funkcje jak w STL-u
//push i pop wymagają przekazania nazwy stosu przez referencję

#ifndef stack_h
#define stack_h

#include "header.h"

struct stack                                    //definicja struktury stosu
{
  struct stack *next;
  int val_x, val_y;
};

typedef struct stack stack;

void push(stack **stos, int val_x, int val_y);  //funkcja wrzucajaca strukturke na stos
void pop(stack **stos);                         //usuwanie ze stosu
stack top(stack *stos);                         //pobranie struktury ze stosu
int empty(stack *stos);                         //usuniecie struktury ze stosu

#endif
