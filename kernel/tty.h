#ifndef TTY_H
#define TTY_H

#include <stdarg.h>

void printf(char* text, ...);
void fprintf(int fh, char* text, ...);
void vfprintf(int fh, char* text, va_list args);

#endif
