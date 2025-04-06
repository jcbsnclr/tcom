// SPDX-License-Identifier: AGPL-3.0-only

#include <util/basic.h>
#include <util/err.h>

#include <errno.h>
#include <stdarg.h>
// #include <stdio.h>
#include <stdlib.h>
// #include <string.h>

#if TARGET == debug && defined(__GLIBC__)
#include <execinfo.h>
#endif

#include <unistd.h>

void *xalloc(size_t size) {
    void *ptr = calloc(1, size);

    if (!ptr) DIE(ELIBC, "failed to allocate %lu bytes", size);

    return ptr;
}

void *xrealloc(void *ptr_old, size_t size) {
    void *ptr = realloc(ptr_old, size);

    if (!ptr) DIE(ELIBC, "failed to realloc 0x%p to size %lu", ptr_old, size);

    return ptr;
}

void xclose(int fd) {
    if (close(fd) == -1) DIE(ELIBC, "failed to close fd %d", fd);
}
