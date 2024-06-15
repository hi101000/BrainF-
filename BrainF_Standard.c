#include <stdio.h>
#include <stdlib.h>

#define MEMLEN 1000 // The total size of memory

typedef char byte;
void exec(char* code, byte** mem, int pos);

int main(int argc, char* argv[]) {
    int pos = 0;
    // START: file-handling
    char code[400]; // Fixed size buffer for simplicity, adjust size as needed
    if(argc >= 2) {
        FILE *codefile = fopen(argv[1], "r");
        fread(code, sizeof(code), 1, codefile);
        fclose(codefile);
    }
    else
        if (scanf("%399s", code) != 1) {
            fprintf(stderr, "Error reading input\n");
            return 1;
        }
    // END: file-handling

    // Allocate the memory necessary, with each cell being one byte
    byte* mem[MEMLEN]; // create an array of MEMLEN pointers, each pointing to one cell
    for(int i = 0; i < MEMLEN; ++i) {
        mem[i] = (byte*) malloc(sizeof(byte)); // allocating each pointer to one cell
        *mem[i] = 0; // Initialize the memory cells to 0
    }

    // Begin execution of code
    exec(code, mem, pos);

    // END OF EXECUTION

    for(int i = 0; i < MEMLEN; ++i){
        free(mem[i]);
    }

    return 0;
}

void exec(char* code, byte** mem, int pos) {
    int j;
    while (*code != '\0') {
        switch (*code) {
            case '+':
                if ((int)*mem[pos] < 255)
                    ++*mem[pos];
                else
                    printf("OVERFLOW ERROR: CANNOT HAVE VALUE GREATER THAN 255 IN EACH MEMORY CELL\n Traceback:\n\tchar %d\n", pos);
                break;

            case '-':
                if (*mem[pos] > 0)
                    --*mem[pos];
                else
                    printf("UNDERFLOW ERROR: CANNOT HAVE VALUE LESS THAN 0 IN EACH MEMORY CELL\n Traceback:\n\tInstruction %d\n", pos);
                break;

            case '>':
                if (pos < MEMLEN - 1)
                    pos++;
                else
                    printf("OUT-OF-BOUNDS ERROR: CAN ONLY HANDLE %zu MEMORY CELLS\nTraceback:\n\tInstruction %d\n", (size_t)MEMLEN, pos);
                break;

            case '<':
                if (pos > 0)
                    pos--;
                else
                    printf("OUT-OF-BOUNDS ERROR: CANNOT GO PAST MEMORY CELL 0\nTraceback:\n\tInstruction %d\n", pos);
                break;

            case '.':
                putchar(*mem[pos]);
                break;

            case ',':
                do {
                    *mem[pos] = getchar();
                }
                while(*mem[pos] == '\n');
                break;

            case '[':
                if (*mem[pos] == 0) {
                    // Skip to the matching ']'
                    int loop = 1;
                    while (loop > 0) {
                        code++;
                        if (*code == '[') loop++;
                        if (*code == ']') loop--;
                    }
                }
                break;

            case ']':
                if (*mem[pos] != 0) {
                    // Jump back to the matching '['
                    int loop = 1;
                    while (loop > 0) {
                        code--;
                        if (*code == '[') loop--;
                        if (*code == ']') loop++;
                    }
                }
                break;

            default:
                break;
        }
        code++;
    }
}
