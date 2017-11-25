#ifndef ANDROID_BACKEND_H
#define ANDROID_BACKEND_H

#ifdef __cplusplus
#define EXPORT extern "C"
#else
#define EXPORT extern
#endif

typedef struct {
    int x;
    int y;
    int button;
} EventData;


EXPORT int create_ial_fd();


#endif

