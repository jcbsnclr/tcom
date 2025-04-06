// SPDX-License-Identifier: AGPL-3.0-only

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct tcom tcom_t;
typedef const char *tcom_str_t;
typedef bool (*tcom_cb_t)(tcom_t *tc);

typedef struct {
    enum {
        TCOM_EOM
        // ...
    } kind;
    tcom_str_t msg;

    // if available:
    tcom_str_t fpath;
    size_t start, end;
} tcom_err_t;

typedef enum { TCOM_OKAY, TCOM_ERR } tcom_res_t;

typedef enum { TCOM_TRACE, TCOM_DEBUG, TCOM_INFO, TCOM_WARN, TCOM_ERROR } tcom_log_level_t;

typedef void (*tcom_log_cb_t)(tcom_log_level_t level, const char *filename, uint32_t line,
                              const char *func, const char *fmt, ...);

tcom_t *tcom_init();
void tcom_free(tcom_t *tc);

void tcom_set_log_cb(tcom_t *tc, tcom_log_cb_t cb);

// void tcom_get_err(tcom_t *tc, tcom_err_t *err);
// void tcom_clear_err(tcom_t *tc);

tcom_str_t tcom_str(tcom_t *tc, const char *str);
tcom_str_t tcom_str_len(tcom_t *tc, const char *str, size_t len);
tcom_str_t tcom_load(tcom_t *tc, const char *path);

tcom_res_t tcom_feed(tcom_t *tc, tcom_str_t str);
tcom_res_t tcom_parse(tcom_t *tc);

void tcom_def_cmd(tcom_t *tcom, tcom_str_t name, tcom_cb_t cb);
