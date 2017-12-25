#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mg-base.h"
#include "get_icudata_name.h"

#define ANDROID_ICU_DATA_PATH "/system/usr/icu"
#define ICUDATA "icudt"

namespace hybrid_j2v8 {

IcuName::IcuName() {
    char szICUPath[256];
    char icudata_name[64];
    strcpy(szICUPath, ANDROID_ICU_DATA_PATH);

    int len = strlen(szICUPath);

    DIR * dir = opendir(szICUPath);
    struct dirent* dent = NULL;
    while((dent = readdir(dir))) {
        if (strncmp(dent->d_name, ICUDATA, sizeof(ICUDATA) - 1) == 0) {
            struct stat s;
            if (stat(szICUPath, &s) == 0) {
                strcpy(icudata_name, dent->d_name);
                char* str = (char*)strchr(icudata_name, '.');
                if (str) {
                    *str = '\0';
                    break;
                }
            }
        }
    }

    closedir(dir);

    name = icudata_name;
}

IcuName::HashKey IcuName::hash(const char* sufix) {
    if (sufix == NULL || sufix[0] == '\0') {
        return 0;
    }

    HashKey h = sufix[0];
    for (int i = 1; sufix[i]; i++) {
        h = (h << 5) - h + sufix[i];
    }

    return h;
}

IcuName& IcuName::getInstance() {
    static IcuName icuName;
    return icuName;
}

ICUDATA_EXTERN const char* get_icudata_name(const char* surfix) {
    return IcuName::getInstance() + surfix;
}

}
