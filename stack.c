#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

Stack *createStack(int capacity) {
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  if (stack == NULL) {
    printf("Memory allocation for stack failed.\n");
    exit(EXIT_FAILURE);
  }
  stack->array = (int *)malloc(capacity * sizeof(int));
  if (stack->array == NULL) {
    printf("Memory allocation for stack failed.\n");
    exit(EXIT_FAILURE);
  }
  stack->capacity = capacity;
  stack->top = -1;
  return stack;
}

Stack *initializeStack(int *array, int arraySize) {
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  if (stack == NULL) {
    printf("Memory allocation for stack failed.\n");
    exit(EXIT_FAILURE);
  }
  stack->array = array;
  stack->top = arraySize - 1;
  stack->capacity = arraySize;

  return stack;
}

int isFull(Stack *stack) { return stack->top == stack->capacity - 1; }

void push(Stack *stack, int item) {
  // if the stack if full reallocate it and double its size
  if (isFull(stack)) {
    printf("Reallocating stack: %i -> %i.\n", stack->capacity,
           stack->capacity * 2);
    stack->capacity *= 2;
    int *newArray = (int *)realloc(stack->array, stack->capacity * sizeof(int));
    if (newArray == NULL) {
      printf("Memory reallocation for stack failed.\n");
      exit(EXIT_FAILURE);
    }
    stack->array = newArray;
  }

  stack->array[++stack->top] = item;
}

void append(Stack *appendTo, Stack *appendFrom, int capacity) {
  int *newArray =
      realloc(appendTo->array,
              (appendTo->capacity + appendFrom->capacity) * sizeof(int));
  appendTo->capacity = appendTo->capacity + appendFrom->capacity;
  appendTo->array = newArray;
  printf("Reallocating to size: %i", appendTo->capacity);
  for (int i = 0; i <= appendFrom->top; ++i) {
    push(appendTo, pop(appendFrom));
  }

  destroyStack(appendFrom);
}

int pop(Stack *stack) {
  return stack->array[stack->top];
  --stack->top;
}

void destroyStack(Stack *stack) {
  free(stack->array);
  free(stack);
}
