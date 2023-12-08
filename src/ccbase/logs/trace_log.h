#ifndef __CCB_TRACE_LOG_H__
#define __CCB_TRACE_LOG_H__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    void* value;
    unsigned int ok;
    char* msg;
    char current_type[32];
} ccb_maybe_t;


#define CCB_TRACE_ADD(returned) { \
    size_t ccb_len = strlen(returned.msg); \
    size_t ccb_next_len = ccb_len + strlen("traced from: \n" __FILE__ "::") + sizeof(__FUNCTION__) + sizeof("()::") + 8; \
    returned.msg = (char*)realloc(returned.msg, sizeof(char)*ccb_next_len); \
    \
    if (returned.msg == NULL) {\
        fprintf(stderr, "Trace Log, can't allocate mem to trace"); exit(1); \
    } \
    returned.msg = strcat(returned.msg, "traced from: " __FILE__ "::"); \
    returned.msg = strcat(returned.msg, __FUNCTION__); \
    returned.msg = strcat(returned.msg, "()::"); \
    char ccb_tmp_line[16]; sprintf(ccb_tmp_line, "%d\n", __LINE__); \
    returned.msg = strcat(returned.msg, ccb_tmp_line); \
}


#define CCB_TRACE_DECODE_MAYBE(returned, type, var) { \
    if (returned.ok) { \
        if(strcmp(#type, returned.current_type)!=0) { \
            printf( "[     ERROR     ] %s::%s()::l%d: invalid decoding type: expected %s got %s\n", __FILE__, __FUNCTION__, __LINE__, returned.current_type, #type); \
        } \
        var = *(type*)returned.value; \
        free(returned.value); \
            \
    } else { \
        CCB_TRACE_ADD(returned); \
        return returned;\
    }\
} 


#define CCB_TRACE_RETURN_KO(...) { \
    ccb_maybe_t ccb_return;\
    ccb_return.ok = 0; \
    ccb_return.msg = (char*)malloc(sizeof(char)*1024); \
    \
    if (ccb_return.msg == NULL) { \
        fprintf(stderr, "Trace Log, can't allocate mem to trace"); \
        exit(1);\
    } \
    sprintf(ccb_return.msg, __VA_ARGS__); \
    ccb_return.msg = strcat(ccb_return.msg, "\n"); \
    CCB_TRACE_ADD(ccb_return); \
    return ccb_return;\
}


#define CCB_TRACE_RETURN_OK(type, var) { \
    ccb_maybe_t ccb_return; \
    ccb_return.ok = 1; \
    ccb_return.value = malloc(sizeof(type)); \
    \
    sprintf(ccb_return.current_type, "%s", #type); \
    if (ccb_return.value == NULL) { \
        fprintf(stderr, "Trace ok, can't allocate value to return"); \
        exit(1);\
    } \
    ((type *)ccb_return.value)[0] = var; \
    return ccb_return;\
}

#endif