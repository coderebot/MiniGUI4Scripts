
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "glue_utils.h"

static pthread_key_t p_local_holder_key;
static pthread_once_t local_holder_alloc_key = PTHREAD_ONCE_INIT;

static void alloc_holder_key() {
    pthread_key_create(&p_local_holder_key, NULL);
}

ThreadLocalHolder::ThreadLocalHolder() {
    pthread_once(&local_holder_alloc_key, alloc_holder_key);

    prev = (ThreadLocalHolder*)pthread_getspecific(p_local_holder_key);

    pthread_setspecific(p_local_holder_key, (void*)this);
}

ThreadLocalHolder::~ThreadLocalHolder() {
    ThreadLocalHolder* thiz = (ThreadLocalHolder*)pthread_getspecific(p_local_holder_key);

    if (thiz == this) {
        pthread_setspecific(p_local_holder_key, (void*)prev);
    }

    for (vector<void*>::iterator it = values.begin(); it != values.end(); ++it) {
        void* p = *it;
        if (p) {
            free(p);
        }
    }
}

void ThreadLocalHolder::push(void* ptr) {
    if (ptr) {
        ThreadLocalHolder* thiz = (ThreadLocalHolder*)pthread_getspecific(p_local_holder_key);
        if (thiz)
            thiz->values.push_back(ptr);
    }
}

///////////////////////////////////////
WndTemplateBuilder& WndTemplateBuilder::setProp(int id, DWORD dwVal) {
    if (id < 1000) {
        props[id] = dwVal;
    } else if(id < 2000) {
        switch(id) {
        //case 1000: //className
        //    pwnd_tmpl->class_name = (const char*)dwVal;
        //    break;
        case 1001: //x
            pwnd_tmpl->x = (int)dwVal;
            break;
        case 1002: //y
            pwnd_tmpl->y = (int)dwVal;
            break;
        case 1003: //width
            pwnd_tmpl->w = (int)dwVal;
            break;
        case 1004: //height
            pwnd_tmpl->h = (int)dwVal;
            break;
        case 1005: //Text
            //TODO Set Window Text
            pwnd_tmpl->caption = (const char*)dwVal;
            break;
        case 1006: //Renderer
            //TODO Set Renderer
            break;
        case 1007: //BgColor
            pwnd_tmpl->bk_color = dwVal;
            break;
        case 1008: //font name
            pwnd_tmpl->font_name = (const char*)dwVal;
            break;
        }
    } else if (id < 3000) {
        //style
        pwnd_tmpl->style |= dwVal;
    } else if (id < 4000) {
        //ex style
        pwnd_tmpl->ex_style |= dwVal;
    } else {
        //TODO invalidate prop value
    }

    return *this;
}

WndTemplateBuilder& WndTemplateBuilder::setWndClassName(const char* className) {
    pwnd_tmpl->class_name = className;
    return *this;
}


void WndTemplateBuilder::build() {
    if (props.size() > 0) {
        int count = props.size();
        NCS_PROP_ENTRY *ps = new NCS_PROP_ENTRY[count + 1];
        int i = 0;
        for (map<int, DWORD>::iterator it = props.begin();
                it != props.end(); ++it) {
            ps[i].id = it->first;
            ps[i].value = it->second;
            i ++;
        }
        ps[i].id = 0;
        ps[i].value = 0;
        pwnd_tmpl->props = ps;
    }
}

