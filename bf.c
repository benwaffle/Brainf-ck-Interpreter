#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <err.h>

int main(int argc, char *argv[]){
    if (argc != 2) {
            fprintf(stderr, "Usage: %s <program.bf>", argv[0]);
            return 1;
    }
    
    FILE *src = fopen(argv[1], "r");
    if (!src) err(1, NULL);

    fseek(src, 0, SEEK_END);
    long size = ftell(src);
    rewind(src);

    char *prog = malloc(size);
    fread(prog, size, 1, src);
    fclose(src);

    char _tape[30000] = {0};
    
    for(char *tape = _tape, *pc = prog; pc < prog + size; ++pc) {
        switch(*pc){
            case '+': { 
                for (; *pc == '+'; ++pc)
                    ++*tape;
                --pc;
                break; 
            }
            case '-': {
                for (; *pc == '-'; ++pc)
                    --*tape;
                --pc;
                break;
            }
            case '>': {
                for (; *pc == '>'; ++pc)
                    tape++;
                --pc;
                break;
            }
            case '<': {
                for (; *pc == '<'; ++pc)
                    tape--;
                --pc;
                break;
            }
            case '.': putchar(tape[0]); break;
            case ',': tape[0] = getchar(); break;
            case '[': {
                if (tape[0] == 0) {
                    int b = 1;
                    do {
                        ++pc;
                        if (*pc == '[') ++b;
                        if (*pc == ']') --b;
                    } while (b > 0);
                }
                break;
            }
            case ']': {
                if (tape[0] != 0) {
                    int b = -1;
                    do {
                        --pc;
                        if (*pc == '[') ++b;
                        if (*pc == ']') --b;
                    } while (b < 0);
                }
                break;
            }
            default: break;
        }
    }
    free(prog);
}
