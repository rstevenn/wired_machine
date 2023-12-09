#ifndef __CCB_VS_H__
#define __CCB_VS_H__

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
#define CCB_VS_FMT "%.*s"
#define CCB_VS_ARG(vs) (int)vs.len, vs.ptr

// struct 
typedef struct {
    const char* ptr;
    size_t len;
} ccb_vs;

typedef struct {
    const ccb_vs front;
    const ccb_vs back;
    char found;
} ccb_vs_split;

// convert
const ccb_vs ccb_cst2vst(const char* string); // cstring to vstring
const char* ccb_vst2cst(const ccb_vs vstring); // vstring to cstring !! allocate mem

// compare ( ret == 0 => false and ret > 0 => true)
int ccb_vs_eq(const ccb_vs a, const ccb_vs b);
int ccb_vs_is_head(const ccb_vs string, const ccb_vs head);
int ccb_vs_is_tail(const ccb_vs string, const ccb_vs tail);
int ccb_vs_in(const ccb_vs string, const ccb_vs sub_string);

// split
ccb_vs_split ccb_vs_split_char(const ccb_vs string, char split_ch); // split on the 1st instance of char c
ccb_vs_split ccb_vs_split_set(const ccb_vs string, const char* split_ch, size_t nb_chs); // split on any char contain on split_ch
ccb_vs_split ccb_vs_split_string(const ccb_vs string, const ccb_vs split_string); // split on the 1st instance of string

// trim
const ccb_vs ccb_vs_triml(const ccb_vs string, size_t id); // "abcde", 2 => "cde" if id > len(string) => ""
const ccb_vs ccb_vs_trimr(const ccb_vs string, size_t id); // "abcde", 2 => "abc" if id > len(string) => ""

const ccb_vs ccb_vs_head(const ccb_vs string, size_t nb);  // "absde", 2 => "ab" id id > len(string) => string
const ccb_vs ccb_vs_tail(const ccb_vs string, size_t nb);  // "absde", 2 => "de" id id > len(string) => string

// implementation
#define __CCB_VS_IMP__
#ifdef __CCB_VS_IMP__

// trim
const ccb_vs ccb_vs_head(const ccb_vs string, size_t nb) {
    if (string.len < nb)
        return string;
    return (const ccb_vs){.ptr=string.ptr, .len=nb};
}

const ccb_vs ccb_vs_tail(const ccb_vs string, size_t nb) {
    if (string.len < nb)
        return string;
    return (const ccb_vs){.ptr=&string.ptr[string.len-nb], .len=nb};
}

const ccb_vs ccb_vs_triml(const ccb_vs string, size_t id) {
    if (string.len < id)
        return (const ccb_vs){.ptr=&string.ptr[string.len-1], .len=0};
    return (const ccb_vs){.ptr=&string.ptr[id], .len=string.len-id};
}

const ccb_vs ccb_vs_trimr(const ccb_vs string, size_t id) {
    if (string.len < id)
        return (const ccb_vs){.ptr=string.ptr, .len=0};
    return (const ccb_vs){.ptr=string.ptr, .len=string.len-id};
}

// split
ccb_vs_split ccb_vs_split_char(const ccb_vs string, char split_ch) {
    for (size_t i=0; i<string.len; i++) {
        if (string.ptr[i] == split_ch) {
            return (ccb_vs_split) { .front = (const ccb_vs){.ptr=string.ptr, .len=i},
                                    .back  = (const ccb_vs){.ptr=&string.ptr[i+1], .len=(string.len-i-1)},
                                    .found = 1};
        }
    }
    ccb_vs_split split = {.found=0};
    return split;
}

ccb_vs_split ccb_vs_split_set(const ccb_vs string, const char* split_ch, size_t nb_chs) {
    for (size_t i=0; i<string.len; i++) {
        if (ccb_vs_in((const ccb_vs){.ptr=split_ch, .len=nb_chs},
                      (const ccb_vs){.ptr=&string.ptr[i], .len=1})) {
            return (ccb_vs_split) { .front = (const ccb_vs){.ptr=string.ptr, .len=i},
                                    .back  = (const ccb_vs){.ptr=&string.ptr[i+1], .len=(string.len-i-1)},
                                    .found = 1};
        }
    }
    ccb_vs_split split = {.found=0};
    return split;
}

ccb_vs_split ccb_vs_split_string(const ccb_vs string, const ccb_vs split_string) {
    for (size_t i=0; i<string.len; i++) {
        if (ccb_vs_is_head((const ccb_vs){.ptr=&string.ptr[i], .len=(string.len-i)}, split_string)){
            return (ccb_vs_split) { .front = (const ccb_vs){.ptr=string.ptr, .len=i},
                                    .back  = (const ccb_vs){.ptr=&string.ptr[i+split_string.len], 
                                                            .len=(string.len-i-split_string.len)},
                                    .found = 1};
        }
    }
    ccb_vs_split split = {.found=0};
    return split;
}

// convert
const ccb_vs ccb_cst2vst(const char* string) {
    // find size
    size_t size;
    for (size=0; string[size]!='\0'; size++);

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

// compare
int ccb_vs_eq(const ccb_vs a, const ccb_vs b) {
    if (a.len != b.len) return 0;

    for(size_t i=0; i<a.len; i++){
        if (a.ptr[i] != b.ptr[i]) return 0;
    }
    return 1;
}

int ccb_vs_is_head(const ccb_vs string, const ccb_vs head) {
    if (string.len < head.len) return 0;

    for(size_t i=0; i<head.len; i++){
        if (string.ptr[i] != head.ptr[i]) return 0;
    }
    return 1;
}

int ccb_vs_is_tail(const ccb_vs string, const ccb_vs tail) {
    if (string.len < tail.len) return 0;

    for(size_t i=0; i<tail.len; i++){
        if (string.ptr[string.len-i-1] != tail.ptr[tail.len-i-1]) return 0;
    }
    return 1;
}

int ccb_vs_in(const ccb_vs string, const ccb_vs sub_string) {
    if (string.len < sub_string.len) return 0;

    for(size_t i=0; i<string.len-(sub_string.len-1); i++){
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
