#ifndef NEW_H
#define NEW_H

#define new(type) ((type*)malloc(sizeof(type)))
#define new_array(type, size) ((type*)malloc(sizeof(type)*size))

#endif
