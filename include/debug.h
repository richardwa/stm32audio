#ifndef DEBUG_H 
#define DEBUG_H
#ifdef __cplusplus
extern "C" {
#endif
#define DEBUG 0

#ifdef ARDUINO
#define dprintf(...)
#define dshow(expression) 
#elif DEBUG == 0 
#define dprintf(...)
#define dshow(expression) 
#else
#include <stdio.h>
#define dprintf(...) printf(__VA_ARGS__)
#define dshow(expression) printf("# %s: %d", #expression , expression )
#endif

#ifdef __cplusplus
}
#endif
#endif
