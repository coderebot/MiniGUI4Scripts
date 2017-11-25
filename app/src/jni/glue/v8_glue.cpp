#include <stdio.h>
#include "v8.h"
#include <map>
#include "glue_common.h"
#include "common.h"
#include "minigui.h"
#include "gdi.h"
#include "window.h"
#include "mgncs.h"
#include "miui-base.h"

using namespace v8;
using namespace glue;
using std::map;

template <typename R>
struct TypeValue { };

static Local<String> toV8String(Isolate* isolate, const char* str) {
    return v8::String::NewFromUtf8(isolate, str, v8::NewStringType::kInternalized).ToLocalChecked();
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
        return *strval;
    }
    static Local<Value> To(Isolate* isolate, const char* str) {
        return toV8String(isolate, str);
    }
};

template<typename T>
static inline Local<Value> toV8Value(Isolate* isolate, T t) {
    return TypeValue<T>::To(isolate, t);
}

static void dumpV8Exception(v8::Isolate* isolate, v8::TryCatch* tryCatch);
class EventHandlerManager {
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
        Local<Function> func = getHandler(code);
        if (func.IsEmpty()) {
            return Local<Value>();
        }

        Local<Object> reciever = Local<Object>::New(isolate, widgetObject);

        TryCatch tryCatch;
        MaybeLocal<Value> result = func->Call(isolate->GetCurrentContext(), reciever, argc, argv);

        if (tryCatch.HasCaught()) {
            dumpV8Exception(isolate, &tryCatch);
            return Local<Value>();
        }
        return result.ToLocalChecked();
    }

public:

    void addEventHanler(int code, Isolate* isolate, Local<Function> function) {
        functionMap[code].Reset(isolate, function);
    }


    template<typename R>
    R call(int code) {
        Local<Value> args[] = {
            toV8Value(isolate, code)
        };
        return (R)TypeValue<R>::From(isolate, callFunc(code, sizeof(args)/sizeof(args[0]), args));
    }

    template<typename R, typename A1>
    R call(int code, A1 a1) {
        Local<Value> args[] = {
            toV8Value(isolate, code),
            toV8Value(isolate, a1)
        };
        return (R)TypeValue<R>::From(isolate, callFunc(code, sizeof(args)/sizeof(args[0]), args));
    }

    template<typename R, typename A1, typename A2>
    R call(int code, A1 a1, A2 a2) {
        Local<Value> args[] = {
            toV8Value(isolate, code),
            toV8Value(isolate, a1),
            toV8Value(isolate, a2)
        };
        return (R)TypeValue<R>::From(isolate, callFunc(code, sizeof(args)/sizeof(args[0]), args));
    }

    template<typename R, typename A1, typename A2, typename A3>
    R call(int code, A1 a1, A2 a2, A3 a3) {
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


static inline EventHandlerManager * get_event_handler_manager(mWidget *w) {
    return (EventHandlerManager*) GetWidgetEventHandlers(w);
}


static BOOL do_onCreate(mWidget *self, DWORD dwAddData) {
    EventHandlerManager * m = get_event_handler_manager(self);
    if (m) {
        return m->call<bool>(MSG_CREATE);
    }
    return TRUE;
}

#define DO_NO_VOID(name, message) \
static void do_##name(mWidget* self) { \
    EventHandlerManager * m = get_event_handler_manager(self); \
    if (m) { \
        m->call<void>(message); \
    } \
}

DO_NO_VOID(onClose, MSG_CLOSE)

static void do_message(mWidget* self, int message) {
    EventHandlerManager * m = get_event_handler_manager(self);
    if (m) {
        m->call<void>(message);
    }
}

static BOOL do_onKey(mWidget* self, int message, int scancode, DWORD key_status) {
    EventHandlerManager * m = get_event_handler_manager(self);
    if (m) {
        m->call<void>(message, scancode, key_status);
    }
    return NCSR_CONTINUE_MSG;
}

static BOOL do_onMouse(mWidget* self, int message, int x, int y, DWORD key_status) {
    EventHandlerManager * m = get_event_handler_manager(self);
    if (m) {
        m->call<void>(message, x, y, key_status);
    }
    return NCSR_CONTINUE_MSG;
}

static void do_onNotify(mWidget* self, int id, int nc) {
    EventHandlerManager * m = get_event_handler_manager(self);
    if (m) {
        m->call<void>(NCS_NOTIFY_CODE(nc), id);
    }
}

typedef map<WidgetClassDefine*, Persistent<FunctionTemplate> > WidgetFunctionTemplateMap;

static WidgetFunctionTemplateMap sWidgetFunctionTemplates;
static Local<FunctionTemplate> createWidgetClass(Isolate* isolate, WidgetClassDefine *widget);

static Local<FunctionTemplate> getWidgetFunctionTemplate(Isolate* isolate, WidgetClassDefine *widget) {
        WidgetFunctionTemplateMap::iterator it = sWidgetFunctionTemplates.find(widget);
        if (it == sWidgetFunctionTemplates.end()) {
            return Local<FunctionTemplate>();
        }

        return Local<FunctionTemplate>::New(isolate, it->second);
}

static void addWidgetFunctionTemplate(Isolate* isolate, WidgetClassDefine* widget, Local<FunctionTemplate> func_tmpl) {
    sWidgetFunctionTemplates[widget].Reset(isolate, func_tmpl);
}


static void widget_prop_get(Local<String> property, const PropertyCallbackInfo<Value>& info) {
}


static void widget_prop_set(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
}

static Local<FunctionTemplate> getAndInstallWidgetFunctionTemplate(Isolate* isolate, WidgetClassDefine *widget) {
        WidgetFunctionTemplateMap::iterator it = sWidgetFunctionTemplates.find(widget);
        if (it == sWidgetFunctionTemplates.end()) {
            return createWidgetClass(isolate, widget);
        }

        return Local<FunctionTemplate>::New(isolate, it->second);
}

static Local<FunctionTemplate> createWidgetClass(Isolate* isolate, WidgetClassDefine *widget) {
    HandleScope handle_scope(isolate);

    Local<FunctionTemplate> func_tmpl = FunctionTemplate::New(isolate);

    func_tmpl->SetClassName(toV8String(isolate, widget->getClassName()));

    Local<ObjectTemplate> prototype = func_tmpl->PrototypeTemplate();

    if (widget->getParent()) {
        Local<FunctionTemplate> parent = getAndInstallWidgetFunctionTemplate(isolate, widget->getParent());
        func_tmpl->Inherit(parent);
    }

    vector<Property*> &props = widget->getProperties();
    for (vector<Property*>::iterator it = props.begin(); it != props.end(); ++it) {
        Property* prop = *it;
        Local<External> prop_data = External::New(isolate, prop);
        prototype->SetAccessor(toV8String(isolate, prop->name),
                prop->readable() ? widget_prop_get: NULL,
                prop->writeable() ? widget_prop_set: NULL,
                prop_data);
    }

    return func_tmpl;
}

static void dumpV8Exception(v8::Isolate* isolate, v8::TryCatch* tryCatch) {
    v8::String::Utf8Value exception(tryCatch->Exception());
    v8::Handle<v8::Message> message = tryCatch->Message();
    if (message.IsEmpty()) {
        ALOGE("WSSERVER", "%s", *exception);
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

        ALOGE("WSSERVER.ERR", "%s", sourceline_str);
        ALOGE("WSSERVER.ERR", "%s:%d(from %d to %d)", filenameString, lineNumber, start, end);

        v8::Local<v8::Value> stack_trace_string;
        if (tryCatch->StackTrace(isolate->GetCurrentContext()).ToLocal(&stack_trace_string)
                && stack_trace_string->IsString()) {
            v8::String::Utf8Value stack_trace(v8::Local<v8::String>::Cast(stack_trace_string));
            ALOGE("WSSERVER.ERR", "%s", *stack_trace);
        }
    }
}


