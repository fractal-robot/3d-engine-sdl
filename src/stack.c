#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Stack *createStack(int capacity) {
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  stack->capacity = capacity;
  stack->items = (void **)malloc(stack->capacity * sizeof(void *));
  stack->top = -1;
  return stack;
}

int isEmpty(Stack *stack) { return stack->top == -1; }

void push(Stack *stack, void *data) {
  if (stack->top == stack->capacity - 1) {
    stack->capacity *= 2;
    stack->items =
        (void **)realloc(stack->items, stack->capacity * sizeof(void *));
  }
  stack->items[++stack->top] = data;
}

void pushLocal(Stack *stack, void *data, size_t size) {
  if (stack->top == stack->capacity - 1) {
    stack->capacity *= 2;
    stack->items =
        (void **)realloc(stack->items, stack->capacity * sizeof(void *));
  }
  void *newData = malloc(size);
  memcpy(newData, data, size);
  stack->items[++stack->top] = newData;
}

void *pop(Stack *stack) {
  if (isEmpty(stack)) {
    printf("Stack underflow.\n");
    return NULL;
  }
  return stack->items[stack->top--];
}

void deleteStack(Stack *stack) {
  free(stack->items);
  free(stack);
}

Stack *initializeStack(void *array, int arraySize) {
  Stack *stack = (Stack *)malloc(sizeof(Stack));

  stack->items = array;
  stack->top = arraySize - 1;
  stack->capacity = arraySize;

  return stack;
}

void append(Stack *appendTo, Stack *appendFrom) {
  void *newArray =
      realloc(appendTo->items,
              ((appendTo->capacity + appendFrom->capacity)) * sizeof(void *));
  appendTo->capacity = appendTo->capacity + appendFrom->capacity;
  appendTo->items = newArray;
  printf("Append, reallocating to size: %i.\n", appendTo->capacity);
  for (int i = 0; i <= appendFrom->top; ++i) {
    push(appendTo, pop(appendFrom));
  }

  deleteStack(appendFrom);
}

void *getStackItem(Stack *stack, int index) {
  if (index < 0 || index > stack->top) {
    printf("Invalid index fed to stack.");
    return NULL;
  }
  return stack->items[index];
}
