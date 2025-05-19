#ifndef STRING_H
#define STRING_H

#define strcpy(a, b) memcpy(a, b, strlen(b))
#define strcpyz(a, b) \
    strcpy(a, b);     \
    a[strlen(b)] = 0

void memcpy(char* dest, char* src, unsigned int n);

int strlen(char* str);

int strncmp(char* a, char* b, int length);

int strcmp(char* a, char* b);

char* strtok(char* s, char* delim);


#endif
