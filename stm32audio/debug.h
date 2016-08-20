#ifndef DEBUG_H 
#define DEBUG_H
#ifdef __cplusplus
extern "C" {
#endif

#ifdef ARDUINO 
#define dprintf(...)
#define dshow(expression) 
#else
#define dprintf(...) printf(__VA_ARGS__)
#define dshow(expression) 
#endif

#ifdef __cplusplus
}
#endif
#endif
