#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int* items;
	int top;
	int capacity;
} Stack;

void resize(Stack* stack) {

	if(!stack->items) {
		stack->items = (int*)malloc(sizeof(int));
		stack->capacity = 1;
	} 
	else {
		if(stack->top == stack->capacity-1)
			stack->capacity *= 2;
		else
			stack->capacity /= 2;

		stack->items = (int*)realloc(stack->items, stack->capacity*sizeof(int));
	}
}

void push(int val, Stack* stack) {

	if(!stack || stack->top == stack->capacity-1)
		resize(stack);

	stack->top += 1;
	stack->items[stack->top] = val; 

}

int pop(Stack* stack) {

	if(!stack) {
		perror("Error: attempting to pop from an empty stack.\n");
		return EXIT_FAILURE;
	}
	
	int val = stack->items[stack->top];
	stack->top -= 1;
	if(stack->top < stack->capacity/2)
		resize(stack);

	return val;

}

Stack* createStack() {
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	stack->items = NULL;
	stack->top = -1;
	stack->capacity = 0;
	return stack;
}

void freeStack(Stack* stack) {
	free(stack->items);
	free(stack);
}

void printStack(Stack* stack) {
	for(int i=0; i<=stack->top; i++)
		printf("%d: %d\n", i, stack->items[i]);
	printf("\n");
}

int main(int argc, char** argv) {

	Stack* stack = createStack();


	freeStack(stack);

	return 0;
}
