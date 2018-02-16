#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <android/log.h>

struct Redirect {
    int level;
    int oldFds;
    int fds[2];

    Redirect(int l = 0)
    :level(l) {
        fds[0] = fds[1] = 0;
        oldFds = 0;
    }

    void dup(FILE* fp, int l) {
        pipe(fds);
        level = l;
	    fprintf(fp, "\n"); //active the fpout;
        oldFds = dup2(fds[1], fileno(fp));
    }

    int setFd(fd_set& fdset, int maxfd = 0) {
        if (fds[0] == 0) {
            return maxfd;
        }
        FD_SET(fds[0], &fdset);
        if (fds[0] > maxfd) {
            return fds[0];
        }
        return maxfd;
    }

    void checkAndRead(fd_set& fdset) {
        //__android_log_print(ANDROID_LOG_INFO, "==DJJ", "check:%d", fds[0]);
        if (!FD_ISSET(fds[0], &fdset)) {
            return ;
        }

        char szbuf[1024+1];

        while (1) {
            ssize_t r = read(fds[0], szbuf, sizeof(szbuf) - 1);
            //__android_log_print(ANDROID_LOG_INFO, "==DJJ", "get:%d size=%d", fds[0], r);
            if (r <= 0) break;
            szbuf[r] = 0;
            log(szbuf);

            if (r < sizeof(szbuf) - 1) {
                break;
            }
        }
    }


    void log(const char* str) {
        __android_log_print(level, "DUP MINIGUI", "%s", str);
    }


    ~Redirect() {
        if (oldFds > 0) {
            dup2(fds[1], oldFds);
            close(fds[0]);
            close(fds[1]);
        }
    }
};

static Redirect _redirects[2];

void * read_pipe_fds(void *) {
    fd_set fds;

    while(1) {
        int nfds = 0;
        FD_ZERO(&fds);

        for (int i = 0; i < sizeof (_redirects) / sizeof(_redirects[0]); i++) {
            nfds = _redirects[i].setFd(fds, nfds);
            //__android_log_print(ANDROID_LOG_INFO, "==DJJ", "nfds=%d", nfds);
        }

        if (select(nfds+1, &fds, NULL, NULL, NULL) > 0) {
            for (int i = 0; i < sizeof (_redirects) / sizeof(_redirects[0]); i++) {
                _redirects[i].checkAndRead(fds);
            }
        }
    }
}


void RedirectStdOut () {

    _redirects[0].dup(stdout, ANDROID_LOG_INFO);
    _redirects[1].dup(stderr, ANDROID_LOG_ERROR);

    pthread_t tid;
    pthread_create(&tid, NULL, read_pipe_fds, NULL);
}



