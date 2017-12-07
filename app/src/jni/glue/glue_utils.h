#ifndef GLUE_UTILS_H
#define GLUE_UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <map>
#include "common.h"
#include "minigui.h"
#include "gdi.h"
#include "window.h"
#include "mgncs.h"

using std::vector;
using std::map;

class ThreadLocalHolder {
    ThreadLocalHolder* prev;

    vector<void*> values;
public:
    ThreadLocalHolder();
    ~ThreadLocalHolder();

    static void push(void* ptr);
};


class WndTemplateBuilder {
    NCS_WND_TEMPLATE* pwnd_tmpl;
    map<int, DWORD> props;

public:
    WndTemplateBuilder(NCS_WND_TEMPLATE* wnd_tmpl) : pwnd_tmpl(wnd_tmpl) {
    }

    WndTemplateBuilder& setProp(int id, DWORD val);
    WndTemplateBuilder& setWndClassName(const char* className);

    void build();
};

#endif

