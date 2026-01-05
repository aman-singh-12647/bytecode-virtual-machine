#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "isa.h"

#define MAX_LINE_LEN 256
#define MAX_LABEL_LEN 64

struct Label
{
  char name[MAX_LABEL_LEN];
  int address;
  struct Label *next;
} *label_table = NULL;

struct ParsedInstruction
{
  int op;
  char arg[64];
  int expects_arg;
  struct ParsedInstruction *next;
} *instructions = NULL;

/**
 * trim_whitespace - Trims leading and trailing whitespace from a string
 * @param str The input string to trim
 *
 * @return A pointer to the trimmed string (modified in-place)
 */
char *trim_whitespace(char *str)
{
  // Trim leading whitespace
  while (isspace((unsigned char)*str))
    str++;

  // Check if all characters are whitespace
  if (*str == 0)
    return str;

  // Trim trailing whitespace
  char *end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end))
    end--;

  end[1] = '\0';
  return str;
}

/**
 * has_argument - Checks if an opcode requires an argument
 * @param opcode The opcode integer
 *
 * @return 1 if the opcode requires an argument, 0 otherwise
 */
int has_argument(int opcode)
{
  switch (opcode)
  {
  case OP_PUSH:
  case OP_JMP:
  case OP_JZ:
  case OP_JNZ:
  case OP_STORE:
  case OP_LOAD:
  case OP_CALL:
    return 1;
  default:
    return 0;
  }
}

/**
 * get_opcode - Returns the opcode integer for a given mnemonic string
 * @param mnemonic The instruction mnemonic (e.g., "PUSH", "ADD")
 *
 * @return The opcode integer, or -1 if the mnemonic is invalid
 */
int get_opcode(const char *mnemonic)
{
  if (strcmp(mnemonic, "PUSH") == 0)
    return OP_PUSH;
  if (strcmp(mnemonic, "POP") == 0)
    return OP_POP;
  if (strcmp(mnemonic, "DUP") == 0)
    return OP_DUP;
  if (strcmp(mnemonic, "HALT") == 0)
    return OP_HALT;
  if (strcmp(mnemonic, "ADD") == 0)
    return OP_ADD;
  if (strcmp(mnemonic, "SUB") == 0)
    return OP_SUB;
  if (strcmp(mnemonic, "MUL") == 0)
    return OP_MUL;
  if (strcmp(mnemonic, "DIV") == 0)
    return OP_DIV;
  if (strcmp(mnemonic, "CMP") == 0)
    return OP_CMP;
  if (strcmp(mnemonic, "JMP") == 0)
    return OP_JMP;
  if (strcmp(mnemonic, "JZ") == 0)
    return OP_JZ;
  if (strcmp(mnemonic, "JNZ") == 0)
    return OP_JNZ;
  if (strcmp(mnemonic, "STORE") == 0)
    return OP_STORE;
  if (strcmp(mnemonic, "LOAD") == 0)
    return OP_LOAD;
  if (strcmp(mnemonic, "CALL") == 0)
    return OP_CALL;
  if (strcmp(mnemonic, "RET") == 0)
    return OP_RET;
  return -1;
}