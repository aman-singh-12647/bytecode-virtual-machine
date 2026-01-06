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
 * Reads a 4-byte integer from the bytecode at the current program counter.
 * Increments the program counter by 4 bytes after reading.
 * @param vm Pointer to the VM instance
 *
 * @return The 32-bit integer value read from bytecode
 */
int read_bytes(VM *vm)
{
    int val = 0;
    val |= (unsigned char)(*vm->pc++) << 0;
    val |= (unsigned char)(*vm->pc++) << 8;
    val |= (unsigned char)(*vm->pc++) << 16;
    val |= (unsigned char)(*vm->pc++) << 24;
    return val;
}

/**
 * Executes the bytecode program loaded in the VM.
 * Fetches and executes instructions sequentially until the program halts
 * or encounters an error.
 * @param vm Pointer to the VM instance
 * @param verbose int: 1 if true, 0 otherwise
 */
void run(VM *vm, int verbose)
{
    while (vm->running && (vm->pc - vm->code) < vm->code_size)
    {
        unsigned char opcode = *vm->pc;
        vm->pc++;

        switch (opcode)
        {
        case OP_PUSH:
        {
            int val = read_bytes(vm);
            push(vm, val);
            break;
        }
        case OP_POP:
        {
            pop(vm);
            break;
        }
        case OP_DUP:
        {
            int val = pop(vm);
            push(vm, val);
            push(vm, val);
            break;
        }
        case OP_HALT:
        {
            vm->running = 0;
            break;
        }
        case OP_ADD:
        {
            int b = pop(vm);
            int a = pop(vm);
            push(vm, a + b);
            break;
        }
        case OP_SUB:
        {
            int b = pop(vm);
            int a = pop(vm);
            push(vm, a - b);
            break;
        }
        case OP_MUL:
        {
            int b = pop(vm);
            int a = pop(vm);
            push(vm, a * b);
            break;
        }
        case OP_DIV:
        {
            int b = pop(vm);
            int a = pop(vm);
            if (b == 0)
            {
                fprintf(stderr, "Error: Divide by Zero\n");
                vm->running = 0;
            }
            else
            {
                push(vm, a / b);
            }
            break;
        }
        case OP_CMP:
        {
            int b = pop(vm);
            int a = pop(vm);
            push(vm, (a < b) ? 1 : 0);
            break;
        }
        case OP_STORE:
        {
            int idx = read_bytes(vm);
            int val = pop(vm);
            mem_store(vm, idx, val);
            break;
        }
        case OP_LOAD:
        {
            int idx = read_bytes(vm);
            int val = mem_load(vm, idx);
            push(vm, val);
            break;
        }
        case OP_JMP:
        {
            int target = read_bytes(vm);
            vm->pc = vm->code + target;
            break;
        }
        case OP_JZ:
        {
            int target = read_bytes(vm);
            int val = pop(vm);
            if (val == 0)
                vm->pc = vm->code + target;
            break;
        }
        case OP_JNZ:
        {
            int target = read_bytes(vm);
            int val = pop(vm);
            if (val != 0)
                vm->pc = vm->code + target;
            break;
        }
        case OP_CALL:
        {
            if (vm->rsp >= RETURN_STACK_SIZE)
            {
                fprintf(stderr, "Error: Call Stack Overflow\n");
                vm->running = 0;
            }
            else
            {
                int target = read_bytes(vm);
                vm->return_stack[vm->rsp++] = (int)(vm->pc - vm->code);
                vm->pc = vm->code + target;
            }
            break;
        }
        case OP_RET:
        {
            if (vm->rsp <= 0)
            {
                fprintf(stderr, "Error: Call Stack Underflow\n");
                vm->running = 0;
            }
            else
            {
                int ret_addr = vm->return_stack[--vm->rsp];
                vm->pc = vm->code + ret_addr;
            }
            break;
        }
        default:
            fprintf(stderr, "Unknown Opcode: 0x%X\n", opcode);
            vm->running = 0;
            break;
        }
        if (verbose == 1)
            print_stack(vm);
    }
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