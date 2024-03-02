#ifndef STACK_H
#define STACK_H

#include <stddef.h>

typedef struct {
  void **items;
  int top;
  int capacity;
} Stack;

Stack *createStack(int capacity);
int isStackEmpty(Stack *stack);
void *pop(Stack *stack);
void freeStack(Stack *stack);
Stack *initializeStack(void *array, int arraySize);
void append(Stack *appendTo, Stack *appendFrom);
void push(Stack *stack, void *data);
void pushLocal(Stack *stack, void *data, size_t size);
void *getStackItem(Stack *stack, int index);

#endif
