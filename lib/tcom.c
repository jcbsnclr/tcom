#include <stdlib.h>
#include <tcom.h>
#include <assert.h>

typedef uint64_t hash_t;

typedef struct {
    size_t rcount;
    size_t len;
    uint64_t hash;
    char data[];
} hdr_t;

static inline hdr_t *hdr(tcom_str_t str) {
    return (hdr_t *)str - 1;
}

typedef struct cmd_def {
    tcom_str_t name;
    tcom_cb_t cb;
    struct cmd_def *next;
} cmd_def_t;

struct tcom {
    cmd_def_t *cmds;

    char *src;
    size_t len, cap, ptr;

    tcom_log_cb_t log;
};

typedef bool (*ccond_t)(char);

#define LOG(tc, l, ...) tc->log((l), __FILE__, __LINE__, __func__, __VA_ARGS__)

tcom_t *tcom_init(void) {
    tcom_t *tc = malloc(sizeof(tcom_t));
    assert(tc);

    return tc;
}

void tcom_free(tcom_t *tc) {
    free(tc);
}

void tcom_set_log_cb(tcom_t *tc, tcom_log_cb_t cb) {
    assert(tc);
    assert(cb);

    tc->log = cb;

    LOG(tc, TCOM_INFO, "Hello, World!");
}
