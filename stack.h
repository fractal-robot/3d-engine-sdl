#ifndef STACK_H
#define STACK_H

#define MAX_SIZE 100

typedef struct {
  int *array;
  int capacity;
  int top;
} Stack;

Stack *createStack(int capacity);
int isFull(Stack *stack);
void push(Stack *stack, int item);
int pop(Stack *stack);
void destroyStack(Stack *stack);
void append(Stack *appendTo, Stack *appendFrom, int capacity);

#endif
