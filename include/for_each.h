#ifndef FOR_EACH_H
#define FOR_EACH_H
#include <stdarg.h>

void for_each(void **arr, void (*callback)(void *element, va_list), ...);

#endif
