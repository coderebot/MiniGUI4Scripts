#include <stdio.h>
#include "v8.h"
#include "libplatform/libplatform.h"
#include <map>
#include "glue_common.h"
#include "common.h"
#include "minigui.h"
#include "gdi.h"
#include "window.h"
#include "mgncs.h"
#include "mg-base.h"
#include "glue_utils.h"

using namespace v8;
using namespace glue;
using std::map;

namespace v8_glue {

static Isolate* sIsolate;
static Persistent<Context> sContext;

template <typename R>
struct TypeValue { };

static Local<String> toV8String(Isolate* isolate, const char* str) {
    if (str == NULL) {
        return Local<String>();
    }
    return v8::String::NewFromUtf8(isolate, str, NewStringType::kNormal).ToLocalChecked();
}

static string toString(Isolate* isolate, Local<Value> str) {
    if (str.IsEmpty()) {
        return string();
    }

    String::Utf8Value utf8str(str);
    return string(*utf8str);
}

static Local<String> toV8String(Isolate* isolate, const string& str) {
    return toV8String(isolate, str.c_str());
}

template<>
struct TypeValue<void> {
    static void DefaultValue() { }
    static void From(Isolate* isolate, const Local<Value>& value) { }
};

#define DEF_Prime_TypeValue(Type, DefVal, V8Type, ToV8Type, NewType) \
template<> \
struct TypeValue<Type> { \
    static Type DefaultValue() { return DefVal; } \
    static Type From(Isolate* isolate, const Local<Value> & value) { \
        Local<V8Type> val = value->ToV8Type(isolate->GetCurrentContext()).ToLocalChecked(); \
        if (val.IsEmpty()) { \
            return DefaultValue(); \
        } \
        return val->Value(); \
    } \
    static Local<Value> To(Isolate* isolate, Type val) { \
        return NewType(isolate, val); \
    } \
};

static inline Local<Integer> IntegerNew(Isolate* isolate, int64_t val) {
    return Integer::New(isolate, (int32_t)val);
}

DEF_Prime_TypeValue(int, 0, Int32, ToInt32, Integer::New)
DEF_Prime_TypeValue(uint32_t, 0, v8::Uint32, ToUint32, Integer::NewFromUnsigned)
DEF_Prime_TypeValue(int64_t, 0, Integer, ToInteger, IntegerNew)
DEF_Prime_TypeValue(unsigned long, 0, Integer, ToInteger, IntegerNew)
DEF_Prime_TypeValue(double, 0.0, Number, ToNumber, Number::New)
DEF_Prime_TypeValue(bool, false, Boolean, ToBoolean, Boolean::New)

template<>
struct TypeValue<const char*> {
    static const char* DefaultValue() { return NULL; }
    static const char* From(Isolate *isolate, const Local<Value>& value) {
        Local<String> val = value->ToString(isolate->GetCurrentContext()).ToLocalChecked();
        if (val.IsEmpty()) {
            return DefaultValue();
        }
        String::Utf8Value strval (val);
        const char* p = *strval;
        if (p) {
            char* pret = strdup(p);
            ThreadLocalHolder::push(pret);
            return pret;
        }
        return NULL;
    }
    static Local<Value> To(Isolate* isolate, const char* str) {
        return toV8String(isolate, str);
    }
};

template<>
struct TypeValue<string> {
    static string DefaultValue() { return string(""); }
    static string From(Isolate* isolate, const Local<Value>& value) {
        Local<String> val = value->ToString(isolate->GetCurrentContext()).ToLocalChecked();
        if (val.IsEmpty()) {
            return DefaultValue();
        }
        String::Utf8Value strval(val);
        return string(*strval);
    }
    static Local<Value> To(Isolate* isolate, const string str) {
        return toV8String(isolate, str);
    }
};

template<typename T>
static inline Local<Value> toV8Value(Isolate* isolate, T t) {
    return TypeValue<T>::To(isolate, t);
}

static DWORD toPropValue(Isolate *isolate, PropType *prop_type, Local<Value> value) {
    switch(prop_type->getType()) {
    case PropType::INT:
    case PropType::GROUP:
        return prop_type->from(TypeValue<int>::From(isolate, value));
    case PropType::STRING:
    case PropType::TEXT:
    case PropType::IMAGE:
    case PropType::FONT:
    case PropType::ENUM:
        return prop_type->from(TypeValue<const char*>::From(isolate, value));
    case PropType::COLOR:
        return (DWORD)TypeValue<unsigned int>::From(isolate, value);
    }

    return 0;
}

static Local<Value> fromPropValue(Isolate* isolate, PropType *prop_type, unsigned long dwVal) {
    switch(prop_type->getType()) {
    case PropType::INT:
    case PropType::GROUP:
        return TypeValue<int>::To(isolate, (int)dwVal);
    case PropType::STRING:
    case PropType::TEXT:
    case PropType::IMAGE:
    case PropType::FONT:
        return TypeValue<const char*>::To(isolate, (const char*)dwVal);
    case PropType::ENUM: {
        string enum_value = ((EnumType*)prop_type)->getNameByValue((DWORD)dwVal);
        return TypeValue<const char*>::To(isolate, enum_value.c_str());
    }
    case PropType::COLOR:
        return TypeValue<unsigned int>::To(isolate, (DWORD)dwVal);
    }
    return Local<Value>();
}

static mWidget* getWrapWidget(Local<Object> obj) {
    Local<External> field = Local<External>::Cast(obj->GetInternalField(0));
    void *ptr = field->Value();
    return static_cast<mWidget*>(ptr);
}

static mWidget* getWidget(Isolate* isolate, Local<Value> value) {
    if (value->IsNumber()) {
        return (mWidget*)(TypeValue<unsigned long>::From(isolate, value));
    } else if(value->IsObject()) {
        return getWrapWidget(Local<Object>::Cast(value));
    }

    return NULL;
}

#include "widget_method.cpp"

static void dumpWndTemplate(const NCS_WND_TEMPLATE* tmpl, char* prefix) {
    int len = strlen(prefix);
#define _D_TMP(format, member) ALOGI("MiniGUIV8", "\t%s"#member "= " format, prefix, tmpl->member)
    ALOGI("MiniGUIV8", "%s{", prefix);
    _D_TMP("%s", class_name);
    _D_TMP("%d", id);
    _D_TMP("%d", x);
    _D_TMP("%d", y);
    _D_TMP("%d", w);
    _D_TMP("%d", h);
    _D_TMP("0x%08X", style);
    _D_TMP("0x%08X", ex_style);
    _D_TMP("%s", caption);
    if (tmpl->props) {
        for (int i = 0; tmpl->props[i].id > 0; i++) {
            ALOGI("MiniGUIV8", "\t%sProp %d=0x%08x", prefix, tmpl->props[i].id, tmpl->props[i].value);
        }
    }
    if (tmpl->rdr_info) {
        ALOGI("MiniGUIV8", "\t%sglobal render=%s", tmpl->rdr_info->glb_rdr);
        ALOGI("MiniGUIV8", "\t%scontrol render=%s", tmpl->rdr_info->ctl_rdr);
        if (tmpl->rdr_info->elements) {
            for(int i = 0; tmpl->rdr_info->elements[i].id > 0; i++) {
                ALOGI("MiniGUIV8", "\t%sRdr %d=0x%08x", tmpl->rdr_info->elements[i].id, tmpl->rdr_info->elements[i].value);
            }
        }
    }

    if(tmpl->handlers) {
        for (int i = 0; tmpl->handlers[i].handler != NULL; i++) {
            ALOGI("MiniGUIV8", "\t%sHandler message 0x%08x: handler:%p", prefix, tmpl->handlers[i].message, tmpl->handlers[i].handler);
        }
    }

    _D_TMP("0x%08x", bk_color);
    _D_TMP("%s", font_name);

    if (tmpl->ctrls) {
        prefix[len] = '\t';
        prefix[len+1] = '\0';
        for (int i = 0; i < tmpl->count; i++) {
            dumpWndTemplate(&tmpl->ctrls[i], prefix);
        }
        prefix[len] = '\0';
    }

    ALOGI("MiniGUIV8", "%s}", prefix);
#undef _D_TMP
}


static void dumpWndTemplate(const NCS_WND_TEMPLATE* tmpl) {
    char szPrefix[64] = "\0";
    dumpWndTemplate(tmpl, szPrefix);
}
static void dumpWndTemplate(const NCS_MNWND_TEMPLATE* tmp) {
    dumpWndTemplate((const NCS_WND_TEMPLATE*)tmp);
}


static void dumpV8Exception(v8::Isolate* isolate, v8::TryCatch* tryCatch);
static Local<Object> wrap_widget_object(Isolate* isolate, mWidget* widget);

class EventHandlerManager : public NCS_CREATE_NOTIFY_INFO {
    typedef map<int, Persistent<Function> >  FunctionMap;

    FunctionMap functionMap;
    Persistent<Object> widgetObject;
    Isolate * isolate;

    Local<Function> getHandler(int code) {
        FunctionMap::iterator it = functionMap.find(code);
        if (it == functionMap.end()) {
            return Local<Function>();
        }
        return Local<Function>::New(isolate, it->second);
    }


    Local<Value>  callFunc(int code, int argc, Local<Value> argv[]) {

        Local<Context> context = Local<Context>::New(isolate, sContext);
        Context::Scope context_scope(context);

        Local<Function> func = getHandler(code);
        if (func.IsEmpty()) {
            return Local<Value>();
        }

        Local<Object> reciever = Local<Object>::New(isolate, widgetObject);

        TryCatch tryCatch;
        MaybeLocal<Value> result = func->Call(context, reciever, argc, argv);

        if (tryCatch.HasCaught()) {
            dumpV8Exception(isolate, &tryCatch);
            return Local<Value>();
        }
        return result.ToLocalChecked();
    }

    static void on_created(EventHandlerManager * self, mWidget* widget, DWORD special_id) {
        self->onCreate(widget);
    }

    void onCreate(mWidget* widget) {
        widgetObject.Reset(isolate, wrap_widget_object(isolate, widget));
        SetWidgetEventHandlers(widget, (void*)this);
    }


    static BOOL do_onCreate(mWidget *self, DWORD dwAddData) {
        EventHandlerManager * m = get_event_handler_manager(self);
        if (m) {
            m->call<void>(MSG_CREATE, (unsigned long)self);
        }
        return TRUE;
    }

#define DO_NO_VOID(name, message) \
    static void do_##name(mWidget* self) { \
        EventHandlerManager * m = get_event_handler_manager(self); \
        if (m) { \
            m->call<void>(message, (unsigned long)self); \
        } \
    }

    DO_NO_VOID(onClose, MSG_CLOSE)

    static void do_message(mWidget* self, int message) {
        EventHandlerManager * m = get_event_handler_manager(self);
        if (m) {
            m->call<void>(message, (unsigned long)self);
        }
    }

    static BOOL do_onKey(mWidget* self, int message, int scancode, DWORD key_status) {
        EventHandlerManager * m = get_event_handler_manager(self);
        if (m) {
            m->call<void>(message, (unsigned long)self, scancode, key_status);
        }
        return NCSR_CONTINUE_MSG;
    }

    static BOOL do_onMouse(mWidget* self, int message, int x, int y, DWORD key_status) {
        EventHandlerManager * m = get_event_handler_manager(self);
        if (m) {
            m->call<void>(message, (unsigned long)self, x, y, key_status);
        }
        return NCSR_CONTINUE_MSG;
    }

    static void do_onNotify(mWidget* self, int id, int nc, DWORD addData) {
        EventHandlerManager * m = get_event_handler_manager(self);
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

public:

    EventHandlerManager(Isolate *isolate) {
        this->isolate = isolate;
        this->onCreated = (void(*)(NCS_CREATE_NOTIFY_INFO*,mComponent*,DWORD))on_created;
    }


    static inline EventHandlerManager * get_event_handler_manager(mWidget *w) {
        return (EventHandlerManager*) GetWidgetEventHandlers(w);
    }


    void addEventHanler(int code, Isolate* isolate, Local<Function> function) {
        functionMap[code].Reset(isolate, function);
    }

    NCS_EVENT_HANDLER * createHandlers() {
        int count = functionMap.size();
        if (count <= 0) {
            return NULL;
        }
        NCS_EVENT_HANDLER * handlers = new NCS_EVENT_HANDLER[count+1];
        int i = 0;
        for (FunctionMap::iterator it = functionMap.begin();
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

    void apply(NCS_WND_TEMPLATE *pwnd_tmpl) {
        pwnd_tmpl->handlers = createHandlers();
        pwnd_tmpl->notify_info = this;
    }


    template<typename R>
    R call(int code) {
        Isolate::Scope isolate_scope(isolate);
        HandleScope handle_scope(isolate);

        Local<Value> args[] = {
            toV8Value(isolate, code)
        };
        return (R)TypeValue<R>::From(isolate, callFunc(code, sizeof(args)/sizeof(args[0]), args));
    }

    template<typename R, typename A1>
    R call(int code, A1 a1) {
        Isolate::Scope isolate_scope(isolate);
        HandleScope handle_scope(isolate);

        Local<Value> args[] = {
            toV8Value(isolate, code),
            toV8Value(isolate, a1)
        };
        return (R)TypeValue<R>::From(isolate, callFunc(code, sizeof(args)/sizeof(args[0]), args));
    }

    template<typename R, typename A1, typename A2>
    R call(int code, A1 a1, A2 a2) {
        Isolate::Scope isolate_scope(isolate);
        HandleScope handle_scope(isolate);

        Local<Value> args[] = {
            toV8Value(isolate, code),
            toV8Value(isolate, a1),
            toV8Value(isolate, a2)
        };
        return (R)TypeValue<R>::From(isolate, callFunc(code, sizeof(args)/sizeof(args[0]), args));
    }

    template<typename R, typename A1, typename A2, typename A3>
    R call(int code, A1 a1, A2 a2, A3 a3) {
        Isolate::Scope isolate_scope(isolate);
        HandleScope handle_scope(isolate);

        Local<Value> args[] = {
            toV8Value(isolate, code),
            toV8Value(isolate, a1),
            toV8Value(isolate, a2),
            toV8Value(isolate, a3)
        };
        return (R)TypeValue<R>::From(isolate, callFunc(code, sizeof(args)/sizeof(args[0]), args));
    }

    template<typename R, typename A1, typename A2, typename A3, typename A4>
    R call(int code, A1 a1, A2 a2, A3 a3, A4 a4) {
        Isolate::Scope isolate_scope(isolate);
        HandleScope handle_scope(isolate);

        Local<Value> args[] = {
            toV8Value(isolate, code),
            toV8Value(isolate, a1),
            toV8Value(isolate, a2),
            toV8Value(isolate, a3),
            toV8Value(isolate, a4)
        };
        return (R)TypeValue<R>::From(isolate, callFunc(code, sizeof(args)/sizeof(args[0]), args));
    }

};



static Local<FunctionTemplate> createWidgetClass(Isolate* isolate, WidgetClassDefine *widget);

static Local<FunctionTemplate> getWidgetFunctionTemplate(Isolate* isolate, WidgetClassDefine *widget) {
    Persistent<FunctionTemplate>* f =
            widget->getGlueObject<Persistent<FunctionTemplate> >();

    if (f == NULL) {
        return Local<FunctionTemplate>();
    }
    return Local<FunctionTemplate>::New(isolate, *f);
}

static void addWidgetFunctionTemplate(Isolate* isolate, WidgetClassDefine* widget, Local<FunctionTemplate> func_tmpl) {

    Persistent<FunctionTemplate>* f =
            widget->getGlueObject<Persistent<FunctionTemplate> >();
    if (f == NULL) {
        f = new Persistent<FunctionTemplate>();
        widget->setGlueObject(f);
    }
    f->Reset(isolate, func_tmpl);
}

static Property* getWrapProperty(Local<Value> obj) {
    Local<External> prop = Local<External>::Cast(obj);
    return static_cast<Property*>(prop->Value());
}

static void widget_prop_get(Local<String> property, const PropertyCallbackInfo<Value>& info) {
    mWidget *widget = getWrapWidget(info.This());
    Property* prop = getWrapProperty(info.Data());

    //DWORD dwVal = _c(widget)->getProperty(widget, prop->id);
    PValue val;
    if (getWidgetProperty(widget, prop, val)) {
        info.GetReturnValue().Set(fromPropValue(info.GetIsolate(), prop->type, val.d.uval));
    }
}


static void widget_prop_set(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
    mWidget *widget = getWrapWidget(info.This());
    Property* prop = getWrapProperty(info.Data());

    ThreadLocalHolder localHolder();

    DWORD dwVal = toPropValue(info.GetIsolate(), prop->type, value);

    setWidgetProperty(widget, prop, dwVal);

    info.GetReturnValue().Set(value);
}

static Local<FunctionTemplate> getAndInstallWidgetFunctionTemplate(Isolate* isolate, WidgetClassDefine *widget) {
    Persistent<FunctionTemplate>* f =
            widget->getGlueObject<Persistent<FunctionTemplate> >();
    if (f == NULL) {
        f = new Persistent<FunctionTemplate>();
        widget->setGlueObject(f);
        f->Reset(isolate, createWidgetClass(isolate, widget));
    }
    return getWidgetFunctionTemplate(isolate, widget);
}

static Local<FunctionTemplate> createWidgetClass(Isolate* isolate, WidgetClassDefine *widget) {
    HandleScope handle_scope(isolate);

    Local<FunctionTemplate> func_tmpl = FunctionTemplate::New(isolate);

    func_tmpl->SetClassName(toV8String(isolate, widget->getClassName()));

    Local<ObjectTemplate> prototype = /*func_tmpl->InstanceTemplate();//*/func_tmpl->PrototypeTemplate();

    init_widget_methods(isolate, widget->getOwnerClass(), prototype);

    if (widget->getParent()) {
        Local<FunctionTemplate> parent = getAndInstallWidgetFunctionTemplate(isolate, widget->getParent());
        func_tmpl->Inherit(parent);
    }

    Local<ObjectTemplate> instance_tmpl = func_tmpl->InstanceTemplate();
    map<string,Property*> &props = widget->getProperties();
    for (map<string,Property*>::iterator it = props.begin(); it != props.end(); ++it) {
        Property* prop = it->second;
        Local<External> prop_data = External::New(isolate, prop);
        ALOGE("MiniGUI V8", "set prop:%s.%s", widget->getClassName(), prop->name.c_str());
        instance_tmpl->SetAccessor(toV8String(isolate, prop->name),
                widget_prop_get,
                widget_prop_set,
                prop_data);
    }


    instance_tmpl->SetInternalFieldCount(1);

    return func_tmpl;
}

static Local<FunctionTemplate> getWidgetFunctionTemplate(Isolate* isolate, mWidget *widget) {
    WidgetClassDefine * widget_clzz_def = WidgetClassDefine::getClassDefine(_c(widget));
    return getAndInstallWidgetFunctionTemplate(isolate, widget_clzz_def);
}

static void dumpV8Exception(v8::Isolate* isolate, v8::TryCatch* tryCatch) {
    v8::String::Utf8Value exception(tryCatch->Exception());
    v8::Handle<v8::Message> message = tryCatch->Message();
    if (message.IsEmpty()) {
        ALOGE("MiniGUIV8", "%s", *exception);
        return ;
    } else {
        v8::String::Utf8Value filename(message->GetScriptResourceName());
        int lineNumber = message->GetLineNumber();
        int start = message->GetStartPosition();
        int end = message->GetEndPosition();
        const char* filenameString = *filename;

        v8::String::Utf8Value sourceline(
                message->GetSourceLine(isolate->GetCurrentContext()).ToLocalChecked());
        const char* sourceline_str = *sourceline;

        ALOGE("MiniGUIV8.ERR", "%s", sourceline_str);
        ALOGE("MiniGUIV8.ERR", "%s:%d(from %d to %d)", filenameString, lineNumber, start, end);

        v8::Local<v8::Value> stack_trace_string;
        if (tryCatch->StackTrace(isolate->GetCurrentContext()).ToLocal(&stack_trace_string)
                && stack_trace_string->IsString()) {
            v8::String::Utf8Value stack_trace(v8::Local<v8::String>::Cast(stack_trace_string));
            ALOGE("MiniGUIV8.ERR", "%s", *stack_trace);
        }
    }
}

static void init_wnd_tmpl(NCS_WND_TEMPLATE* pwnd_tmpl, Isolate *isolate, Local<Object> wnd_tmpl_obj) {

    WndTemplateBuilder builder(pwnd_tmpl);

    WidgetClassDefine* widgetClassDefine = NULL;

    EventHandlerManager * pevent_handler_mgr = NULL;

    Local<Value> v8ClassName = wnd_tmpl_obj->Get(isolate->GetCurrentContext(), toV8String(isolate, "Class")).ToLocalChecked();
    if (!v8ClassName.IsEmpty() && v8ClassName->IsString()) {
        pwnd_tmpl->class_name = TypeValue<const char*>::From(isolate, v8ClassName);
    }

    if (pwnd_tmpl->class_name) {
        widgetClassDefine = WidgetClassDefine::getClassDefine(pwnd_tmpl->class_name);
    }

    if (widgetClassDefine) {
        widgetClassDefine->initWndTemplateByDefaults(&builder);
    }

    Local<Array> prop_names = wnd_tmpl_obj->GetOwnPropertyNames(isolate->GetCurrentContext()).ToLocalChecked();
    for (int i = 0; i < prop_names->Length(); i++) {
        Local<Value> key = prop_names->Get(isolate->GetCurrentContext(), i).ToLocalChecked();
        string str_key = toString(isolate, key);
        if (str_key.empty()) {
            continue; //TODO error
        }

        Local<Value> value = wnd_tmpl_obj->Get(isolate->GetCurrentContext(), key).ToLocalChecked();

        if (str_key == "children"
                && value->IsArray()) {
            Local<Array> children = Local<Array>::Cast(value);
            int count = children->Length();
            NCS_WND_TEMPLATE * ctrls = new NCS_WND_TEMPLATE[count];
            memset(ctrls, 0, sizeof(NCS_WND_TEMPLATE)*count);
            for (int chl = 0; chl < count; chl ++) {
                Local<Value> ctrl_tmpl_obj = children->Get(isolate->GetCurrentContext(), chl).ToLocalChecked();
                init_wnd_tmpl(&ctrls[chl], isolate, Local<Object>::Cast(ctrl_tmpl_obj));
            }
            pwnd_tmpl->ctrls = ctrls;
            pwnd_tmpl->count = count;
        } else if (str_key == "id") {
            pwnd_tmpl->id = TypeValue<int>::From(isolate, value);
        } else if(strncmp(str_key.c_str(), "on", 2) == 0
                && value->IsFunction() && widgetClassDefine) {
            int message = widgetClassDefine->getEvent(str_key.c_str());
            if (pevent_handler_mgr == NULL) {
                pevent_handler_mgr = new EventHandlerManager(isolate);
            }
            pevent_handler_mgr->addEventHanler(message, isolate, Local<Function>::Cast(value));
        } else if (str_key == "Class") {
            continue; //class is replaced
        } else {
            Property * prop = Property::getProperty(str_key.c_str());
            if (prop == NULL) {
                ALOGE("MiniGUI V8", "Unsport property:%s", str_key.c_str());
                continue;
            }
            DWORD dwVal = toPropValue(isolate, prop->type, value);
            int id = prop->id;
            builder.setProp(prop->id, dwVal);
        }
    }

    builder.build();

    if (pevent_handler_mgr) {
        pevent_handler_mgr->apply((NCS_WND_TEMPLATE*)pwnd_tmpl);
    }
}

static void init_mainwnd_tmpl(NCS_MNWND_TEMPLATE *pmainwnd_tmpl, Isolate * isolate, Local<Object> tmpl) {
    memset(pmainwnd_tmpl, 0, sizeof(NCS_MNWND_TEMPLATE));
    pmainwnd_tmpl->class_name = "MainWnd";

    init_wnd_tmpl((NCS_WND_TEMPLATE*)pmainwnd_tmpl, isolate, tmpl);
}

static Local<Object> wrap_widget_object(Isolate* isolate, mWidget* widget) {
    EscapableHandleScope handle_scope(isolate);

    Local<FunctionTemplate> func_tmpl = getWidgetFunctionTemplate(isolate, widget);
    if (func_tmpl.IsEmpty()) {
        return Local<Object>();
    }

    Local<Function> func = func_tmpl->GetFunction(isolate->GetCurrentContext()).ToLocalChecked();
    if (func.IsEmpty()) {
        return Local<Object>();
    }

    Local<Object> thiz = func->NewInstance(isolate->GetCurrentContext(), 0, NULL).ToLocalChecked();

    if (thiz.IsEmpty()) {
        return Local<Object>();
    }

    Local<External> widget_ptr = External::New(isolate, widget);

    //set widget object
    thiz->SetInternalField(0, widget_ptr);

    return handle_scope.Escape(thiz);
}

static void mg_createMainWindow(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    if (args.Length() == 0) {
        isolate->ThrowException(
                toV8String(isolate, "Bad parameters of createMainWindow"));
        return ;
    }
    Local<Object> wnd_tmpl_obj = args[0]->ToObject();
    if (wnd_tmpl_obj.IsEmpty()) {
        isolate->ThrowException(
                toV8String(isolate, "Bad parameters of createMainWindow: need window template object"));
        return ;
    }

    ThreadLocalHolder localHolder;

    NCS_MNWND_TEMPLATE wnd_tmpl;

    HWND host = HWND_DESKTOP;

    init_mainwnd_tmpl(&wnd_tmpl, isolate, wnd_tmpl_obj);

    dumpWndTemplate(&wnd_tmpl);

    mWidget* widget = ncsCreateMainWindowIndirect(&wnd_tmpl, host);

    Local<Object> widget_obj = wrap_widget_object(isolate, widget);

    args.GetReturnValue().Set(widget_obj);
}

static void mg_wrapWidget(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    if (args.Length() == 0) {
        isolate->ThrowException(
                toV8String(isolate, "Need a nativePtr of widget as paramter"));
        return ;
    }

    if (!args[0]->IsNumber()) {
        isolate->ThrowException(
                toV8String(isolate, "first paramter must be a number of nativePtr of wrap"));
        return ;
    }

    unsigned long ptr = TypeValue<unsigned long>::From(isolate, args[0]);
    if (ptr == 0) {
        return ;
    }
    mWidget * widget = (mWidget*)ptr;

    Local<Object> widget_obj = wrap_widget_object(isolate, widget);
    args.GetReturnValue().Set(widget_obj);
}

static void mg_MessageBox(const FunctionCallbackInfo<Value>& args) {
    mWidget * parent = NULL;
    string text;
    string caption;
    DWORD  style = 0;
    Isolate* isolate = args.GetIsolate();

    int argc = args.Length();
    if (argc > 0) {
        parent = getWidget(isolate, args[0]);
    }
    if (argc > 1) {
        text = toString(isolate, args[1]);
    }
    if (argc > 2) {
        caption = toString(isolate, args[2]);
    }
    if (argc > 3) {
        style = TypeValue<uint32_t>::From(isolate, args[4]);
    }

    MessageBox(parent ? parent->hwnd : HWND_DESKTOP, text.c_str(), caption.c_str(), style);
}

static void mg_DoModel(const FunctionCallbackInfo<Value>& args) {
    mWidget * parent = NULL;
    Isolate* isolate = args.GetIsolate();
    if (args.Length() < 2) {
        isolate->ThrowException(toV8String(isolate, "DoModel need 2 paramters"));
        return ;
    }
    parent = getWidget(isolate, args[0]);
    Local<Object> wnd_tmpl_obj = args[1]->ToObject();
    if (wnd_tmpl_obj.IsEmpty()) {
        isolate->ThrowException(toV8String(isolate, "DoModel recieve a un object paramters 1"));
        return ;
    }

    ThreadLocalHolder localHolder;

    HWND host = HWND_DESKTOP;
    if (parent) {
        host = parent->hwnd;
    }

    NCS_MNWND_TEMPLATE wnd_tmpl;

    init_mainwnd_tmpl(&wnd_tmpl, isolate, wnd_tmpl_obj);

    dumpWndTemplate(&wnd_tmpl);

    mMainWnd * main_wnd = (mMainWnd*)ncsCreateMainWindowIndirect(&wnd_tmpl, host);

    if (main_wnd) {
        DWORD ret = _c(main_wnd)->doModal(main_wnd, TRUE);
        args.GetReturnValue().Set(TypeValue<unsigned int>::To(isolate, ret));
    }
}

static mWidget * find_window(HWND hwnd, int id) {
    HWND hMainWnd = GetMainWindowHandle(hwnd);
    mWidget* main_wnd = ncsObjFromHandle(hMainWnd);
    if (main_wnd) {
        return (mWidget*)(_c(main_wnd)->getChild(main_wnd, id));
    }
    return NULL;
}

static mWidget * find_window(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    if (args.Length() < 2) {
        isolate->ThrowException(toV8String(isolate, "findWindow need 2 paramters"));
        return NULL;
    }

    mWidget * parent = getWidget(isolate, args[0]);
    int id = TypeValue<int>::From(isolate, args[1]);
    if (parent == NULL) {
        return NULL;
    }
    return find_window(parent->hwnd, id);
}

static void mg_findWnd(const FunctionCallbackInfo<Value>& args) {
    mWidget * widget = find_window(args);
    args.GetReturnValue().Set(TypeValue<unsigned long>::To(args.GetIsolate(), (unsigned long)widget));
}

static void mg_findWndObject(const FunctionCallbackInfo<Value>& args) {
    mWidget * widget = find_window(args);
    if (widget) {
        args.GetReturnValue().Set(wrap_widget_object(args.GetIsolate(), widget));
    }
}


#define ADD_CONSTANTEX(name, Type, value) \
    global->Set(context, toV8String(isolate, name), TypeValue<Type>::To(isolate, (Type)(value)))

#define ADD_INT_CONST(name) ADD_CONSTANTEX(#name, uint32_t, name)

static void init_constants(Isolate* isolate, Local<Context> context) {
    Local<Object> global = context->Global();

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

    init_global_const_values(isolate, context, global);
}

static bool init_global_objects(Isolate *isolate, Local<Context> context) {
    HandleScope handle_scope(isolate);
    TryCatch try_catch(isolate);

    Local<ObjectTemplate> tmpl = ObjectTemplate::New(isolate);

    tmpl->Set(toV8String(isolate, "CreateMainWindow"), FunctionTemplate::New(isolate, mg_createMainWindow));
    tmpl->Set(toV8String(isolate, "wrap"), FunctionTemplate::New(isolate, mg_wrapWidget));
    tmpl->Set(toV8String(isolate, "MessageBox"), FunctionTemplate::New(isolate, mg_MessageBox));
    tmpl->Set(toV8String(isolate, "DoModel"), FunctionTemplate::New(isolate, mg_DoModel));
    tmpl->Set(toV8String(isolate, "findWnd"), FunctionTemplate::New(isolate, mg_findWnd));
    tmpl->Set(toV8String(isolate, "findWndObject"), FunctionTemplate::New(isolate, mg_findWndObject));

    //Context::Scope context_scope(context);

    Local<Object> mgobj = tmpl->NewInstance(context).ToLocalChecked();


    if (!context->Global()->Set(context, toV8String(isolate, "mg"), mgobj).FromMaybe(false)) {
        dumpV8Exception(isolate, &try_catch);
        return false;
    }

    init_constants(isolate, context);

    return true;
}


class ShellArrayBufferAllocator : public ArrayBuffer::Allocator {
 public:
  virtual void* Allocate(size_t length) {
    void* data = AllocateUninitialized(length);
    return data == NULL ? data : memset(data, 0, length);
  }
  virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
  virtual void Free(void* data, size_t) { free(data); }
};



static ShellArrayBufferAllocator array_buffer_allocator;


static void Print(const FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handle_scope(args.GetIsolate());
    ThreadLocalHolder localHolder;

    int len = args.Length();
    if (len <= 0) {
        ALOGI("MiniGUI V8 Print", "");
        return ;
    }

    for (int i = 0; i < len; i++) {
        ALOGI("MiniGUI V8 Print", "%s", TypeValue<const char*>::From(args.GetIsolate(), args[i]));
    }
}

static bool init_v8() {
    if (sIsolate != NULL)
        return true;

    V8::InitializeICU();
    Platform* platform = v8::platform::CreateDefaultPlatform();
    V8::InitializePlatform(platform);
    V8::Initialize();

    Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = &array_buffer_allocator;
    sIsolate = v8::Isolate::New(create_params);


    Isolate::Scope isolate_scope(sIsolate);
    HandleScope handle_scope(sIsolate);

    init_widget_classes_methods();

    //create context
    Local<ObjectTemplate> global = ObjectTemplate::New(sIsolate);

    global->Set(toV8String(sIsolate, "print"), FunctionTemplate::New(sIsolate, Print));

    Local<Context> context = Context::New(sIsolate, NULL, global);
    init_global_objects(sIsolate, context);
    sContext.Reset(sIsolate, context);
    return true;
}

extern "C" unsigned long RunV8Script(const char* script_source, const char* filename) {
    if (!init_v8()) {
        ALOGE("MiniGUI V8", "Init v8 failed");
        return 0;
    }

    Isolate* isolate = sIsolate;
    string wrap_source = "(function(){";

    wrap_source += script_source;
    wrap_source += "})();";

    Isolate::Scope isolate_scope(isolate);
    HandleScope handle_scope (isolate);


    Local<Context> context = Local<Context>::New(isolate, sContext);
    Context::Scope context_scope(context);

    Local<String> v8_file_name = toV8String(isolate, filename);

    Local<String> v8_source = toV8String(isolate, wrap_source.c_str());

    TryCatch try_catch(isolate);
    ScriptOrigin origin(v8_file_name);
    Local<Script> script;



    if (!Script::Compile(context, v8_source, &origin).ToLocal(&script)) {

        dumpV8Exception(isolate, &try_catch);
        return 0;
    }

    Local<Value> result;

    if (!script->Run(context).ToLocal(&result)) {
        dumpV8Exception(isolate, &try_catch);
        return 0;
    }

    if (!result->IsObject()) {
        ALOGE("MiniGUI V8", "Cannot get the result of object:%s", filename);
        return 0;
    }

    mWidget *widget = getWrapWidget(result->ToObject(context).ToLocalChecked());

    if (widget == NULL) {
        ALOGE("MiniGUI V8", "Cannot get the widget wrap object:%s", filename);
        return 0;
    }

    return (unsigned long)(widget->hwnd);
}
}
