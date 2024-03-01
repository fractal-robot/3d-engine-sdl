#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Stack *createStack(int capacity) {
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  (capacity > 0) ? (stack->capacity = capacity) : (stack->capacity = 1);
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

void freeStack(Stack *stack) {
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
  if (isEmpty(appendFrom)) {
    freeStack(appendFrom);
    return;
  }

  appendTo->items =
      realloc(appendTo->items,
              (appendTo->capacity + appendFrom->capacity) * sizeof(void *));
  if (appendTo->items == NULL) {
    // Handle memory allocation failure
    printf("Memory allocation failed.\n");
    freeStack(appendFrom);
    return;
  }

  memcpy(appendTo->items + appendTo->top + 1, appendFrom->items,
         (appendFrom->top + 1) * sizeof(void *));
  appendTo->top += appendFrom->top + 1;
  appendTo->capacity += appendFrom->capacity;

  freeStack(appendFrom);
}

void *getStackItem(Stack *stack, int index) {
  if (index < 0 || index > stack->top) {
    printf("Invalid index fed to stack.");
    return NULL;
  }
  return stack->items[index];
}
