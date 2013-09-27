//start with 30000 cells
//+ increments value at current cell by one
//- decrements value at current cell by one
//> moves the data pointer to the next cell
//< moves the data pointer to the previous cell
//. prints the ascii value at the current cell
//, reads a single input character into the current cell
//[ if the value at the current cell is 0, skip to the corresponding ]
//	else move to the next instruction
//] if the value at the current cell is 0, move to the next instruction
//	else move back to the corresponding [

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
	if (!(argc == 2 || argc == 3)){
		printf("usage: %s [file.bf] \"[input]\"\n", argv[0]);
		return 1;
	}
	
	FILE *bfsrc = fopen(argv[1], "r");
	if (bfsrc == NULL){
		printf("file error\n");
		return 1;
	}

	//copy bf source file into char *src
	char *src;
	fseek(bfsrc, 0, SEEK_END);
	long fileSize = ftell(bfsrc);
	rewind(bfsrc);
	
	src = (char *) malloc(sizeof(char) * fileSize);
	if(src == NULL){
		printf("memory error\n");
		return 1;
	}
	if(fread(src, 1, fileSize, bfsrc) != fileSize){
		printf("reading error\n");
		return 1;
	}
	fclose(bfsrc);
	
	int inputLen = 0;
	for(int i = 2; i < argc; i++) inputLen += strlen(argv[i]) + 1; //+1 for space

	char *input = (char*) malloc(sizeof(char) * --inputLen); //-- for extra space after last arg
	for(int i = 2; i < argc; i++){
		strcat(input, argv[i]);
		if(i != argc) strcat(input, " ");
	}
	printf("input: %d chars", inputLen);
//	char *input = (char *)malloc(sizeof(char) * strlen(argv[2])-2);

	int cells[30000], ptr, iptr, bracketCtr, inputPtr;
	//data cells, data ptr, instruction ptr, bracket counter
	
	for(ptr = 0, iptr = 0, bracketCtr = 0, inputPtr = 0; iptr < fileSize;) switch(src[iptr]){
		case '+':								//increment current cell
			cells[ptr]++;
			iptr++; break;
		case '-':								//decrement current cell
			cells[ptr]--;
			iptr++; break;
		case '>':								//move to next cell
			if(ptr == 29999) ptr = 0;			//wrap around
			else ptr++;
			iptr++; break;
		case '<':								//move to previous cell
			if(ptr == 0) ptr = 29999;			//wrap around
			else ptr--;	
			iptr++; break;
		case '.':								//print current cell as ascii
			printf("%c", cells[ptr]);
			iptr++; break;
		case ',':								//read in 1 char to cell
			if(inputPtr >= inputLen) cells[ptr] = 32;
			else cells[ptr] = input[inputPtr++];
			iptr++; break;
		case '[':								//open loop
			if(cells[ptr] == 0) do {				
				iptr++;
				if(src[iptr] == '[') bracketCtr++;
				else if(src[iptr] == ']') bracketCtr--;
			} while(src[iptr] != ']' && bracketCtr != 0);
			else {
				iptr++;
				bracketCtr++;
			}
			break;
		case ']':								//close loop
			if(cells[ptr] == 0){
				iptr++;
				bracketCtr--;
			} else do {
				iptr--;
				if(src[iptr] == '[') bracketCtr++;
				else if(src[iptr] == ']') bracketCtr--;
			} while(src[iptr] != '[' && bracketCtr != 0);
			break;
		default: iptr++; break;	//any other characters are ignored
	}
	printf("\n");
	free(src);
	return 0;
}
