// SPDX-License-Identifier: AGPL-3.0-only

// #include <ctype.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <tcom.h>

#include <util/log.h>
#include <util/err.h>

#include <stddef.h>
#include <string.h>
#include <errno.h>

#include "util/basic.h"

#define TOKENS      \
    X(NONE)         \
    X(LIT)          \
                    \
    X(CURLY_LEFT)   \
    X(CURLY_RIGHT)  \
    X(SQUARE_LEFT)  \
    X(SQUARE_RIGHT) \
                    \
    X(SUBST)        \
                    \
    X(QUOTE)        \
    X(SEMICOLON)    \
                    \
    X(COMMENT)      \
    X(WHITESPACE)

#define X(n) TOK_##n,
typedef enum { TOKENS TOK_MAX } kind_t;
#undef X

#define X(n) [TOK_##n] = #n,
const char *kind_str[TOK_MAX] = {TOKENS};
#undef X

typedef struct {
    size_t start, end;
    kind_t kind;
} token_t;

typedef struct {
    const char *src;
    size_t ptr, len;
    token_t peek;
} lexer_t;

typedef enum { MODE_EVAL, MODE_EXPAND, MODE_STRING } mode_t;

/*
TOK_LIT

"asdas $asdadsads"
asdasd
*/

typedef bool (*ccond_t)(char);

// static bool is_not_tok_end(char c) {
//     return !isspace(c) && c != '#';
// }

static bool is_eof(lexer_t *lx) {
    return lx->ptr >= lx->len;
}

static bool peek(lexer_t *lx, char *out) {
    if (is_eof(lx)) return false;
    // TRY_BOOL(is_eof(lx));

    *out = lx->src[lx->ptr];
    return true;
}

static bool take(lexer_t *lx, char *out) {
    TRY_BOOL(peek(lx, out));
    lx->ptr++;

    return true;
}

static bool is_any(char c, char *pat) {
    while (*pat) {
        if (c == *(pat++)) return true;
    }
    return false;
}

static bool take_if(lexer_t *lx, ccond_t cond) {
    char c;
    TRY_BOOL(peek(lx, &c));

    if (cond(c)) {
        take(lx, &c);
        return true;
    }

    return false;
}

// static bool take_if_any(lexer_t *lx, char *pat) {
//     if (lx->ptr >= lx->len) return false;

//     if (is_any(lx->src[lx->ptr], pat)) {
//         lx->ptr++;
//         return true;
//     }

//     return false;
// }

static bool take_ifc(lexer_t *lx, char exp) {
    char c;
    TRY_BOOL(peek(lx, &c));

    if (c == exp) {
        take(lx, &c);
        return true;
    }

    return false;
}

static bool take_while(lexer_t *lx, ccond_t cond) {
    bool taken = false;

    while (take_if(lx, cond)) taken = true;

    return taken;
}

static bool is_ws(char c) {
    return isspace(c);
}

static bool is_symbol(char c) {
    return !is_ws(c) && !is_any(c, "{}[];");
}

static bool is_not_nl(char c) {
    return c != '\n';
}

static bool take_subst(lexer_t *lx) {
    TRY_BOOL(take_ifc(lx, '$'));
    take_while(lx, is_symbol);

    return true;
}

static bool take_comment(lexer_t *lx) {
    TRY_BOOL(take_ifc(lx, '#'));
    take_while(lx, is_not_nl);

    return true;
}

static bool lexer_take(lexer_t *lx, token_t *out, mode_t mode) {
    (void)mode;

    token_t tok;
    tok.start = lx->ptr;

    if (take_while(lx, is_ws))
        tok.kind = TOK_WHITESPACE;
    else if (take_ifc(lx, '{'))
        tok.kind = TOK_CURLY_LEFT;
    else if (take_ifc(lx, '}'))
        tok.kind = TOK_CURLY_RIGHT;
    else if (take_ifc(lx, '['))
        tok.kind = TOK_SQUARE_LEFT;
    else if (take_ifc(lx, ']'))
        tok.kind = TOK_SQUARE_RIGHT;
    else if (take_ifc(lx, ';'))
        tok.kind = TOK_SEMICOLON;
    else if (take_ifc(lx, '"'))
        tok.kind = TOK_QUOTE;
    else if (take_subst(lx))
        tok.kind = TOK_SUBST;
    else if (take_comment(lx))
        tok.kind = TOK_COMMENT;
    else if (take_while(lx, is_symbol))
        tok.kind = TOK_LIT;
    else
        return false;

    tok.end = lx->ptr;
    *out = tok;

    return true;
}

// static bool lexer_peek(lexer_t *lx, token_t *out, mode_t mode) {
//     TRY_BOOL(lexer_take(lx, out, mode));
//     lx->ptr = out->start;
//     return true;
// }

// typedef struct {

// } cmd_t;

typedef struct {
    lexer_t lx;
} parser_t;

typedef struct ctx ctx_t;

typedef bool (*cb_t)(struct ctx *, parser_t *p);
typedef struct cmd_def {
    const char *name;
    cb_t cb;

    struct cmd_def *next;
} cmd_def_t;

typedef struct var_def {
    const char *name;
    const char *val;

    struct var_def *next;
} var_def_t;

struct ctx {
    cmd_def_t *defs;
    var_def_t *vars;
};

static cmd_def_t *ctx_lookup(ctx_t *ctx, const char *name) {
    cmd_def_t *cur = ctx->defs;
    while (cur) {
        if (strcmp(cur->name, name) == 0) return cur;

        cur = cur->next;
    }

    return NULL;
}

static void ctx_insert(ctx_t *ctx, const char *name, cb_t cb) {
    cmd_def_t *new = xalloc(sizeof(cmd_def_t));

    new->cb = cb;
    new->name = name;
    new->next = ctx->defs;

    ctx->defs = new;
}

/*



*/

int main(int argc, const char *argv[]) {
    (void)ctx_insert;
    (void)ctx_lookup;

    (void)argc;
    (void)argv;

    log_init();

    tcom_t *tc = tcom_init();
    tcom_set_log_cb(tc, (tcom_log_cb_t)log_msg);

    tcom_free(tc);

    return 0;

    // DIE(0, "LMAO");

    const size_t CHUNK = 0x1000;

    size_t cap = CHUNK;
    size_t len = 0;
    char *buf = xalloc(cap);

    for (;;) {
        if (len + CHUNK >= cap) {
            cap *= 2;
            buf = xrealloc(buf, cap);
        }

        size_t res = fread(buf + len, 1, CHUNK, stdin);
        len += res;

        if (res == 0) {
            if (ferror(stdin)) {
                free(buf);
                DIE(errno, "failed to read script from stdin");
            }

            break;
        }
    }

    lexer_t lx;
    lx.src = buf;
    lx.ptr = 0;
    lx.len = len;

    LOG_INFO("source:\n%.*s", (int)lx.len, lx.src);
    token_t tok;
    // while (lexer_take(&lx, &tok, 0))
    //     if (tok.kind != TOK_WHITESPACE)
    //         LOG_TRACE("  %s %lu->%lu -> '%.*s'",
    //                   kind_str[tok.kind],
    //                   tok.start,
    //                   tok.end,
    //                   (int)(tok.end - tok.start),
    //                   lx.src + tok.start);

    // loop over commands
    for (;;) {
        if (!lexer_take(&lx, &tok, MODE_EVAL))
            break;
        else if (tok.kind == TOK_WHITESPACE || tok.kind == TOK_COMMENT)
            continue;
        else if (tok.kind != TOK_LIT)
            DIE(0, "command name must be TOK_LIT");

        const char *str = lx.src + tok.start;
        size_t len = tok.end - tok.start;

        LOG_INFO("start %.*s", (int)len, str);

        // loop over args
        for (;;) {
            token_t arg;
            bool taken = !lexer_take(&lx, &arg, MODE_EVAL);

            if (taken || arg.kind == TOK_SEMICOLON) {
                LOG_INFO("end '%.*s'", (int)len, str);
                break;
            }

            LOG_DEBUG("  arg = %.*s", (int)(arg.end - arg.start), lx.src + arg.start);
        }
    }

    return 0;
}
