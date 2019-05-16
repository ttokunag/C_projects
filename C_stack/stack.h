#ifndef STACK_H
#define STACK_H
#include <stdbool.h>

// Please do not change anything in header file //
// struct definition for each element in stack
struct Element {
  int key;
  char* value;
};

typedef struct Element Element;

// struct definition for stack
struct Stack {
  int top;
  int capacity;
  float growthFactor;
  float shrinkFactor;
  bool dynamic;
  Element** elements;
};

typedef struct Stack Stack;

// set of functions for stack
Stack* makeStack(int cap);
Stack* makeStackG(int cap, float growF);
Stack* makeStackGnS(int cap, float growF, float shrinkF);
Element* pop(Stack* s);
Element** multiPop(Stack* s, int k);
bool push(Stack* s, int k, char* v);
bool pushUnique(Stack* s, int k, char* v);
void reverse(Stack* s); // resizable
Element* peek(Stack* s);
int search(Stack* s, int k, char* v);
int getCapacity(Stack* s);
bool isFull(Stack* s);
void clear(Stack* s);
void cleanStack(Stack* s);
int currentSize(Stack* s);
bool isEmpty(Stack* s);

#endif
