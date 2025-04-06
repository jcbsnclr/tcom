#include <util/err.h>
#include <util/log.h>

#if TARGET == debug && defined(__GLIBC__)
#include <execinfo.h>
#endif

// #define UNW_LOCAL_ONLY
// #include <libunwind.h>

#include <stdlib.h>
#include <string.h>

#define TERM_KV(key, fmt) "" ANSI_FG_CYAN_BRIGHT key ANSI_RESET " = " fmt
#define TERM_STR(fmt) "" ANSI_FG_YELLOW "'" fmt "'" ANSI_RESET

void die(res_t res, const char *filename, uint32_t line, const char *func, const char *fmt, ...) {
    log_info(LOG_ERROR, filename, line, func);

    log_printf(LOG_ERROR, "" ANSI_BOLD "fatal" ANSI_RESET);
    if (res < 0)
        log_printf(LOG_ERROR,
                   " (" TERM_KV("errno", "%ld") " " TERM_STR("%s") ANSI_RESET "): ",
                   res,
                   strerror(res));
    // log_printf(LOG_ERROR,
    //            " (" ANSI_FG_CYAN_BRIGHT "errno" ANSI_RESET " = %d " ANSI_FG_YELLOW
    //            "'%s'" ANSI_RESET "): ",
    //            err,
    //            strerror(err));
    else
        log_printf(LOG_ERROR, ": ");

    va_list args;
    va_start(args, fmt);

    vlog_printf(LOG_ERROR, fmt, args);

    va_end(args);

    log_putc(LOG_ERROR, '\n');

    // #if TARGET == debug && defined(__GLIBC__)
    //     void *bt[32];
    //     size_t size = backtrace(bt, 32);
    //     char **strings = backtrace_symbols(bt, size);

    //     if (strings) {
    //         log_printf(LOG_ERROR, "Backtrace:\n");
    //         for (size_t i = 1; i < size; i++) {
    //             log_printf(LOG_ERROR,
    //                        "  " ANSI_FG_BLACK_BRIGHT "[" ANSI_FG_GREEN "%lu" ANSI_FG_BLACK_BRIGHT
    //                        "]" ANSI_RESET " %s\n",
    //                        i,
    //                        strings[i]);
    //         }

    //         free(strings);
    //     }
    // #endif

    // unw_cursor_t cur;
    // unw_context_t uc;
    // unw_word_t ip, sp;

    // unw_getcontext(&uc);
    // unw_init_local(&cur, &uc);
    // log_printf(LOG_ERROR, "Backtrace:\n");
    // while (unw_step(&cur) > 0) {
    //     unw_get_reg(&cur, UNW_REG_IP, &ip);
    //     unw_get_reg(&cur, UNW_REG_SP, &sp);

    //     char buf[128];
    //     unw_get_proc_name(&cur, buf, 128, NULL);

    //     log_printf(LOG_ERROR, "  * %lu @ %lu        %s\n", ip, sp, buf);
    // }

    exit(1);
}
