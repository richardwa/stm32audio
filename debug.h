#ifndef DEBUG_H 
#define DEBUG_H

#define DEBUG 0
#if DEBUG == 1
#define dprintf(...) printf(__VA_ARGS__)
#else
#define dprintf(...)
#endif

#endif
