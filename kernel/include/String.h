//
// Created by rhys on 19/11/22.
//

#ifndef RHYSOS_STRING_H
#define RHYSOS_STRING_H

#include "Types.h"

char chr(uint32 code);

void strcpy(char* dest, const char* src);
void strcat(char *dest, const char *src);

void strlower(char* in);

uint32 strlen(const char* subj);

void bzero(char* subj, uint32 size);

uint32 strcmp(const char* a, const char* b);

char* strtok(char* s, const char *delim);

#endif //RHYSOS_STRING_H
