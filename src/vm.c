#include <stdio.h>
#include <stdlib.h>
#include "isa.h"

#define STACK_SIZE 256
#define MEMORY_SIZE 1024
#define RETURN_STACK_SIZE 256

typedef struct
{
    int stack[STACK_SIZE];
    int sp;
    int memory[MEMORY_SIZE];
    int return_stack[RETURN_STACK_SIZE];
    int rsp;
    unsigned char *pc;
    unsigned char *code;
    int code_size;
    int running;
} VM;

/**
 * Main entry point for the bytecode virtual machine.
 * Loads a bytecode file from the specified path and executes it.
 * @param argc Argument count
 * @param argv Argument vector (argv[1] should be the path to bytecode file)
 *
 * @return 0 on success, 1 on failure
 */
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Bytecode VM\nUsage: %s <bytecode_file> [-verbose]\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if (!f)
    {
        perror("Failed to open file");
        return 1;
    }

    // Get file size
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Load code into memory
    unsigned char *code = malloc(fsize);
    if (!code)
    {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(f);
        return 1;
    }
    fread(code, 1, fsize, f);
    fclose(f);

    printf("--- Executing %s ---\n", argv[1]);
    int verbose = (argc > 2 && strcmp(argv[2], "-verbose") == 0);

    VM *vm = create(code, fsize);
    run(vm, verbose);

    printf("--- Final State ---\n");
    print_stack(vm);

    free(vm);
    free(code);
    return 0;
}