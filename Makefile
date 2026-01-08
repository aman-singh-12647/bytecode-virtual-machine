CC = gcc
CFLAGS = -Wall -Wextra -g
TESTDIR = tests
VALID_FILES = $(wildcard $(TESTDIR)/*.asm)

all: assembler vm

assembler: src/assembler.c src/isa.h
	$(CC) $(CFLAGS) src/assembler.c -o assembler

vm: src/vm.c src/isa.h
	$(CC) $(CFLAGS) src/vm.c -o vm

tests: assembler vm
	@echo "Running tests...\n"

	@for file in $(VALID_FILES); do \
		echo "Testing $$file..."; \
		./assembler $$file; \
		./vm a.bin -verbose; \
		echo "Tested $$file"; \
	done
	
	@echo "\nAll tests completed."

.PHONY: all tests clean

clean:
	rm -f assembler vm *.bin
