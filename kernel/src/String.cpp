//
// Created by rhys on 19/11/22.
//

#include "String.h"


uint32 strlen(const char* c){
    uint32 len = 0;
    while(c[len]){
        len++;
    }
    return len;
}

char chr(uint32 code){
    return code^0b00110000;
}

void strcpy(char *dest, const char* src) {
    uint32 i=0;
    for(; i<strlen(src) ; i++){
        dest[i] = src[i];
    }
    dest[i] = 0;
}

void strcat(char *dest, const char* src) {
    uint32 start = strlen(src);
    uint32 i=0;
    for(; i<start ; i++){
        dest[start+i-2] = src[i]; // add 2 to ignore the (presumed) /0 at end of dest
    }
    dest[start + i -2] = 0;
}

void strlower(char *in) {
    for(uint32 i=0 ; i<strlen(in) ; i++){
        in[i] -= 32;
    }
}

void bzero(char* subj, uint32 size){
    for(uint32 i=0 ; i<size ; i++){
        subj[i] = 0;
    }
}

uint32 strcmp(const char* a, const char* b){
    if(strlen(a) != strlen(b)){
        return 1;
    }

    for(uint32 i=0 ; i<strlen(a) ; i++){
        if(a[i] != b[i]){
            return 1;
        }
    }

    return 0;
}

char* strtok(char* s, const char *delim){
    char *spanp;
    int c, sc;
    char *tok;
    static char *last;

    if (s == NULL && (s = last) == NULL) {
        return NULL;
    }

    finish:
    c = *s++;
    for (spanp = (char *)delim; (sc = *spanp++) != 0;) {
        if (c == sc)
            goto finish;
    }

    if (c == 0) {
        last = NULL;
        return (NULL);
    }
    tok = s - 1;

    while(TRUE){
        c = *s++;
        spanp = (char *)delim;
        do {
            if ((sc = *spanp++) == c) {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                last = s;
                return (tok);
            }
        } while (sc != 0);
    }
}