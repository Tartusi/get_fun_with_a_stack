#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

#define STACK_SIZE 4096
#define MAX_INST_LENGTH 256
#define INT_SIZE sizeof(int)
//#define DEBUG

void* stack;
void* stackPointer;

short checkState() {
	if(stackPointer>=stack && stackPointer<stack+STACK_SIZE) return 1;
	return 0;
}

char** readInstruction(FILE* file){
	
	char* line=malloc(sizeof(char) * 256);
	if (!fgets(line,MAX_INST_LENGTH ,file)) {
		return NULL;
	}
	
	char** tabInst = malloc(2*sizeof(char*));  // on n'a pas besoin de stocker plus de deux instructions dans le jeu d'instruction actuel	
	strtok(line," ");
	tabInst[0] = strtok(NULL," ");
	tabInst[1] = strtok(NULL," ");
	return tabInst;
}
short push(int value) {

	if(checkState()) {
		*(int *)stackPointer = value;
		stackPointer += INT_SIZE;
		return 1;
	}
	else {
		printf("Underflow or overflow in PUSH, stoping interpretation\n");
		return 0;
	}

}
short pop(int* value) {
	stackPointer=stackPointer - INT_SIZE;
	if(checkState()) {
		*value=*(int*)stackPointer;
		return 1;
	}
	else {
		printf("Underflow or overflow in POP, stoping interpretation\n");
		return 0;
	}
}

short write(){
	int value;
	if(pop(&value)) {
		printf("%d\n", value);
		return 1;
	}
	else {
		printf("Error in WRITE");
		return 0;
	}
}

short dup(){
	int value;
	return pop(&value) | push(value) | push(value);
	return 0;
}

short add(){
	int value[2];
	return pop(&value[0]) | pop(&value[1]) | push(value[0]+value[1]);
}

short mul(){
	int value[2];
	return pop(&value[0]) | pop(&value[1]) | push(value[0]*value[1]);
}

short read(){
	int value;
	#ifdef DEBUG
		printf("Integer expected\n");
	#endif
	if(scanf("%d", &value)){
		return push(value);
	}
	printf("Error reading number, stoping\n");
	return 0;
}

short execInstructionWrapper(char** tabInst){
	switch (tabInst[0][0]) {
		case 'P':
			if(!tabInst[1]) {
				printf("push need an argument\n");
				return 0;
			}
			return push(atoi(tabInst[1]));
		case 'W':
			return write();
		case 'D':
			return dup();
		case 'A':
			return add();
		case 'M':
			return mul();
		case 'R':
			return read();
		default:
			#ifdef DEBUG
				printf("Unknow Instruction: %s\n", tabInst[0]);
			#endif
			break;
	}
	return 0;
}
short execInstruction(char** tabInst){
	short retval;
	if (tabInst && tabInst[0]){
		#ifdef DEBUG
			printf("%s",tabInst[0]);
		#endif

		retval=execInstructionWrapper(tabInst);
		free(tabInst);
	}
	return retval;
}

				
		

int main(int argc, char** argv){
	
	if (argc != 2) {
		printf("Usage: %s filetointerpret\n",argv[0]);
		return 0;
	}
	stack=malloc(STACK_SIZE);
	stackPointer=stack;
	FILE* file=fopen(argv[1],"r");

	#ifdef DEBUG
		printf("Starting of parsing\n");
	#endif
	
	while(execInstruction(readInstruction(file)));
	free(stack);	
	#ifdef DEBUG
		printf("End of parsing\n");
	#endif
}



