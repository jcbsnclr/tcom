#pragma once

#define ATTR_PRINTF(fmt, va) [[gnu::format(printf, (fmt), (va))]]
#define ATTR_NORETURN [[noreturn]]
#define ATTR_TLOCAL __thread
