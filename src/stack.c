#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_RESIZE_FACTOR 2

Stack *createStack(int capacity) {
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  stack->capacity = (capacity > 0) ? capacity : 1;
  stack->items = (void **)malloc(stack->capacity * sizeof(void *));
  stack->top = -1;
  return stack;
}

int isStackEmpty(Stack *stack) { return stack->top == -1; }

void push(Stack *stack, void *data) {
  if (stack->top == stack->capacity - 1) {
    stack->capacity += STACK_RESIZE_FACTOR;
    stack->items =
        (void **)realloc(stack->items, stack->capacity * sizeof(void *));
  }
  stack->items[++stack->top] = data;
}

void pushLocal(Stack *stack, void *data, size_t size) {
  if (stack->top == stack->capacity - 1) {
    stack->capacity += STACK_RESIZE_FACTOR;
    stack->items =
        (void **)realloc(stack->items, stack->capacity * sizeof(void *));
  }
  void *newData = malloc(size);
  memcpy(newData, data, size);
  stack->items[++stack->top] = newData;
}

void *pop(Stack *stack) {
  if (isStackEmpty(stack)) {
    printf("Stack underflow.\n");
    return NULL;
  }
  void *data = stack->items[stack->top];
  stack->top--;
  return data;
}

Stack *initializeStack(void *array, int arraySize) {
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  stack->items = array;
  stack->top = arraySize - 1;
  stack->capacity = arraySize;
  return stack;
}

void append(Stack *appendTo, Stack *appendFrom) {
  if (isStackEmpty(appendFrom)) {
    free(appendFrom->items);
    free(appendFrom);
    return;
  }

  appendTo->capacity += appendFrom->capacity;
  appendTo->items =
      realloc(appendTo->items, appendTo->capacity * sizeof(void *));
  if (appendTo->items == NULL) {
    // Handle memory allocation failure
    printf("Memory allocation failed.\n");
    free(appendFrom->items);
    free(appendFrom);
    return;
  }

  memcpy(appendTo->items + appendTo->top + 1, appendFrom->items,
         (appendFrom->top + 1) * sizeof(void *));
  appendTo->top += appendFrom->top + 1;

  free(appendFrom->items);
  free(appendFrom);
}

void *getStackItem(Stack *stack, int index) {
  if (index < 0 || index > stack->top) {
    printf("Invalid index fed to stack.");
    return NULL;
  }
  return stack->items[index];
}

void freeStack(Stack *stack) {
  free(stack->items);
  free(stack);
}
