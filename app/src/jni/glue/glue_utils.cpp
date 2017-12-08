
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "glue_utils.h"
#include "glue_common.h"

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

bool getWidgetProperty(mWidget * widget, glue::Property *prop, PValue& val) {
    int id = prop->id;

    if (id < 1000) {
        val.set((unsigned long)(_c(widget)->getProperty(widget, id)));
    } else if (id < 2000) {
        switch(id) {
        case 1001: case 1002: case 1003: case 1004: {
            RECT rect;
            if(!GetWindowRect(widget->hwnd, &rect)) {
                return false;
            }
            int ival = 0;
            switch(id) {
            case 1001: ival = rect.left; break;
            case 1002: ival = rect.top; break;
            case 1003: ival = rect.right - rect.left; break;
            case 1004: ival = rect.bottom - rect.top; break;
            }
            val.set((unsigned long)ival);
            break;
        }
        case 1005: {
            char* text = NULL;
            int len = GetWindowTextLength(widget->hwnd);
            if (len > 0) {
                text = (char*)malloc(len + 1);
                GetWindowText(widget->hwnd, text, len);
            }
            val.set(text);
            break;
        }
        case 1006: //render
            return false; //TODO
        case 1007: //bkcolor
            val.set((unsigned long)GetWindowBkColor(widget->hwnd));
        case 1008: //font name
            return false;
        default:
            return false;
        }
    } else if (id < 4000) {
        //style & exstyle
        glue::PropType * ptype = prop->type;
        if (ptype->getType() != glue::PropType::ENUM) {
            return false;
        }

        glue::EnumType *penumType = (glue::EnumType*)ptype;

        DWORD dwStyle = id < 3000 ? GetWindowStyle(widget->hwnd) : GetWindowExStyle(widget->hwnd);
        string strval = penumType->getNameByValue(dwStyle);
        val.set(strval.c_str());
    } else {
        return false;
    }

    return true;
}

bool setWidgetProperty(mWidget * widget, glue::Property *prop, DWORD dwVal) {
    int id = prop->id;

    if (id < 1000) {
        return _c(widget)->setProperty(widget, id, dwVal) == TRUE;
    } else if (id < 2000) {
        switch(id) {
        case 1001: case 1002: case 1003: case 1004: {
            RECT rect;
            if(!GetWindowRect(widget->hwnd, &rect)) {
                return false;
            }
            int ival = (int)dwVal;
            switch(id) {
            case 1001: rect.left = ival; break;
            case 1002: rect.top = ival; break;
            case 1003: rect.right = rect.left + ival; break;
            case 1004: rect.bottom = rect.top + ival; break;
            }
            return MoveWindow(widget->hwnd, rect.left, rect.top, RECTW(rect), RECTH(rect), TRUE) == TRUE;
        }
        case 1005: {
            return SetWindowText(widget->hwnd, (const char*)dwVal) == TRUE;
        }
        case 1006: //render
            return false; //TODO
        case 1007: //bkcolor
            return SetWindowBkColor(widget->hwnd, dwVal) == TRUE;
        case 1008: //font name
            return false;
        default:
            return false;
        }
    } else if (id < 4000) {
        //style & exstyle
        glue::PropType * ptype = prop->type;
        if (ptype->getType() != glue::PropType::ENUM) {
            return false;
        }

        glue::EnumType *penumType = (glue::EnumType*)ptype;

        DWORD dwMask = penumType->getMask();
        if (id < 3000) {
            ExcludeWindowStyle(widget->hwnd, dwMask);
            IncludeWindowStyle(widget->hwnd, dwVal);
        } else {
            ExcludeWindowExStyle(widget->hwnd, dwMask);
            IncludeWindowExStyle(widget->hwnd, dwVal);
        }
    } else {
        return false;
    }

    return true;
}

