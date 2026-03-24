#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MEMORY_SIZE 30000

int main(int argc, char *argv[]) {

    if (argc < 2){
        printf("usage: brainfuck path/to/the/program.bf");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (!file) {
        printf("missing file, \"%s\".\n", argv[1]);
        return 1;
    }

    uint8_t *memory = (uint8_t*)calloc(MEMORY_SIZE, sizeof(uint8_t));
    int cell = 0;
    int c;
    int stack = 0;

    while ((c = fgetc(file)) != EOF) {
        switch (c) {
            case '>': cell++; if (cell >= MEMORY_SIZE) cell = 0; break;
            case '<': cell--; if (cell < 0) cell = MEMORY_SIZE - 1; break;
            case '+': memory[cell]++; break;
            case '-': memory[cell]--; break;
            case '.': putchar(memory[cell]); break;
            case ',': memory[cell] = getchar(); break;
            case '[':
                if (memory[cell] == 0) {
                    stack = 1;
                    while (stack > 0) {
                        c = fgetc(file);
                        if (c == '[') stack++;
                        else if (c == ']') stack--;
                    }
                }
                break;
            case ']':
                if (memory[cell] != 0) {
                    stack = 1;
                    long pos = ftell(file) - 2;
                    while (stack > 0) {
                        fseek(file, pos, SEEK_SET);
                        c = fgetc(file);
                        if (c == ']') stack++;
                        else if (c == '[') stack--;
                        pos--;
                    }
                    fseek(file, pos +1, SEEK_SET);
                }
                break;
        }
    }
    puts("\n");
    fclose(file);
    free(memory);
    return 0;
}
