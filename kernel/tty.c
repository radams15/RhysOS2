#include "tty.h"

#include <type.h>

#define FORMAT_MARK '%'

extern int stdin, stderr, stdout;

void fprinti(int fh, unsigned int num, int base) {
    char buffer[64];
    char* ptr = &buffer[sizeof(buffer) - 1];
    int remainder;

    *ptr = '\0';

    if (num == 0) {
        write(fh, "0", 1);
        return;
    }

    while (num != 0) {
        remainder = num % base;

        if (remainder < 10)
            *--ptr = '0' + remainder;
        else
            *--ptr = 'A' + remainder - 10;

        num /= base;
    }

    while (*ptr != '\0') {
        write(fh, ptr++, 1);
    }
}

void vfprintf(int fh, char* text, va_list args) {
    char skip_next = 0;
    int i;
    char formatter;

    for (i = 0; text[i] != NULL; i++) {
        if (skip_next) {
            skip_next = 0;
            continue;
        }

        if (text[i] == FORMAT_MARK) {
            formatter = text[i + 1];

            switch (formatter) {
                case 'd':  // int
                    fprinti(fh, va_arg(args, int), 10);
                    break;

                case 'c': {  // char
                    char c = va_arg(args, char);
                    write(fh, &c, 1);
                    break;
                }

                case 's': { // string
                    char* c = va_arg(args, char*);
                    write(fh, c, strlen(c));
                    break;
                }

                case 'x':  // hex int
                    fprinti(fh, va_arg(args, int), 16);
                    break;

                case FORMAT_MARK: {
                    char c = FORMAT_MARK;
                    write(fh, &c, 1);
                    break;
                }

                default:
                    break;
            }

            skip_next = 1;
        } else {
            write(fh, text+i, 1);
        }
    }
}

void fprintf(int fh, char* text, ...) {
    va_list ptr;
    va_start(ptr, text);

    vfprintf(fh, text, ptr);

    va_end(ptr);
}

void printf(char* text, ...) {
    va_list ptr;
    va_start(ptr, text);

    vfprintf(stdout, text, ptr);

    va_end(ptr);
}
