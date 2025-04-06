// SPDX-License-Identifier: AGPL-3.0-only

#pragma once

#include <stddef.h>
#include <string.h>

typedef int errno_t;

// #define FMT_PRINTF(fmt, va) __attribute__((format (printf, (fmt), (va))))
// errno_t read_file(const char *path, char **data, off_t *len);

void *xalloc(size_t size);
void *xrealloc(void *ptr, size_t size);

void xclose(int fd);

#define NEW(type) xalloc(sizeof(type))

#define ZERO_OF_N(ptr, type, n) memset((ptr), 0, sizeof(type) * (n))
#define ZERO_N(ptr, n) ZERO_OF_N((ptr), typeof(*ptr), (n))
#define ZERO_OF(ptr, type) ZERO_OF_N(ptr, type, 1)
#define ZERO(ptr) ZERO_N(ptr, 1)

#define MAX(n1, n2) ((n1) > (n2) ? n1 : n2)
#define MIN(n1, n2) ((n1) < (n2) ? n1 : n2)
