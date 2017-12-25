#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <pthread.h>

#include <unistd.h>

#include "common.h"
#include "minigui.h"
#include "newgal.h"
#include "sysvideo.h"
#include "ial.h"

#include "mg-base.h"
#include "android-backend.h"

#define ENGINE "MG_GAL_ENGINE"
#define MODE "MG_DEFAULTMODE"
#define ANDROID_ENGIN "and-bmp"
#define IAL_ENGINE "MG_IAL_ENGINE"

typedef int bool;

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} CondSignal;

void init_cond_signal(CondSignal* pcs) {
    pthread_mutex_init(&(pcs->mutex), NULL);
    pthread_cond_init(&(pcs->cond), NULL);
}

void send_signal(CondSignal* pcs) {
    pthread_mutex_lock(&(pcs->mutex));
    pthread_cond_signal(&(pcs->cond));
    pthread_mutex_unlock(&(pcs->mutex));
}

void wait_signal(CondSignal* pcs) {
    pthread_mutex_lock(&(pcs->mutex));
    pthread_cond_wait(&(pcs->cond), &(pcs->mutex));
    pthread_mutex_unlock(&(pcs->mutex));
}

static CondSignal sUpdateRectSignal;

struct GAL_PrivateVideoData {
    int w, h;
    int pitch;
    unsigned char* buffer;
};

static void set_gal_mode(int width, int height) {
    char szMode[32];
    setenv(ENGINE, ANDROID_ENGIN, 1);
    sprintf(szMode, "%dx%d-16bpp", width, height);
    setenv(MODE, szMode, 1);
}


bool startMiniGUI(int width, int height) {
    set_gal_mode(width, height);
    //set ial engine
    setenv(IAL_ENGINE, ANDROID_ENGIN, 1);

    init_cond_signal(&sUpdateRectSignal);

    return InitGUI(0, NULL) == 0;
}


VideoBootStrap Android_bootstrap;

static int bitmap_Available(void)
{
    return(1);
}

static void Bitmap_DeleteDevice(GAL_VideoDevice *device)
{
    free(device->hidden);
    free(device);
}


static int Bitmap_VideoInit(GAL_VideoDevice* self, GAL_PixelFormat* vformat) {
    vformat->BitsPerPixel = 16;
    vformat->BytesPerPixel = 2;

    return 0;
}

static GAL_Rect **Bitmap_ListModes(GAL_VideoDevice* self, GAL_PixelFormat *vformat, Uint32 flags) {
    return (GAL_Rect**)-1;
}

static GAL_Surface *Bitmap_SetVideoMode(GAL_VideoDevice* self, GAL_Surface *current,
            int width, int height, int bpp, Uint32 flags) {
    int pitch;
    struct GAL_PrivateVideoData * data = self->hidden;

    if (data->buffer) {
        free(data->buffer);
    }

    pitch = width * ((bpp + 7)/8);
    pitch = (pitch + 3) & ~3;

    data->buffer = (unsigned char*)malloc(pitch * height);
    if (data->buffer == NULL) {
        ALOGE("MiniGUI GAL", "couldn't allocate buffer for %dx%d-%dbpp, (pitch=%d total=%d)", width, height, bpp, pitch, pitch* height);
        return NULL;
    }

    memset(data->buffer, 255, pitch * height);

    current->flags = flags & GAL_FULLSCREEN;
    data->w = current->w = width;
    data->h = current->h = height;
    data->pitch = pitch;
    current->pitch = pitch;
    current->pixels = data->buffer;

    return current;
}

static int Bitmap_AllocHWSurface(GAL_VideoDevice* self, GAL_Surface* surface) {
    return -1;
}

static void Bitmap_FreeHWSurface(GAL_VideoDevice* self, GAL_Surface *surface) {
    surface->pixels = NULL;
}

static int Bitmap_SetColors(GAL_VideoDevice* self, int firstcolor, int nrcolor, GAL_Color *colors) {
    return 1;
}

static void Bitmap_VideoQuit(GAL_VideoDevice * self) {
    if (self->screen->pixels != NULL) {
        free(self->screen->pixels);
        self->screen->pixels = NULL;
    }
}

static void Bitmap_UpdateRects(GAL_VideoDevice* self, int numrects, GAL_Rect *rects) {
    send_signal(&sUpdateRectSignal);
}


static GAL_VideoDevice *s_device;

static GAL_VideoDevice *bitmap_CreateDevice(int devindex)
{
    GAL_VideoDevice *device;

    /* Initialize all variables that we clean on shutdown */
    device = (GAL_VideoDevice *)malloc(sizeof(GAL_VideoDevice));
    if ( device ) {
        memset(device, 0, (sizeof *device));
        device->hidden = (struct GAL_PrivateVideoData*)(malloc(sizeof(struct GAL_PrivateVideoData)));
        memset(device->hidden, 0, sizeof *device->hidden);
    }
    if ( (device == NULL) || (device->hidden == NULL) ) {
        GAL_OutOfMemory();
        if ( device ) {
            free(device);
        }
        return(0);
    }
    memset(device->hidden, 0, (sizeof *device->hidden));

    /* Set the function pointers */
    device->VideoInit = Bitmap_VideoInit;
    device->ListModes = Bitmap_ListModes;
    device->SetVideoMode = Bitmap_SetVideoMode;
    device->SetColors = Bitmap_SetColors;
    device->VideoQuit = Bitmap_VideoQuit;
#ifndef _MGRM_THREADS
    device->RequestHWSurface = NULL;
#endif
    device->AllocHWSurface = Bitmap_AllocHWSurface;
    device->CheckHWBlit = NULL;
    device->FillHWRect = NULL;
    device->SetHWColorKey = NULL;
    device->SetHWAlpha = NULL;
    device->FreeHWSurface = Bitmap_FreeHWSurface;
    device->UpdateRects = Bitmap_UpdateRects;

    device->free = Bitmap_DeleteDevice;

    s_device = device;

    return device;
}



VideoBootStrap Android_bootstrap = {
    ANDROID_ENGIN, "android bitmap engine",
    bitmap_Available, bitmap_CreateDevice
};

void* get_surface_buffer(int *pwidth, int *pheight) {
    if (s_device == NULL){
        return NULL;
    }

    wait_signal(&sUpdateRectSignal);

    struct GAL_PrivateVideoData * data = s_device->hidden;
    if (data != NULL) {
        *pwidth = data->pitch;
        *pheight = data->h;
        return data->buffer;
    }

    return NULL;
}


/////////////////////////////////////////////////////////////////
/*
 * Mouse operations -- Event
 */

static EventData sEventData;
static int sEventFd;
static int ialFds[2];

int create_ial_fd() {
    pipe(ialFds);
    sEventFd = ialFds[0];
    return ialFds[1];
}


static void mouse_getxy (int *x, int* y)
{
    *x = sEventData.x;
    *y = sEventData.y;
}

static int mouse_getbutton (void)
{
    int buttons = 0;

    if (sEventData.button)
        buttons |= IAL_MOUSE_LEFTBUTTON;
    //if (s_pcxvfb_event_mouse.data.mouse.button & RIGHTBUTTON)
    //    buttons |= IAL_MOUSE_RIGHTBUTTON;

    return buttons;
}

static int mouse_update (void)
{
    return 1;
}

static int keyboard_update (void)
{
    return 0;
}

static const char* keyboard_getstate (void)
{
    return NULL;
}

static int wait_event(int which, int maxfd, fd_set* in, fd_set *out, fd_set *except,
        struct timeval *timeout)
{
    fd_set rfds;
    int fd, e;

    send_signal(&sUpdateRectSignal);
    if (!in) {
        in = &rfds;
        FD_ZERO(&rfds);
    }

    fd = sEventFd;

    if (fd > 0) {
        FD_SET(fd, in);
        if (fd > maxfd) {
            maxfd = fd;
        }
    }

    e = select(maxfd + 1, in, out, except, timeout);

    if (e > 0) {
        if (fd > 0 && FD_ISSET(fd, in)) {
            FD_CLR(fd, in);
            int ret = read(fd, &sEventData, sizeof(sEventData));
            if (ret == sizeof(sEventData)) {
                return IAL_MOUSEEVENT;
            }
        }
    } else if (e < 0) {
        return -1;
    }

    return 0;
}


BOOL InitAndroidInput (INPUT* input, const char* mdev, const char* mtype)
{
    input->update_mouse = mouse_update;
    input->get_mouse_xy = mouse_getxy;
    input->set_mouse_xy = NULL;
    input->get_mouse_button = mouse_getbutton;
    input->set_mouse_range = NULL;
    input->suspend_mouse= NULL;
    input->resume_mouse = NULL;

    input->update_keyboard = keyboard_update;
    input->get_keyboard_state = keyboard_getstate;
    input->suspend_keyboard = NULL;
    input->resume_keyboard = NULL;
    input->set_leds = NULL;

    input->wait_event = wait_event;


    return TRUE;
}

void TermAndroidInput (void)
{
    close(ialFds[0]);
    close(ialFds[1]);
    sEventFd = 0;
}



