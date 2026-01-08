# Bytecode Virtual Machine & Assembler

## Overview

This project implements a lightweight, stack-based Virtual Machine (VM) and a corresponding Assembler, both written in C. The system is designed to execute a custom bytecode instruction set supporting integer arithmetic, memory management, control flow (branching/looping), and function calls.

## Project Structure

* **`src/vm.c`**: The core virtual machine implementation containing the fetch-decode-execute loop and stack management.
* **`src/assembler.c`**: A two-pass assembler that parses assembly text files, resolves labels, and generates binary bytecode.
* **`src/isa.h`**: The shared header defining the Instruction Set Architecture (opcodes).
* **`tests/`**: Directory containing assembly test files (`.asm`).
* **`Makefile`**: Build automation script.

## Prerequisites & Platform Support

### Supported Platforms
This project is written in standard C99 and is designed to be cross-platform. It has been tested on:
* **Linux** (Ubuntu/Debian, Fedora)
* **macOS** (via Terminal)
* **Windows** (via WSL, MinGW, or Cygwin)

### Build Requirements
To compile the project, you need the following installed on your system:
* **C Compiler:** `gcc` (GNU Compiler Collection) or `clang`.
* **Build Tool:** `make` (GNU Make).

**Verify Installation:**
```bash
gcc --version
make --version
```
## Building the Project

The project uses `make` for compilation. Ensure you have `gcc` installed.

To build both the Assembler and the VM:

```bash
make all
```

To build components individually:
```bash
make assembler
make vm
```

To clean build artifacts (executables and binary files):
```bash
make clean
```

## Usage
1. The AssemblerThe assembler converts human-readable assembly code (`.asm`) into machine-readable bytecode (`.bin`).

Syntax:
```bash
./assembler <input_file.asm> [<output_file.bin>]
```

If the output file is not specified, it defaults to `a.bin`.

2. The Virtual MachineThe VM executes the binary bytecode. 

It includes a verbose mode to print the stack state after every instruction.Syntax:
```bash
./vm <bytecode_file.bin> [-verbose]
```

## Running Tests

The project includes an automated test suite configured in the Makefile. It looks for `.asm` files in the tests directory, compiles them, and runs them on the VM with verbose output.

To run the test suite:
```bash
make tests
```

## Instruction Set Architecture (ISA)
The VM supports the following operations:
| Category | Opcode | Description |
| :--- | :--- | :--- |
| **Data** | `PUSH`, `POP`, `DUP` | Stack manipulation. |
| **Math** | `ADD`, `SUB`, `MUL`, `DIV` | Integer arithmetic. |
| **Logic** | `CMP` | Compare top two values (pushes 1 if true, 0 if false). |
| **Control** | `JMP`, `JZ`, `JNZ` | Unconditional and conditional jumps (Zero/Not Zero). |
| **Memory** | `LOAD`, `STORE` | Read/Write to global memory. |
| **Functions** | `CALL`, `RET` | Function calls using a dedicated return stack. |
| **System** | `HALT` | Stops execution. |

## Technical Specs
- Stack Size: 256 integers
- Memory Size: 1024 integers
- Return Stack Size: 256 integers
- Endianness: Little-Endian
