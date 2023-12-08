#ifndef __CCB_VS_H__
#define __CCB_VS_H__

#include <cstddef>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>

#include "../logs/log.h"

/*
    allow for custom allocator
    use #define ccb_vs_malloc <your_allocator> to define it
    stdlib maloc will be use if not define
*/
#ifndef ccb_vs_malloc
    #define ccb_vs_malloc malloc
#endif

/* formats utils
    usage: printf("text" CCB_VS_MT "text", CCB_VS_ARG(<ccb_vs variable>));
*/
#define CCB_VS_MT "%.*s"
#define CCB_VS_ARG(vs) vs.ptr, vs.len 

// struct 
typedef struct {
    const char* ptr;
    size_t len;
} ccb_vs;

// convert
const ccb_vs ccb_cst2vst(const char* string); // cstring to vstring
const char* ccb_vst2cst(const ccb_vs vstring); // vstring to cstring !! allocate mem

// compare ( ret == 0 => false and ret > 0 => true)
int ccb_vs_eq(const ccb_vs a, const ccb_vs b);
int ccb_vs_head(const ccb_vs string, const ccb_vs head);
int ccb_vs_tail(const ccb_vs string, const ccb_vs tail);
int ccb_vs_in(const ccb_vs string, const ccb_vs sub_string);

#define __CCB_VS_IMP__
#ifdef __CCB_VS_IMP__

const ccb_vs ccb_cst2vst(const char* string) {
    // find size
    size_t size;
    for (size=0; *string!='\0'; size++);

    // build struct
    return (const ccb_vs){.ptr=string, .len=size};
}

const char* ccb_vst2cst(const ccb_vs vstring) {
    char * string = (char*)malloc(sizeof(char)*(vstring.len+1));
    CCB_CHECK(string!=NULL, "Can't allocate string");
    
    // copy
    for (size_t i=0; i<vstring.len; i++) 
        string[i] = vstring.ptr[i];
    string[vstring.len] = '\0';

    return (const char*) string;
}

int ccb_vs_eq(const ccb_vs a, const ccb_vs b) {
    if (a.len != b.len) return 0;

    for(size_t i=0; i<a.len; i++){
        if (a.ptr[i] == b.ptr[i]) return 0;
    }
    return 1;
}

int ccb_vs_head(const ccb_vs string, const ccb_vs head) {
    if (string.len < head.len) return 0;

    for(size_t i=0; i<head.len; i++){
        if (string.ptr[i] == head.ptr[i]) return 0;
    }
    return 1;
}

int ccb_vs_tail(const ccb_vs string, const ccb_vs tail) {
    if (string.len < tail.len) return 0;

    for(size_t i=0; i<tail.len; i++){
        if (string.ptr[string.len-i-1] == tail.ptr[tail.len-i-1]) return 0;
    }
    return 1;
}

int ccb_vs_in(const ccb_vs string, const ccb_vs sub_string) {
    if (string.len < sub_string.len) return 0;

    for(size_t i=0; i<string.len-sub_string.len; i++){
        char found = 1;
        for (size_t j=0; j<sub_string.len; j++) {
            if (string.ptr[i+j] != sub_string.ptr[j]){
                found = 0;
                break;
            }
        }

        if (found)
            return 1;
    }
    return 0;
}


#endif
#endif
