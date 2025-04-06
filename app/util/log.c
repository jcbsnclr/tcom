#include <util/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static log_level_t log_filter = LOG_TRACE;
static FILE *log_outp;

const char *log_level_str[] = {
    [LOG_TRACE] = "" ANSI_BOLD ANSI_FG_MAGENTA "TRACE" ANSI_RESET,
    [LOG_DEBUG] = "" ANSI_BOLD ANSI_FG_CYAN "DEBUG" ANSI_RESET,
    [LOG_INFO] = "" ANSI_BOLD ANSI_FG_GREEN "INFO " ANSI_RESET,
    [LOG_WARN] = "" ANSI_BOLD ANSI_FG_YELLOW "WARN " ANSI_RESET,
    [LOG_ERROR] = "" ANSI_BOLD ANSI_FG_RED_BRIGHT "ERROR" ANSI_RESET,
};

void log_init() {
    log_outp = stderr;

    char *val = getenv("LOG_FILTER");

    if (!val)
        return;
    else if (strcmp(val, "trace") == 0)
        log_filter = LOG_TRACE;
    else if (strcmp(val, "debug") == 0)
        log_filter = LOG_DEBUG;
    else if (strcmp(val, "info") == 0)
        log_filter = LOG_INFO;
    else if (strcmp(val, "warn") == 0)
        log_filter = LOG_WARN;
    else if (strcmp(val, "error") == 0)
        log_filter = LOG_ERROR;
}

void vlog_printf(log_level_t level, const char *fmt, va_list args) {
    if (level < log_filter) return;

    vfprintf(log_outp, fmt, args);
}

void log_printf(log_level_t level, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    vlog_printf(level, fmt, args);

    va_end(args);
}

void log_putc(log_level_t level, char c) {
    if (level < log_filter) return;
    fputc(c, log_outp);
}

void log_info(log_level_t level, const char *filename, uint32_t line, const char *func) {
    log_printf(level,
               "%s [" ANSI_FG_BLACK_BRIGHT "%s:%d" ANSI_RESET " " ANSI_FG_CYAN "%s" ANSI_RESET
               "]: ",
               log_level_str[level],
               filename,
               line,
               func);
}

void vlog_msg(log_level_t level, const char *filename, uint32_t line, const char *func,
              const char *fmt, va_list args) {
    log_info(level, filename, line, func);

    vlog_printf(level, fmt, args);
    log_putc(level, '\n');
}

void log_msg(log_level_t level, const char *filename, uint32_t line, const char *func,
             const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    vlog_msg(level, filename, line, func, fmt, args);

    va_end(args);
}
