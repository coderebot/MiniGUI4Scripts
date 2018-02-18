#include <stdio.h>
#include <string.h>
#include <stdint.h>
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "mg-base.h"
#include "common.h"
#include "minigui.h"
#include "gdi.h"
#include "window.h"
#include "mgncs.h"
#include "glue_common.h"
#include "glue_utils.h"
using namespace glue;

namespace lua_glue {

static lua_State *gL;

static const char* toHolderString(lua_State* L, int idx) {
    const char* str_value = lua_tostring(L, idx);
    if (!str_value) return NULL;
    char* dup_str = strdup(str_value);
    ThreadLocalHolder::push(dup_str);
    return dup_str;
}

static mWidget* getWrapWidget(lua_State* L, int idx) {
    if (lua_isnumber(L, idx)) {
        return (mWidget*)lua_tointeger(L, idx);
    }

    if (lua_islightuserdata(L, idx)) {
        return (mWidget*)lua_touserdata(L, idx);
    }
    if (lua_isuserdata(L, idx)) {
        mWidget **ptr = (mWidget**)lua_touserdata(L, idx);
        return ptr ? *ptr : NULL;
    }
    return NULL;
}


static void toValue(int& val, lua_State* L, int idx) {
    val = lua_tointeger(L, idx);
}

static char tohex(unsigned char c) {
    return c >= 10 ? 'A' + (c-10) : '0' + c;
}

static void toValue(unsigned long& val, lua_State* L, int idx) {
    double d = lua_tonumber(L, idx);
    val = (unsigned long)d;
}
static void toValue(unsigned int& val, lua_State* L, int idx) {
    unsigned long lv;
    toValue(lv, L, idx);
    val = (unsigned int)lv;
}

static void toValue(long& val, lua_State* L, int idx) {
    val = (long)lua_tointeger(L, idx);
}
static void toValue(float& val, lua_State* L, int idx) {
    val = (float)lua_tonumber(L, idx);
}
static void toValue(double& val, lua_State* L, int idx) {
    val = (double)lua_tonumber(L, idx);
}
static void toValue(string& val, lua_State* L, int idx) {
    const char *s = lua_tostring(L, idx);
    if (s) {
        val = s;
    } else {
        val = "";
    }
}
static void toValue(bool &val, lua_State* L, int idx) {
    val = lua_toboolean(L, idx) != 0;
}

static int pushvalue(lua_State* L, int val) {
    lua_pushinteger(L, val);
    return 1;
}

static int pushvalue(lua_State* L, unsigned int val) {
    lua_pushinteger(L, val);
    return 1;
}

static int pushvalue(lua_State* L, long val) {
    lua_pushinteger(L, val);
    return 1;
}

static int pushvalue(lua_State* L, unsigned long val) {
    lua_pushinteger(L, val);
    return 1;
}

static int pushvalue(lua_State* L, double val) {
    lua_pushnumber(L, val);
    return 1;
}

static int pushvalue(lua_State* L, float val) {
    lua_pushnumber(L, val);
    return 1;
}
static int pushvalue(lua_State* L, bool b) {
    lua_pushboolean(L, b ? 1 : 0);
    return 1;
}

static int pushvalue(lua_State* L, const string& str) {
    lua_pushstring(L, str.c_str());
    return 1;
}

static int pushvalue(lua_State* L, const char* str) {
    lua_pushstring(L, str);
    return 1;
}


static int traceback (lua_State *L) {
  if (!lua_isstring(L, 1))  /* 'message' not a string? */
    return 1;  /* keep it intact */
  lua_getfield(L, LUA_GLOBALSINDEX, "debug");
  if (!lua_istable(L, -1)) {
    lua_pop(L, 1);
    return 1;
  }
  lua_getfield(L, -1, "traceback");
  if (!lua_isfunction(L, -1)) {
    lua_pop(L, 2);
    return 1;
  }
  lua_pushvalue(L, 1);  /* pass error message */
  lua_pushinteger(L, 2);  /* skip this function and traceback */
  lua_call(L, 2, 1);  /* call debug.traceback */
  ALOGE("LuaGlue", "Error:%s", lua_tostring(L, -1));
  return 1;
}

static int docall(lua_State* L, int argc, int nret) {
    int base = lua_gettop(L) - argc;
    lua_pushcfunction(L, traceback);
    lua_insert(L, base);
    int ret = lua_pcall(L, argc, nret, base);
    lua_remove(L, base);
    return ret;
}



static DWORD toPropValue(PropType *prop_type, lua_State* L, int idx) {
    switch(prop_type->getType()) {
    case PropType::INT:
    case PropType::GROUP:
        return (DWORD) lua_tointeger(L, idx);
    case PropType::STRING:
    case PropType::TEXT:
    case PropType::IMAGE:
    case PropType::FONT:
        return prop_type->from(toHolderString(L, idx));
    case PropType::ENUM:
        return prop_type->from(lua_tostring(L, idx));
    case PropType::COLOR: {
            DWORD dwVal;
            toValue(dwVal, L, idx);
            return dwVal;
        }
    }

    return 0;
}

template<typename R>
struct LuaReturn {
    enum { RET_NUM = 1 };
    static R From(lua_State* L, int idx) {
        R r;
        toValue(r, L, idx);
        return r;
    }
};
template<> struct LuaReturn<void> {
    enum { RET_NUM = 0 };
    static void From(lua_State* L, int idx) { }
};

#include "lua_widgets.cpp"
class EventHandlerManager : public NCS_CREATE_NOTIFY_INFO {
    typedef EventHandlerManager self_t;

    lua_State* L;
    int  handlerTableRef;

    static void on_created(self_t* self, mWidget* widget, DWORD special_id) {
        self->onCreate(widget);
    }

    void onCreate(mWidget* widget) {
        SetWidgetEventHandlers(widget, (void*)this);
    }

    static BOOL do_onCreate(mWidget* self, DWORD dwAddData) {
        self_t * m = from(self);
        if (m) {
            m->call<void>(MSG_CREATE, (unsigned long)self);
        }
        return TRUE;
    }

#define DO_NO_VOID(name, message) \
    static void do_##name(mWidget* self) { \
        self_t * m = from(self); \
        if (m) { \
            m->call<void>(message, (unsigned long)self); \
        } \
    }

    DO_NO_VOID(onClose, MSG_CLOSE)

    static void do_message(mWidget* self, int message) {
        self_t * m = from(self);
        if (m) {
            m->call<void>(message, (unsigned long)self);
        }
    }

    static BOOL do_onKey(mWidget* self, int message, int scancode, DWORD key_status) {
        self_t * m = from(self);
        if (m) {
            m->call<void>(message, (unsigned long)self, scancode, key_status);
        }
        return NCSR_CONTINUE_MSG;
    }

    static BOOL do_onMouse(mWidget* self, int message, int x, int y, DWORD key_status) {
        self_t * m = from(self);
        if (m) {
            m->call<void>(message, (unsigned long)self, x, y, key_status);
        }
        return NCSR_CONTINUE_MSG;
    }

    static void do_onNotify(mWidget* self, int id, int nc, DWORD addData) {
        self_t * m = from(self);
        if (m) {
            m->call<void>(NCS_NOTIFY_CODE(nc), (unsigned long)self, id, addData);
        }
    }

    static void* getNativeHandler(int id) {
        switch(id) {
        case MSG_CREATE: return (void*) do_onCreate;
        case MSG_FONTCHANGED:
        case MSG_DESTROY:
        case MSG_SETFOCUS:
        case MSG_KILLFOCUS:
            return (void*) do_message;
        case MSG_CLOSE:
            return (void*) do_onClose;
        case MSG_KEYDOWN:
        case MSG_KEYUP:
        case MSG_CHAR:
        case MSG_SYSKEYDOWN:
        case MSG_SYSKEYUP:
        case MSG_SYSCHAR:
        case MSG_KEYLONGPRESS:
        case MSG_KEYALWAYSPRESS:
            return (void*)do_onKey;
        case MSG_LBUTTONDOWN:
        case MSG_LBUTTONUP:
        case MSG_MOUSEMOVE:
        case MSG_RBUTTONDOWN:
        case MSG_RBUTTONUP:
        case MSG_RBUTTONDBLCLK:
            return (void*)do_onMouse;
        default:
            if ((id & 0xFFFF0000) == 0xFFFF0000) {
                return (void*)do_onNotify;
            }
        }
        return NULL;
    }

    int callFunc(int code, int argc, int nret) {
        int base = lua_gettop(L) - argc;
        getHandler(code);
        if (lua_isnil(L, -1)) {
            lua_pop(L, argc);
            return 0;
        }

        lua_insert(L, base + 1);
        docall(L, argc, nret);
        return nret;
    }

    template<typename R>
    R call(int code) {
        pushvalue(L, code);
        callFunc(code, 1, LuaReturn<R>::RET_NUM);
        return LuaReturn<R>::From(L, -1);
    }
    template<typename R, typename A1>
    R call(int code, A1 a1) {
        pushvalue(L, code);
        pushvalue(L, a1);
        callFunc(code, 2, LuaReturn<R>::RET_NUM);
        return LuaReturn<R>::From(L, -1);
    }
    template<typename R, typename A1, typename A2>
    R call(int code, A1 a1, A2 a2) {
        pushvalue(L, code);
        pushvalue(L, a1);
        pushvalue(L, a2);
        callFunc(code, 3, LuaReturn<R>::RET_NUM);
        return LuaReturn<R>::From(L, -1);
    }
    template<typename R, typename A1, typename A2, typename A3>
    R call(int code, A1 a1, A2 a2, A3 a3) {
        pushvalue(L, code);
        pushvalue(L, a1);
        pushvalue(L, a2);
        pushvalue(L, a3);
        callFunc(code, 4, LuaReturn<R>::RET_NUM);
        return LuaReturn<R>::From(L, -1);
    }
    template<typename R, typename A1, typename A2, typename A3, typename A4>
    R call(int code, A1 a1, A2 a2, A3 a3, A4 a4) {
        pushvalue(L, code);
        pushvalue(L, a1);
        pushvalue(L, a2);
        pushvalue(L, a3);
        pushvalue(L, a4);
        callFunc(code, 5, LuaReturn<R>::RET_NUM);
        return LuaReturn<R>::From(L, -1);
    }

public:
    int getHandler(int code) {
        if (handlerTableRef <= 0) return 0;

        lua_rawgeti(L, LUA_REGISTRYINDEX, handlerTableRef);
        lua_pushinteger(L, code);
        lua_rawget(L, -2);
        lua_remove(L, -2);
        return 1;
    }

    EventHandlerManager(lua_State* L)
    :L(L), handlerTableRef(0) {
        onCreated = (void(*)(NCS_CREATE_NOTIFY_INFO*,mComponent*,DWORD))on_created;
    }

    ~EventHandlerManager() {
        if (handlerTableRef > 0) {
            luaL_unref(L, LUA_REGISTRYINDEX, handlerTableRef);
        }
    }

    static inline self_t* from(mWidget* w) {
        return (self_t*) GetWidgetEventHandlers(w);
    }

    void addEventHandler(int code, int idx) {
        int top = lua_gettop(L);
        if (handlerTableRef <= 0) {
            lua_newtable(L);
            lua_pushvalue(L, -1);
            handlerTableRef = luaL_ref(L, LUA_REGISTRYINDEX);
        } else {
            lua_rawgeti(L, LUA_REGISTRYINDEX, handlerTableRef);
        }

        lua_pushinteger(L, code);
        lua_pushvalue(L, idx);
        lua_rawset(L, -3);
        lua_pop(L, lua_gettop(L) - top);
    }

    NCS_EVENT_HANDLER* createHandlers() {
        if (handlerTableRef <= 0) {
            return NULL;
        }

        int top = lua_gettop(L);
        lua_rawgeti(L, LUA_REGISTRYINDEX, handlerTableRef);
        int tidx = lua_gettop(L);


        vector<NCS_EVENT_HANDLER> vhandlers;
        lua_pushnil(L);
        while (lua_next(L, tidx) != 0) {
            int code = lua_tointeger(L, -2);
            void *handle = getNativeHandler(code);
            if (handle) {
                NCS_EVENT_HANDLER h;
                h.message = code;
                h.handler = handle;
                vhandlers.push_back(h);
            }
            lua_pop(L, 1);
        }
        lua_pop(L, lua_gettop(L) - top);

        int i = 0;
        int count = vhandlers.size();
        if (count <= 0) {
            return NULL;
        }
        NCS_EVENT_HANDLER * handlers = new NCS_EVENT_HANDLER[count + 1];
        for (; i < count; i++) {
            handlers[i] = vhandlers[i];
        }

        handlers[i].message = 0;
        handlers[i].handler = NULL;
        return handlers;
    }

    void apply(NCS_WND_TEMPLATE* pwnd_tmpl) {
        pwnd_tmpl->handlers = createHandlers();
        pwnd_tmpl->notify_info = this;
    }

};



static int mg_log(lua_State* L) {
    int top = lua_gettop(L);
    lua_getglobal(L, "tostring");
    int index = 0;
    char szbuff[1024];
    for (int i = 1; i <= top; i++) {
        const char* s;
        lua_pushvalue(L, -1);
        lua_pushvalue(L, i);
        lua_call(L, 1, 1);
        s = lua_tostring(L, -1);
        if (s == NULL) {
            ALOGE("LuaGlue", "mg_log paramter %d is invalidate", i);
        } else {
            int len = strlen(s);
            if ((index + len) > sizeof(szbuff) + 1) {
                if (index == 0) {
                    ALOGE("LuaGlue LOG", "%s", s);
                } else {
                    szbuff[index] = '\0';
                    ALOGE("LuaGlue LOG", "%s", szbuff);
                }
                index = 0;
            } else {
                if (index > 0) {
                    szbuff[index++] = '\t';
                }
                strcpy(szbuff + index, s);
                index += len;
            }
        }
    }

    if (index > 0) {
        szbuff[index] = '\0';
        ALOGE("LuaGlue LOG", "%s", szbuff);
    }

    return 0;
}

static WidgetClassDefine* get_widget_class_define(NCS_WND_TEMPLATE* pwnd_tmpl, lua_State* L, int idx) {
    lua_pushstring(L, "Class");
    lua_gettable(L, idx);
    const char* class_name = toHolderString(L, -1);
    if (class_name) {
        pwnd_tmpl->class_name = class_name;
    }

    if (pwnd_tmpl->class_name) {
        return WidgetClassDefine::getClassDefine(pwnd_tmpl->class_name);
    }
    return NULL;
}


static void init_wnd_tmpl(NCS_WND_TEMPLATE* pwnd_tmpl, lua_State* L, int idx) {

    WndTemplateBuilder builder(pwnd_tmpl);

    WidgetClassDefine* widgetClassDefine = get_widget_class_define(pwnd_tmpl, L, idx);

    EventHandlerManager * pevent_handler_mgr = NULL;


    if (widgetClassDefine) {
        widgetClassDefine->initWndTemplateByDefaults(&builder);
    }

    lua_pushnil(L);
    while(lua_next(L, idx) != 0) {
        const char* str_name = lua_tostring(L, -2);
        int value_idx = lua_gettop(L);

        if (str_name == NULL) {
            goto NEXT;
        }

        if (strcmp(str_name, "children") == 0
                && lua_istable(L, -1)) {
            int count = lua_objlen(L, -1);
            if (count <= 0) {
                goto NEXT;
            }

            NCS_WND_TEMPLATE* ctrls = new NCS_WND_TEMPLATE[count];
            memset(ctrls, 0, sizeof(NCS_WND_TEMPLATE)*count);
            for (int i = 0; i < count; i++) {
                int pretop = lua_gettop(L);
                lua_rawgeti(L, value_idx, i+1);
                init_wnd_tmpl(&ctrls[i], L, lua_gettop(L));
                lua_pop(L, lua_gettop(L) - pretop);
            }
            pwnd_tmpl->ctrls = ctrls;
            pwnd_tmpl->count = count;

        } else if (strcmp(str_name, "id") == 0) {
            pwnd_tmpl->id = lua_tointeger(L, -1);
        } else if (strcmp(str_name, "Class") == 0) {
            goto NEXT;
        } else if(strncmp(str_name, "on", 2) == 0) {
            int message = widgetClassDefine->getEvent(str_name);
            if (pevent_handler_mgr == NULL) {
                pevent_handler_mgr = new EventHandlerManager(L);
            }

            pevent_handler_mgr->addEventHandler(message, value_idx);

        } else {
            Property * prop = Property::getProperty(str_name);
            if (prop == NULL) {
                ALOGE("LuaGlue", "Unspport property:%s", str_name);
                continue;
            }
            DWORD dwVal = toPropValue(prop->type, L, value_idx);
            builder.setProp(prop->id, dwVal);
        }
NEXT:
        lua_pop(L, 1);
    }

    if (pevent_handler_mgr) {
        pevent_handler_mgr->apply(pwnd_tmpl);
    }

    builder.build();


}


static void init_mainwnd_tmpl(NCS_MNWND_TEMPLATE* pmainwnd_tmpl, lua_State* L, int idx) {
    memset(pmainwnd_tmpl, 0, sizeof(NCS_MNWND_TEMPLATE));
    pmainwnd_tmpl->class_name = "MainWnd";

    init_wnd_tmpl((NCS_WND_TEMPLATE*)pmainwnd_tmpl, L, idx);
}


static int mg_createMainWindow(lua_State* L) {
    int n = lua_gettop(L);
    if (n <= 0 || !lua_istable(L, 1)) {
        ALOGE("LuaGlue", "CreateMainWindow paramter must be a table");
        return 0;
    }

    ThreadLocalHolder localHolder;
    NCS_MNWND_TEMPLATE wnd_tmpl;
    init_mainwnd_tmpl(&wnd_tmpl, L, 1);
    HWND host = HWND_DESKTOP;

    dumpWndTemplate(&wnd_tmpl);

    mWidget * widget = ncsCreateMainWindowIndirect(&wnd_tmpl, host);

    lua_pushlightuserdata(L, widget);
    return 1;
}

static int mg_wrap(lua_State* L) {
    int top = lua_gettop(L);
    if (top <= 0) {
        lua_pushnil(L);
        ALOGE("LuaGlue", "mg.wrap need widget pointer");
        return 1;
    }

    if (lua_isuserdata(L, 1)) {
        return 1;
    }

    mWidget* widget = NULL;
    if (lua_isnumber(L, 1)) {
        widget = (mWidget*)lua_tointeger(L, 1);
    } else if (lua_islightuserdata(L, 1)) {
        widget = (mWidget*)lua_touserdata(L, 1);
    }

    if (widget) {
        return wrap_widget_object(L, widget);
    }

    ALOGE("LuaGlue", "is not widget pointer:%s", lua_typename(L, lua_type(L, -1)));

    lua_pushnil(L);
    return 1;
}

static int mg_MessageBox(lua_State* L) {
    int top = lua_gettop(L);
    const char* caption = "caption";
    const char* text = "information";
    DWORD style = 0;
    if (top <= 0) {
        ALOGE("LuaGlue", "MessageBox need paramters");
        lua_pushnil(L);
        return 1;
    }

    mWidget* widget = getWrapWidget(L, 1);

    if (top >= 2) {
        text = lua_tostring(L, 2);
    }
    if (top >= 3) {
        caption = lua_tostring(L, 3);
    }
    if (top >= 4) {
        style = (DWORD)lua_tointeger(L, 4);
    }

    DWORD ret = MessageBox(widget ? widget->hwnd : HWND_DESKTOP,
            text, caption, style);
    lua_pushinteger(L, ret);
    return 1;
}

static int mg_DoModal(lua_State* L) {
    int top = lua_gettop(L);

    if (top <= 0 || !lua_istable(L, 1)) {
        lua_pushinteger(L, 0);
        return 1;
    }

    mWidget* host = NULL;
    if (top >= 2) {
        host = getWrapWidget(L, 2);
    }

    ThreadLocalHolder localHolder;
    NCS_MNWND_TEMPLATE wnd_tmpl;

    init_mainwnd_tmpl(&wnd_tmpl, L, 1);

    dumpWndTemplate(&wnd_tmpl);

    mMainWnd* main_wnd = (mMainWnd*)ncsCreateMainWindowIndirect(&wnd_tmpl, host ? host->hwnd : HWND_DESKTOP);

    if (main_wnd) {
        DWORD ret = _c(main_wnd)->doModal(main_wnd, TRUE);
        lua_pushinteger(L, ret);
        return 1;
    }

    lua_pushinteger(L, 0);
    return 1;
}

static mWidget * find_window(HWND hwnd, int id) {
    HWND hMainWnd = GetMainWindowHandle(hwnd);
    mWidget* main_wnd = ncsObjFromHandle(hMainWnd);
    if (main_wnd) {
        mWidget* widget = (mWidget*)(_c(main_wnd)->getChild(main_wnd, id));
        return widget;
    }
    return NULL;
}

static int mg_findWndObject(lua_State* L) {
    if (lua_gettop(L) < 2) {
        lua_pushnil(L);
        return 1;
    }

    mWidget* widget = getWrapWidget(L, 1);
    if (widget == NULL) {
        lua_pushnil(L);
        return 1;
    }

    int id = lua_tointeger(L, 2);

    mWidget* found = find_window(widget->hwnd, id);

    if (found) {
        return wrap_widget_object(L, found);
    }

    lua_pushnil(L);
    return 1;
}


struct luaL_Reg mg_methods[] = {
    {"log", mg_log },
    {"CreateMainWindow", mg_createMainWindow},
    {"wrap", mg_wrap},
    {"MessageBox", mg_MessageBox},
    {"DoModal", mg_DoModal},
    {"findWndObject", mg_findWndObject}
};

#define ADD_INT_CONSTEX(name, value) do { \
    lua_pushstring(L, name); \
    lua_pushinteger(L, value); \
    lua_settable(L, tbl); \
}while(0);

#define ADD_INT_CONST(name) ADD_INT_CONSTEX(#name, name)

static void init_constants(lua_State* L, int tbl) {
    ADD_INT_CONST(MB_OK);
    ADD_INT_CONST(MB_OKCANCEL);
    ADD_INT_CONST(MB_YESNO);
    ADD_INT_CONST(MB_RETRYCANCEL);
    ADD_INT_CONST(MB_ABORTRETRYIGNORE);
    ADD_INT_CONST(MB_YESNOCANCEL);
    ADD_INT_CONST(MB_ICONHAND);
    ADD_INT_CONST(MB_ICONQUESTION);
    ADD_INT_CONST(MB_ICONEXCLAMATION);
    ADD_INT_CONST(MB_ICONASTERISK);
    ADD_INT_CONST(MB_DEFBUTTON1);
    ADD_INT_CONST(MB_DEFBUTTON2);
    ADD_INT_CONST(MB_DEFBUTTON3);

    DEF_CONST(ADD_INT_CONSTEX)
}


static void initmg(lua_State* L) {
    lua_newtable(L);
    int mg_idx = lua_gettop(L);

    for (int i = 0;
            i < sizeof(mg_methods)/sizeof(mg_methods[0]);
            i ++) {
        if (mg_methods[i].name && mg_methods[i].func) {
            lua_pushstring(L, mg_methods[i].name);
            lua_pushcfunction(L, mg_methods[i].func);
            lua_settable(L, mg_idx);
        }
    }

    init_constants(L, mg_idx);

    //set the widget class table
    lua_newtable(L);
    lua_rawseti(L, mg_idx, 1);

    //set the global mg
    lua_pushvalue(L, mg_idx);
    lua_setglobal(L, "mg");
}



extern "C" unsigned long RunLuaScript(const char* script_source, const char* filename) {

    lua_State* L = lua_open();
    gL = L;
    luaL_openlibs(gL);

    //init mg
    initmg(gL);

    //do string
    lua_pushcfunction(L, traceback);
    int err_func = lua_gettop(L);
    luaL_loadstring(L, script_source);
    lua_pcall(L, 0, LUA_MULTRET, err_func);
    if(lua_gettop(L) > 1) {
        if (lua_islightuserdata(L, -1)
            || lua_isuserdata(L, -1)) {
            void *ptr = (void*)lua_topointer(L, -1);
            if (ptr == NULL) {
                return 0;
            }
            mWidget * widget = (mWidget*)ptr;
            return (unsigned long)widget->hwnd;
        } else if (lua_isstring(L, -1)) {
            //const char* err = lua_tostring(L, -1);
            //ALOGE("LuaGlue", "Error:%s", err);
        }
    }

    return 0;
}


}

