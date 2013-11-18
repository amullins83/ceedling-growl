#include "for_each.h"
#include <stdarg.h>

void for_each(void **arr, void (*callback)(void *element, va_list), ...) {
    int i;
    va_list args;
    va_start(args, callback);

    for(i = 0; arr[i] != 0 ; i++)
        callback(arr[i], args);

    va_end(args);
}
