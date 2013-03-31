#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>

#include <vector>
#include <iostream>

void print_usage(const char* progname) {
	fprintf(stderr, "Usage : \n");
	fprintf(stderr, "	%s <source>\n", progname);
}

const int BF_ERROR_UNPAIRED_BRACKET = -2;
const int BF_ERROR_STACK_UNDERFLOW = -3;
const int BF_ERROR_STACK_OVERFLOW = -4;

int parse_int(char* code, size_t* cp) {
	int result = 0;

	while (isdigit(code[*cp])) {
		result = result * 10 + (code[*cp]-'0');
		(*cp)++;
	}

	return result;
}

int skip_brackets(char* code, size_t* cp) {
	int balance = 1;
	
	while (balance != 0) {
		if (code[*cp] == '[') balance++;
		else if (code[*cp] == ']') balance--;
		else if (code[*cp] == '\0') return -1;
		(*cp)++;
	}
	
	return 0;
}

int interpret_file(char* code) {
	const size_t MEMORY_SIZE = 16;
	const size_t STACK_SIZE = 256;

	unsigned char memory[MEMORY_SIZE];
	memset(memory, 0, sizeof(memory));
	size_t mp = 0;

	size_t stack[STACK_SIZE];
	memset(stack, 0, sizeof(stack));
	size_t sp = 0;

	size_t cp = 0;
	
	while (code[cp] != 0) {
		char opcode = code[cp++];
	
		switch (opcode) {
			case '+': {
				int v = 1;
				if (isdigit(code[cp])) {
					v = parse_int(code, &cp);
				}
				memory[mp] += v;
				break;
			}
			case '-': {
				int v = 1;
				if (isdigit(code[cp])) {
					v = parse_int(code, &cp);
				}
				memory[mp] -= v;
				break;
			}
			case '>': {
				int v = 1;
				if (isdigit(code[cp])) {
					v = parse_int(code, &cp);
				}
				mp = (mp + v) % MEMORY_SIZE;
				break;
			}
			case '<': {
				int v = 1;
				if (isdigit(code[cp])) {
					v = parse_int(code, &cp);
				}
				mp = (mp - v) % MEMORY_SIZE;
				break;
			}

			case 'p':
				for (size_t i = 0; i < MEMORY_SIZE; i++) {
					if (mp != i) {
						printf("%d ", memory[i]);
					} else printf("[%d] ", memory[i]);
				}
				printf("\n");
				break;

			case '.': {
        unsigned int temp = memory[mp];
				printf("%2x\n", temp);
				break;
      }
			case ',': {
        unsigned int temp;
				scanf("%2x", &temp);
        memory[mp] = temp;
				break;
      }

			case '[':
				if (memory[mp] == 0) {
					if (skip_brackets(code, &cp)) {
						return BF_ERROR_UNPAIRED_BRACKET;
					}
				} else {
					if (sp == sizeof(stack) - 1) {
						return BF_ERROR_STACK_OVERFLOW;
					} else {
						stack[sp++] = cp - 1;
					}
				}
				break;
			case ']':
				if (sp == 0) return BF_ERROR_STACK_UNDERFLOW;
				cp = stack[--sp];
				break;
		}
	}

	return 0;
}

long fsize(FILE* fd) {
	long cur = ftell(fd);
	fseek(fd, 0, SEEK_END);
	long result = ftell(fd);
	fseek(fd, cur, SEEK_SET);
	return result;
}

int fully_read(char* buffer, size_t length, FILE* fd) {
	size_t read = 0;
	while (read != length) {
		int c = fread(buffer, 1, length, fd);
		if (c == -1) return -1;
		read += c;
	}
	return 0;
}

int main(int argc, char** argv) {
	if (argc < 2) {
		print_usage(argv[0]);
		return EXIT_FAILURE;
	}
	
	const char* filename = argv[1];
	FILE* fd = fopen(filename, "rb");
	long size = fsize(fd);
	char* code = (char*) malloc(size + 1);
	
	int err = 0;
	
	if ((err = fully_read(code, size, fd)) != 0) {
		fprintf(stderr, "Couldn't read the file!\n");
		return EXIT_FAILURE;
	}
	code[size] = 0;
	
	if ((err = interpret_file(code)) != 0) {
		fprintf(stderr, "Couldn't interpret the file! %d\n", err);
		return EXIT_FAILURE;
	}
			
	return EXIT_SUCCESS;
}
