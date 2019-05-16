#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "stack.h"

/*
* Author: Tomoya Tokunaga (mailto: ttokunag@ucsd.edu) 
* About this file:
* This file contains codes which implements the Stack data structure
* in C language. Refer to stack.h file for codes on Struct objects  
* which are used in here
*/


/*
* description: function which pops the top element of a stack
* may shrink the stack if it's shrinkable
*
* @param Stack* s: a pointer to a stack object
*/
Element* pop(Stack* s){
	// return NULL pointer if a given stack is empty
	if (isEmpty(s)) {
		return NULL;
	}
	// shrink the capacity of the stack if it's x% full
	if (s->dynamic && (float)currentSize(s)/(float)(s->capacity) <= s->shrinkFactor) {
		// allocates memory for half size elements
		Element** shrinkE = (Element**)malloc((s->capacity)*sizeof(Element*)/2);
		for (int i = 0; i < currentSize(s); i++) {
			// copy an element
			shrinkE[i] = (Element*)malloc(sizeof(Element));
			shrinkE[i]->key = s->elements[i]->key;
			shrinkE[i]->value = s->elements[i]->value;
			// free uo the old pointer
			free(s->elements[i]);
		}
		// free up old pointer and assign new one
		free(s->elements);
		s->elements = shrinkE;

		// half the capacity
		int* cap_ptr = &(s->capacity);
		(*cap_ptr) = (*cap_ptr)/2;
	}

	// set the pointer to the top
	int* top_ptr = &(s->top);
	// "copy" the top element
	//Element* pop_ele = s->elements[(*top_ptr)-1];
	Element* pop_ele = (Element*)malloc(sizeof(Element));
	pop_ele->key = (s->elements[(*top_ptr)-1])->key;
	pop_ele->value = (s->elements[(*top_ptr)-1])->value;
	
	// frees up the memory for top element
	free(s->elements[*top_ptr]);
	(*top_ptr)--;

	return pop_ele;
}

/*
* description: function which pops multiple elements fromt
* the top of the given stack. Multiple use of pop function
*
* @param Stack* s: a pointer to a stack object
* @param int k: the number of pop operations
*/
Element** multiPop(Stack* s, int k){
	// if the given stack is empty, return NULL pointer
	if (isEmpty(s)) {
		return NULL;
	}
	// allocates memory for popped items
	Element** pop_items = (Element**)malloc(k * sizeof(Element*));

	for (int i = 0; i < k; i++) {
		// allocates memory for a popped element
		Element* ith_pop = (Element*)malloc(sizeof(Element));
		ith_pop = pop(s);
		// if the stack gets empty, break the loop and return
		// the pointer to pointers
		if (ith_pop == NULL) {
			// ith_pop is no longer needed, so free it up
			free(ith_pop);
			break;
		}
		//int* i_ptr = (int*)malloc(sizeof(int));
		//pop_items[*i_ptr] = ith_pop;
		pop_items[i] = ith_pop;
	}

	return pop_items;
}

// Utility function: add element for push
// This function is a hint, you may ignore its implementation
void addE(Stack* s, int k, char* v) {

}

// Utility function: expandCapacity for push
// This function is a hint, you may ignore its implementation
void expandCapacity(Stack* s) {

}

/*
* description: function which pushes an item to the top of the given
* stack. This may expand the stack if it's growable.
*
* @param Stack* s: a pointer to a stack object
* @param int k: key of the element to be pushed
* @param char* v: value of the element to be pushed
*/
bool push(Stack* s, int k, char* v){
	// if the stack is full, return false
	if (isFull(s)) {
		return false;
	}
	// grow the capacity of the stack if it's x% full
	if (s->dynamic && ((float)currentSize(s)/(float)(s->capacity) >= s->growthFactor)) {
		// allocate memory for double size elements
		Element** grownE = (Element**)malloc((s->capacity)*sizeof(Element*)*2);
		for (int i = 0; i < currentSize(s); i++) {
			//Element* ith_ele = s->elements[i];
			// copy an element
			grownE[i] = (Element*)malloc(sizeof(Element));
			grownE[i]->key = s->elements[i]->key;
			grownE[i]->value = s->elements[i]->value;
			// free up the old pointer
			free(s->elements[i]);
		}
		// free up old pointer and assign new one
		free(s->elements);
		s->elements = grownE;

		// double the capacity
		int* cap_ptr = &(s->capacity);
		(*cap_ptr) = (*cap_ptr)*2;
	}

	// initialize Element object to be pushed
	Element* ele = (Element*)malloc(sizeof(Element));
	ele->key = k;
	ele->value = v;
	// pushes the element
	int* top_ptr = &(s->top);
	(s->elements)[*top_ptr] = ele;
	// increments the top
	(*top_ptr)++;

	return true;
}

/*
* description: function which pushes an item which is not identical
* to the top of the given element. (we call two items are identical 
* if their key & value are the same)
*
* @param Stack* s: a pointer to a stack object
* @param int k: key of the element to be pushed
* @param char* v: value of the element to be pushed
*/
bool pushUnique(Stack* s, int k, char* v){
	
	if (isFull(s) && !(s->dynamic)) {
		return false;
	}	
	// if the peek element is the same as the given element
	// return false
	if (peek(s)->key == k && peek(s)->value == v) {
		return false;
	}
	// otherwise, add the element and return true
	push(s, k, v);
	return true;
}

// reverse the elements in stack
void reverse(Stack* s){
	// allocates memory for indexes to swap
	int* i = (int*)malloc(sizeof(int));
	int* j = (int*)malloc(sizeof(int));
	// initializes swap indexes
	*i = currentSize(s) - 1;
	*j = 0;
	// keep looping until completing reverse
	while (*i > *j) {
		// allocates memory for a temporary object
		Element* temp = (Element*)malloc(sizeof(Element));
		// the followings are swapping elements
		temp->key = s->elements[*i]->key;
		temp->value = s->elements[*i]->value;
		s->elements[*i]->key = s->elements[*j]->key;
		s->elements[*i]->value = s->elements[*j]->value;
		s->elements[*j]->key = temp->key;
		s->elements[*j]->value = temp->value;
		// moves cursors for the next loop
		(*i)--;
		(*j)++;
		// temp is no longer needed, so free up the memory
		free(temp);
	}
	// the end of the operation, so free up memory for the indexes
	free(i);
	free(j);
}

/*
* description: function which returns the pointer to the top 
* of the given stack
*
* @param Stack* s: a pointer to a stack object
*/
Element* peek(Stack* s){
	// sets a pointer to the top
	int* top_ptr = &(s->top);
	// retrieves the top element
	Element* ptr = (s->elements)[(*top_ptr) - 1];
	return ptr;
	
}
/*
* description: function which returns 1-based distance from the top 
* of the given stack. If the value is not found, or the stack is empty,
* return -1
*
* @param Stack* s: a pointer to a stack object
* @param int k: key of the element we look for in the stack
* @param char* v: value of the element we look for in the stack
*/
int search(Stack* s, int k, char* v){
	// returns false if the given stack is empty
	if (isEmpty(s)) {
		return -1;
	}
	// linear search to find the target
	int* top_ptr = &(s->top);
	for (int i = 0; i < currentSize(s); i++) {
		if (s->elements[i]->key == k && s->elements[i]->value == v) {
			return *top_ptr - i;
		}
	}
	// if the target is not found, return -1
	return -1;
}

/*
* description: function which returns the capacity of the given stack
*
* @param Stack* s: a pointer to a stack object
*/
int getCapacity(Stack* s){
	
	return s->capacity;

}

/*
* description: function which tells if the given stack is full
*
* @param Stack* s: a pointer to a stack object
*/
bool isFull(Stack* s){
	return s->top == s->capacity;
}

/*
* description: function which clear the contents of the given stack
*
* @param Stack* s: a pointer to a stack object
*/
void clear(Stack *s) {
	// free up all pointers' memory
	for (int i = 0; i < s->capacity; i++) {
		free(s->elements[i]);
	}
	// free up the pointer to pointers
	free(s->elements);
	// re-allocate memory for the pointer to pointers
	s->elements = (Element**)malloc(s->capacity + sizeof(Element*));
	// set the size of the stack zero
	int* top_ptr = &(s->top);
	*top_ptr = 0;
}

/*
* description: function which free up the memory allocated for the 
* given stack
*
* @param Stack* s: a pointer to a stack object
*/
void cleanStack(Stack *s) {
	for(int i = 0; i < s->capacity; i++) {
		free(s->elements[i]);
	}
	free(s->elements);
	free(s);
}

/*
* description: function which returns the number of elements in the 
* given stack. Does NOT return the capacity of the stack
*
* @param Stack* s: a pointer to a stack object
*/
int currentSize(Stack* s){
	return s->top;
}

/*
* description: function which tells if the given stack is empty
*
* @param Stack* s: a pointer to a stack object
*/
bool isEmpty(Stack* s){
	return s->top == 0;
}

/*
* description: a constructor for a fixed size stack
*
* @param int cap: the capacity of the stack
*/
Stack* makeStack(int cap){
	// allocates memory to the stack
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	// initialize each variable
	stack->top = 0;
	stack->capacity = cap;
	stack->dynamic = false;
	// allocates memory for the pointer to pointers
	stack->elements = (Element**)malloc(cap * sizeof(Element*));

	return stack;
}

/*
* description: a constructor for a growable stack
*
* @param int cap: the capacity of the stack
* @param float growF: a float which determine when the stack expands
*/
Stack* makeStackG(int cap, float growF){
	// allocates memory to the stack
	Stack* stackG = (Stack*)malloc(sizeof(Stack));
	// initialize each variable
	stackG->top = 0;
	stackG->capacity = cap;
	stackG->dynamic = true;
	stackG->growthFactor = growF;
	// allocates memory for the pointer to pointers
	stackG->elements = (Element**)malloc(cap * sizeof(Element*));

	return stackG;
}

/*
* description: a constructor for a growable, shrinkable stack
*
* @param int cap: the capacity of the stack
* @param float growF: a float which determines when the stack expands
* @param float shrinkF: a float which determines when the stack shrinks
*/
Stack* makeStackGnS(int cap, float growF, float shrinkF){
	// allocates memory to the stack
	Stack* stackGS = (Stack*)malloc(sizeof(Stack));
	// initialize each variable
	stackGS->top = 0;
	stackGS->capacity = cap;
	stackGS->dynamic = true;
	stackGS->growthFactor = growF;
	stackGS->shrinkFactor = shrinkF;
	// allocates memory for the pointer to pointers
	stackGS->elements = (Element**)malloc(cap * sizeof(Element*));

	return stackGS;
}


/*int main(void) {
	
	Stack* stack = makeStackG(4, 0.5);
	push(stack, 939526, "hello world!");
	push(stack, 682340, "I'm about to cry!!");
	push(stack, 189262, "I have stomachache");
	//pushUnique(stack, 189262, "I have stomachache");
	pushUnique(stack, 281734, "Unique push!!");
	push(stack, 798743, "This is growable!"); //successfully grown!
	Element** popped = multiPop(stack, 2);
	//Element* top = peek(stack);
	// reverse succeessfully worked!
	//reverse(stack);
	
	for (int i = 0; i < currentSize(stack); i++) {
		printf("k is: %i  v is: %s\n", stack->elements[i]->key, stack->elements[i]->value);
	}

	//clear properly worked!
	//clear(stack);

	printf("Current capacity is: %i\n", getCapacity(stack));
	printf("Current size is    : %i\n", currentSize(stack));
	printf("Is it empty?       : %i\n", isEmpty(stack));
	printf("Is it full?        : %i\n", isFull(stack));
	//printf("Value of the top   : %s\n", top->value);
	//printf("Key of the top     : %i\n", top->key);
	printf("Popped key         : %i\n", popped[1]->key);
	printf("Popped value       : %s\n", popped[1]->value);
	printf("Searching...       : %i\n", search(stack, 939526, "hello world!"));
	
	return 0;

}*/
