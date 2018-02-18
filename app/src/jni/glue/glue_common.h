#ifndef GLUE_COMMON_H
#define GLUE_COMMON_H

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
using std::string;
using std::map;

class WndTemplateBuilder;

namespace glue {

class PropType {
public:
    PropType() { }
    virtual ~PropType() { }

    virtual int getType() = 0;

    virtual DWORD from(const char* str) { return (DWORD)str; }
    virtual DWORD from(long val) { return (DWORD)val; }

    enum {
        INT,
        STRING,
        COLOR,
        TEXT,
        IMAGE,
        GROUP,
        FONT,
        RDR,
        ENUM,
    };
};

template<const int TYPE>
class TPropType : public PropType {
public:
    TPropType() { }

    virtual int getType() { return TYPE; }
};

class EnumType : public TPropType<PropType::ENUM> {
    map<string, int> valMaps;
    string name;
    EnumType() { }
public:

    static EnumType* create(const char* name, ...);

    virtual DWORD from(const char* str) {
        map<string, int>::iterator it = valMaps.find(str);
        if (it == valMaps.end()) {
            return (DWORD)(-1);
        }

        return it->second;
    }

    string getNameByValue(DWORD style) {
        DWORD dwMask = getMask();

        for(map<string, int>::iterator it = valMaps.begin(); it != valMaps.end(); ++it) {
            if (it->second == (dwMask & style)) {
                return it->first;
            }
        }
        return string();
    }

    DWORD getMask() {
        DWORD dwMask = 0;
        for(map<string, int>::iterator it = valMaps.begin(); it != valMaps.end(); ++it) {
            dwMask |= it->second;
        }
        return dwMask;
    }

};

template<const int Type>
class IntTypeBase : public TPropType<Type> {
public:
    IntTypeBase() { }

    virtual DWORD from(const char* str) {
        return (DWORD)strtol(str, NULL, 0);
    }
};


typedef IntTypeBase<PropType::INT> IntType;
typedef IntTypeBase<PropType::COLOR> ColorType;
typedef IntTypeBase<PropType::GROUP> GroupType;
typedef IntTypeBase<PropType::RDR> RdrType;

typedef TPropType<PropType::STRING> StringType;
typedef TPropType<PropType::TEXT> TextType;
typedef TPropType<PropType::IMAGE> ImageType;
typedef TPropType<PropType::FONT> FontType;

class Property;

struct PropValue {
    union {
        char *strVal;
        long iVal;
    }v;

    Property * prop;

    PropValue() {
        prop = NULL;
        v.iVal = 0;
    }
    PropValue(Property * p) {
        prop = p;
        v.iVal = 0;
    }

    PropValue(Property* p, long val) {
        setValue(p, val);
    }
    void setValue(Property* p, long val) {
        prop = p;
        v.iVal = val;
    }

    PropValue(Property* p, const char* strval) {
        setValue(p, strval);
    }

    void setValue(Property* p, const char* strval) {
        prop = p;
        v.strVal = strval ? strdup(strval) : NULL;
    }

    ~PropValue() {
        int vt = getValueType();
        if (vt == STR_VAL && v.strVal != NULL) {
            free(v.strVal);
        }
    }

    DWORD to() {
        PropType* type = getPropType();
        if (type == NULL) {
            return 0;
        }

        int vt = getValueType();
        if (vt == STR_VAL) {
            return type->from(v.strVal);
        }
        return type->from(v.iVal);
    }

private:
    enum {
        INT_VAL,
        STR_VAL
    };

    int getValueType() {
        PropType *type = getPropType();
        if (type == NULL) return INT_VAL;

        switch(type->getType()) {
        case PropType::STRING:
        case PropType::TEXT:
        case PropType::IMAGE:
        case PropType::FONT:
        case PropType::ENUM:
            return STR_VAL;
        }
        return INT_VAL;
    }

    PropType* getPropType();
};


struct Property {
    enum {
        READ = 1,
        WRITE = 2,
        RDWT = READ|WRITE
    };
    string name;
    PropType *type;
    int id;
    int access;
    PropValue* pDefValue;

    bool readable() { (access & READ) == READ; }
    bool writeable() { (access & WRITE) == WRITE; }

    Property(const char* name, PropType *type, int id, int access, PropValue* pdefVal = NULL) {
        this->name = name;
        this->type = type;
        this->id = id;
        this->access = access;
        pDefValue = pdefVal;
        if (pDefValue) {
            pDefValue->prop = this;
        }
    }

    template<typename T>
    void setDefValue(const T& val) {
        if (pDefValue == NULL) {
            pDefValue = new PropValue();
        }
        pDefValue->setValue(this, val);
    }

    bool hasDefValue() const { return pDefValue != NULL; }
    DWORD getDefValue() { return hasDefValue() ? pDefValue->to() : 0; }

    static Property* getProperty(const char* name);
};


class WidgetClassDefine {

    WidgetClassDefine * parent;
    mWidgetClass * ownerClass;
    string className;
    map<string, Property*> properties;
    map<string, PropValue*> defValues;
    map<string, int> eventMap;

    void * glueObject;

public:
    WidgetClassDefine(const char* name, mWidgetClass* ownerClass) {
        className = name;
        this->ownerClass = ownerClass;
        glueObject = NULL;
        parent = getClassDefine((mWidgetClass*)(ownerClass->super));
        addClassDefine(this);
    }
    ~WidgetClassDefine() {
        //TODO
    }

    mWidgetClass * getOwnerClass() { return ownerClass; }

    const char* getClassName() {
        return className.c_str();
    }
    WidgetClassDefine * getParent() {
        return parent;
    }

    map<string, Property*>& getProperties() {
        return properties;
    }

    void addProperty(Property* prop) {
        properties[prop->name] = prop;
    }

    void addEvent(const char* name, int id) {
        eventMap[name] = id;
    }

    int getEvent(const char* name) {
        map<string, int>::iterator it = eventMap.find(name);
        if (it == eventMap.end()) {
            return getParent() ? getParent()->getEvent(name) : 0;
        }
        return it->second;
    }

    Property* getProperty(const char* name) {
        map<string, Property*>::iterator it = properties.find(name);
        if ( it == properties.end()) {
            if (getParent()) {
                return getParent()->getProperty(name);
            }
            return NULL;
        }
        return it->second;
    }

    template<typename T>
    void setDefPropValue(const char* name, const T& val) {
        Property * prop = getProperty(name);
        if (prop == NULL) {
            //TODO Error
            return ;
        }
        defValues[name] = new PropValue(prop, val);
    }

    void setDefPropValue(const char* name, PropValue * pdefVal) {
        Property * prop = getProperty(name);
        if (prop == NULL) {
            //TODO Error
            return ;
        }

        pdefVal->prop = prop;
        defValues[name] = pdefVal;
    }

    template<typename T>
    T* getGlueObject() {
        return (T*)glueObject;
    }

    template<typename T>
    void setGlueObject(T * obj) {
        glueObject = (void*)obj;
    }

    void initWndTemplateByDefaults(WndTemplateBuilder* pbuilder);

    static WidgetClassDefine * getClassDefine(mWidgetClass* ownerClass);
    static WidgetClassDefine * getClassDefine(const char* name);

private:
    static void addClassDefine(WidgetClassDefine* define);

    static map<mWidgetClass*, WidgetClassDefine*>* sWidgetMaps;
    static map<string, WidgetClassDefine*>* sNamedWidgetMaps;
};

enum {
    WIDGET_KEY = 0x7FAC2300,
    WIDGET_KEY_MAX
};


int GetEventIdByName(const char* name);

bool InitGlue();

void* GetWidgetEventHandlers(mWidget *widget);
void SetWidgetEventHandlers(mWidget* widget, void *pt);

unsigned long RunScript(const char* script, const char* filename);


void dumpWndTemplate(const NCS_WND_TEMPLATE* tmpl, char* prefix);

static inline void dumpWndTemplate(const NCS_WND_TEMPLATE* tmpl) {
    char szPrefix[64] = "\0";
    dumpWndTemplate(tmpl, szPrefix);
}
static inline void dumpWndTemplate(const NCS_MNWND_TEMPLATE* tmp) {
    dumpWndTemplate((const NCS_WND_TEMPLATE*)tmp);
}

template<class TScriptObject>
class TEventHandlerManager : public NCS_CREATE_NOTIFY_INFO {
    typedef TScriptObject script_t;
    typedef typename script_t::persistent_handler_t persistent_handler_t;
    typedef typename script_t::local_handler_t local_handler_t;
    typedef TEventHandlerManager<script_t> self_t;

    typedef map<int, persistent_handler_t> FunctionMap;
    script_t    script;
    FunctionMap functionMap;

    static void on_created(self_t* self, mWidget* widget, DWORD special_id) {
        self->onCreate(widget);
    }

    void onCreate(mWidget* widget) {
        SetWidgetEventHandlers(widget, (void*)this);
        script.setWidgetObject(widget);
    }

    static BOOL do_onCreate(mWidget* self, DWORD dwAddData) {
        self_t * m = from(self);
        if (m) {
            m->script.template call<void>(MSG_CREATE, (unsigned long)self);
        }
        return TRUE;
    }

#define DO_NO_VOID(name, message) \
    static void do_##name(mWidget* self) { \
        self_t * m = from(self); \
        if (m) { \
            m->script.template call<void>(message, (unsigned long)self); \
        } \
    }

    DO_NO_VOID(onClose, MSG_CLOSE)

    static void do_message(mWidget* self, int message) {
        self_t * m = from(self);
        if (m) {
            m->script.template call<void>(message, (unsigned long)self);
        }
    }

    static BOOL do_onKey(mWidget* self, int message, int scancode, DWORD key_status) {
        self_t * m = from(self);
        if (m) {
            m->script.template call<void>(message, (unsigned long)self, scancode, key_status);
        }
        return NCSR_CONTINUE_MSG;
    }

    static BOOL do_onMouse(mWidget* self, int message, int x, int y, DWORD key_status) {
        self_t * m = from(self);
        if (m) {
            m->script.template call<void>(message, (unsigned long)self, x, y, key_status);
        }
        return NCSR_CONTINUE_MSG;
    }

    static void do_onNotify(mWidget* self, int id, int nc, DWORD addData) {
        self_t * m = from(self);
        if (m) {
            m->script.template call<void>(NCS_NOTIFY_CODE(nc), (unsigned long)self, id, addData);
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

public:
    local_handler_t getHandler(int code) {
        typename FunctionMap::iterator it = functionMap.find(code);
        if (it == functionMap.end())
            return script.toNullLocal();
        local_handler_t lret;
        script.assign(lret, it->second);
        return lret;
    }

    TEventHandlerManager()
    :script(this) {
        onCreated = (void(*)(NCS_CREATE_NOTIFY_INFO*,mComponent*,DWORD))on_created;
    }

    template<class A>
    TEventHandlerManager(A a)
    :script(this, a) {
        onCreated = (void(*)(NCS_CREATE_NOTIFY_INFO*,mComponent*,DWORD))on_created;
    }

    ~TEventHandlerManager() {
        for (typename FunctionMap::iterator it = functionMap.begin();
                it != functionMap.end(); ++it) {
            script.release(it->second);
        }
    }

    static inline self_t* from(mWidget* w) {
        return (self_t*) GetWidgetEventHandlers(w);
    }

    void addEventHandler(int code, local_handler_t func) {
        if (!script.isFunc(func)) {
            return ;
        }

        script.addRef(func);

        script.assign(functionMap[code], func);
    }

    NCS_EVENT_HANDLER* createHandlers() {
        int count = functionMap.size();
        if (count <= 0) {
            return NULL;
        }

        NCS_EVENT_HANDLER * handlers = new NCS_EVENT_HANDLER[count + 1];
        int i = 0;
        for (typename FunctionMap::iterator it = functionMap.begin();
                it != functionMap.end(); ++it) {
            void *handle = getNativeHandler(it->first);
            if (handle) {
                handlers[i].message = it->first;
                handlers[i].handler = handle;
                i ++;
            }
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


}


#endif

