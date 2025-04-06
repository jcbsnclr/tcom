#pragma once

// for readability
#define QUEUE_TYPE_NAME(name) name##_t

// function signatures
#define QUEUE_ADD_SIG(name, type) static int name##_add(QUEUE_TYPE_NAME(name) * q, type val)
#define QUEUE_TAKE_SIG(name, type) static int name##_take(QUEUE_TYPE_NAME(name) * q, type * val)

// define structure with given name + _t
#define QUEUE_STRUCT_DECL(name, type, size) \
    typedef struct {                        \
        type buf[(size) + 1];               \
        size_t head, tail;                  \
    } QUEUE_TYPE_NAME(name);

// declare add function
#define QUEUE_ADD_DECL(name, type, size)      \
    QUEUE_ADD_SIG(name, type) {               \
        size_t new_head = q->head + 1;        \
                                              \
        if (new_head >= (size)) new_head = 0; \
        if (new_head == q->tail) return -1;   \
                                              \
        q->buf[q->head] = val;                \
        q->head = new_head;                   \
                                              \
        return 0;                             \
    }

// declare take function
#define QUEUE_TAKE_DECL(name, type, size)     \
    QUEUE_TAKE_SIG(name, type) {              \
        if (q->head == q->tail) return -1;    \
                                              \
        size_t new_tail = q->tail + 1;        \
        if (new_tail >= (size)) new_tail = 0; \
                                              \
        *val = q->buf[q->tail];               \
        q->tail = new_tail;                   \
                                              \
        return 0;                             \
    }

// all together now
#define QUEUE_DECL(name, type, size)     \
    QUEUE_STRUCT_DECL(name, type, size) \
    QUEUE_ADD_DECL(name, type, size)     \
    QUEUE_TAKE_DECL(name, type, size)
