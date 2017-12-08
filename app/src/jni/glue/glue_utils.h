#ifndef GLUE_UTILS_H
#define GLUE_UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

namespace glue {
struct Property;
};


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

struct PValue {
    union {
        const char* strval;
        unsigned long uval;
        long lval;
    }d;

    int type;

    enum {
        NONE,
        STRING,
        LONG,
        ULONG
    };

    PValue() {
        d.lval = 0;
        type = NONE;
    }

    void set(const char* strval) {
        d.strval = strval ? strdup(strval) : NULL;
        type = STRING;
    }

    void set(char* strval) {
        d.strval = strval;
        type = STRING;
    }

    void set(long val) {
        d.lval = val;
        type = LONG;
    }
    void set(unsigned long uval) {
        d.uval = uval;
        type = ULONG;
    }

    ~PValue() {
        if (type == STRING && d.strval) {
            free((void*)d.strval);
        }
    }
};

bool getWidgetProperty(mWidget * widget, glue::Property *prop, PValue& val);
bool setWidgetProperty(mWidget * widget, glue::Property *prop, DWORD dwVal);

#endif

