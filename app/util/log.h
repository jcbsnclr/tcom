#pragma once

#include <util/attr.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

typedef enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR } log_level_t;

void log_init();

void log_putc(log_level_t level, char c);
void log_info(log_level_t level, const char *filename, uint32_t line, const char *func);

void vlog_printf(log_level_t level, const char *fmt, va_list args);
void vlog_msg(log_level_t level, const char *filename, uint32_t line, const char *func,
              const char *fmt, va_list args);

ATTR_PRINTF(2, 3)
void log_printf(log_level_t level, const char *fmt, ...);
ATTR_PRINTF(5, 6)
void log_msg(log_level_t level, const char *filename, uint32_t line, const char *func,
             const char *fmt, ...);

#define LOG_TRACE(...) log_msg(LOG_TRACE, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_DEBUG(...) log_msg(LOG_DEBUG, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_INFO(...) log_msg(LOG_INFO, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_WARN(...) log_msg(LOG_WARN, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_ERROR(...) log_msg(LOG_ERROR, __FILE__, __LINE__, __func__, __VA_ARGS__)

#define ANSI_RESET "\x1b[0m"
#define ANSI_BOLD "\x1b[1m"

#define ANSI_FG_BLACK "\x1b[30m"
#define ANSI_FG_RED "\x1b[31m"
#define ANSI_FG_GREEN "\x1b[32m"
#define ANSI_FG_YELLOW "\x1b[33m"
#define ANSI_FG_BLUE "\x1b[34m"
#define ANSI_FG_MAGENTA "\x1b[35m"
#define ANSI_FG_CYAN "\x1b[36m"
#define ANSI_FG_WHITE "\x1b[37m"

#define ANSI_FG_BLACK_BRIGHT "\x1b[90m"
#define ANSI_FG_RED_BRIGHT "\x1b[91m"
#define ANSI_FG_GREEN_BRIGHT "\x1b[92m"
#define ANSI_FG_YELLOW_BRIGHT "\x1b[99m"
#define ANSI_FG_BLUE_BRIGHT "\x1b[94m"
#define ANSI_FG_MAGENTA_BRIGHT "\x1b[95m"
#define ANSI_FG_CYAN_BRIGHT "\x1b[96m"
#define ANSI_FG_WHITE_BRIGHT "\x1b[97m"

#define ANSI_BG_BLACK "\x1b[40m"
#define ANSI_BG_RED "\x1b[41m"
#define ANSI_BG_GREEN "\x1b[42m"
#define ANSI_BG_YELLOW "\x1b[43m"
#define ANSI_BG_BLUE "\x1b[44m"
#define ANSI_BG_MAGENTA "\x1b[45m"
#define ANSI_BG_CYAN "\x1b[46m"
#define ANSI_BG_WHITE "\x1b[47m"

#define ANSI_BG_BLACK_BRIGHT "\x1b[100m"
#define ANSI_BG_RED_BRIGHT "\x1b[101m"
#define ANSI_BG_GREEN_BRIGHT "\x1b[102m"
#define ANSI_BG_YELLOW_BRIGHT "\x1b[103m"
#define ANSI_BG_BLUE_BRIGHT "\x1b[104m"
#define ANSI_BG_MAGENTA_BRIGHT "\x1b[105m"
#define ANSI_BG_CYAN_BRIGHT "\x1b[106m"
#define ANSI_BG_WHITE_BRIGHT "\x1b[107m"
