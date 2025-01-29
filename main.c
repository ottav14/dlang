#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_SIZE 64
#define MAX_WORD_COUNT 3

typedef struct {
	int* items;
	int top;
	int capacity;
} Stack;

typedef struct {
	char items[MAX_WORD_COUNT][MAX_WORD_SIZE];
	int size;
} WordList;

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

WordList getWords(char* line) {

	int length = strlen(line);
	WordList words;
	words.size = 0;
	if(length == 1)
		return words;

	char currentWord[MAX_WORD_SIZE]; 
	int cwix = 0;

	for(int i=0; i<length; i++) {
		if(line[i] == ' ' || i == length-1) {
			if(words.size == MAX_WORD_COUNT) {
				words.size = -1;
				return words;
			}
			strcpy(words.items[words.size], currentWord);
			words.size++;			
			currentWord[0] = '\0';
			cwix = 0;
		}
		else {
			currentWord[cwix] = line[i];
			cwix++;
			currentWord[cwix] = '\0';
		}
	}
	return words;
}

int isNumber(char* word) {

	int length = strlen(word);
	for(int i=0; i<length; i++) {
		int current = word[i] - '0';
		if(current < 0 || current > 9)
			return 0;
	}
	return 1;

}

int validLine(WordList words) {

	const char* instructions[] = {
		"push",
		"pop",
		"add",
		"sub"
	};

	if(words.size == 0)
		return 1;

	if(words.size == 1) {
		printf("word size 1 ");
		return 0;
	}
	if(words.size == -1) {
		printf("too many words ");
		return 0;
	}

	// Check instruction
	int flag = 0;
	for(int i=0; i<4; i++) {
		if(strcmp(words.items[0], instructions[i]) == 0) {
			flag = 1;
			break;
		}
	}
	if(!flag) {
		printf("invalid instruction ");
		return 0;
	}

	// Check operands
	for(int i=1; i<words.size; i++) {
		if(!isNumber(words.items[i])) {
			printf("operand %d isnt a number ", i);
			return 0;
		}
	}

	return 1;
}

void interpretLine(char* line) {
	WordList words = getWords(line);

	if(validLine(words))
		printf("valid\n");
	else
		printf("invalid\n");

}

int main(int argc, char** argv) {

	Stack* stack = createStack();
    FILE *file;
    char buffer[1024];

	if(argc != 2) {
		perror("Expects 1 option in the form of a source file.");
		return EXIT_FAILURE;
	}
    
    file = fopen(argv[1], "r");
    
    if(file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    while(fgets(buffer, sizeof(buffer), file) != NULL)
		interpretLine(buffer);

    fclose(file);
	freeStack(stack);

	return 0;
}
