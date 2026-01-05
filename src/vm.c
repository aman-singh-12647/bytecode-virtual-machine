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
 * Creates and initializes a new VM instance.
 * @param code Pointer to bytecode buffer
 * @param size Size of bytecode in bytes
 *
 * @return Pointer to initialized VM structure
 */
VM *create(unsigned char *code, int size)
{
    VM *vm = (VM *)malloc(sizeof(VM));
    vm->sp = 0;
    vm->rsp = 0;
    vm->code = code;
    vm->code_size = size;
    vm->pc = code;
    vm->running = 1;
    return vm;
}

/**
 * Loads a value from VM memory at the specified address.
 * @param vm Pointer to the VM instance
 * @param addr Memory address to load from
 *
 * @return Value at the address, or 0 if address is out of bounds
 */
int mem_load(VM *vm, int addr)
{
    if (addr < 0 || addr >= MEMORY_SIZE)
    {
        fprintf(stderr, "Error: Memory Load Violation @ %d\n", addr);
        vm->running = 0;
        return 0;
    }
    return vm->memory[addr];
}

/**
 * Stores a value in VM memory at the specified address.
 * @param vm Pointer to the VM instance
 * @param addr Memory address to store at
 *
 * @param val Value to store
 */
void mem_store(VM *vm, int addr, int val)
{
    if (addr < 0 || addr >= MEMORY_SIZE)
    {
        fprintf(stderr, "Error: Memory Store Violation @ %d\n", addr);
        vm->running = 0;
        return;
    }
    vm->memory[addr] = val;
}

/**
 * Pops a value from the stack.
 * @param vm Pointer to the VM instance
 *
 * @return Value popped from the stack, or 0 if stack underflow occurs
 */
int pop(VM *vm)
{
    if (vm->sp <= 0)
    {
        fprintf(stderr, "Error: Stack Underflow\n");
        vm->running = 0;
        return 0;
    }
    return vm->stack[--vm->sp];
}

/**
 * Prints the current state of the stack to stdout.
 * @param vm Pointer to the VM instance
 */
void print_stack(VM *vm)
{
    printf("Stack: [");
    for (int i = 0; i < vm->sp; i++)
    {
        printf(" %d", vm->stack[i]);
    }
    printf(" ]\n");
}
/**
 * Pushes a value onto the stack.
 * @param vm Pointer to the VM instance
 * @param value Value to push onto the stack
 */
void push(VM *vm, int value)
{
    if (vm->sp >= STACK_SIZE)
    {
        fprintf(stderr, "Error: Stack Overflow\n");
        vm->running = 0;
        return;
    }
    vm->stack[vm->sp++] = value;
}

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