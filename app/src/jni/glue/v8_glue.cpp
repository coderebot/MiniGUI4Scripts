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
#include "miui-base.h"

using namespace v8;
using namespace glue;
using std::map;

template <typename R>
struct TypeValue { };

static Local<String> toV8String(Isolate* isolate, const char* str) {
    return v8::String::NewFromUtf8(isolate, str, v8::NewStringType::kInternalized).ToLocalChecked();
}

static const char* toString(Isolate* isolate, Local<Value> str) {
    if (str.IsEmpty()) {
        return NULL;
    }

    String::Utf8Value utf8str(str);
    return *utf8str;
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

static Local<Value> fromPropValue(Isolate* isolate, PropType *prop_type, DWORD dwVal) {
    switch(prop_type->getType()) {
    case PropType::INT:
    case PropType::GROUP:
        return TypeValue<int>::To(isolate, (int)dwVal);
    case PropType::STRING:
    case PropType::TEXT:
    case PropType::IMAGE:
    case PropType::FONT:
    case PropType::ENUM:
        return TypeValue<const char*>::To(isolate, (const char*)dwVal);
    case PropType::COLOR:
        return TypeValue<unsigned int>::To(isolate, dwVal);
    }
    return Local<Value>();
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



static Local<FunctionTemplate> createWidgetClass(Isolate* isolate, WidgetClassDefine *widget);

static Local<FunctionTemplate> getWidgetFunctionTemplate(Isolate* isolate, WidgetClassDefine *widget) {
    Persistent<FunctionTemplate>* f =
            widget->getGlueObject<Persistent<FunctionTemplate> >();

    if (f == NULL) {
        return Local<FunctionTemplate>();
    }
    return Local<FunctionTemplate>::New(isolate, *f);
}

static Local<FunctionTemplate> getWidgetFunctionTemplate(Isolate* isolate, mWidget *widget) {
    WidgetClassDefine * widget_clzz_def = WidgetClassDefine::getClassDefine(_c(widget));
    return getWidgetFunctionTemplate(isolate, widget_clzz_def);
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

static mWidget* getWrapWidget(Local<Object> obj) {
    Local<External> field = Local<External>::Cast(obj->GetInternalField(0));
    void *ptr = field->Value();
    return static_cast<mWidget*>(ptr);
}

static Property* getWrapProperty(Local<Value> obj) {
    Local<External> prop = Local<External>::Cast(obj);
    return static_cast<Property*>(prop->Value());
}

static void widget_prop_get(Local<String> property, const PropertyCallbackInfo<Value>& info) {
    mWidget *widget = getWrapWidget(info.Holder());
    Property* prop = getWrapProperty(info.Data());

    DWORD dwVal = _c(widget)->getProperty(widget, prop->id);

    info.GetReturnValue().Set(fromPropValue(info.GetIsolate(), prop->type, dwVal));
}


static void widget_prop_set(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
    mWidget *widget = getWrapWidget(info.Holder());
    Property* prop = getWrapProperty(info.Data());

    DWORD dwVal = toPropValue(info.GetIsolate(), prop->type, value);

    _c(widget)->setProperty(widget, prop->id, dwVal);

    info.GetReturnValue().Set(value);
}

static Local<FunctionTemplate> getAndInstallWidgetFunctionTemplate(Isolate* isolate, WidgetClassDefine *widget) {
    Persistent<FunctionTemplate>* f =
            widget->getGlueObject<Persistent<FunctionTemplate> >();
    if (f == NULL) {
        f = new Persistent<FunctionTemplate>();
        widget->setGlueObject(f);
        createWidgetClass(isolate, widget);
    }
    return getWidgetFunctionTemplate(isolate, widget);
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

    Local<ObjectTemplate> instance_tmpl = func_tmpl->InstanceTemplate();
    instance_tmpl->SetInternalFieldCount(1);

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

static void init_wnd_tmpl(NCS_WND_TEMPLATE* pwnd_tmpl, Isolate *isolate, Local<Object> wnd_tmpl_obj) {
    memset(pwnd_tmpl, 0, sizeof(*pwnd_tmpl));

    map<int, DWORD> props;
    EventHandlerManager * pevent_handler_mgr = NULL;

    Local<Array> prop_names = wnd_tmpl_obj->GetOwnPropertyNames(isolate->GetCurrentContext()).ToLocalChecked();
    for (int i = 0; i < prop_names->Length(); i++) {
        Local<Value> key = prop_names->Get(isolate->GetCurrentContext(), i).ToLocalChecked();
        Local<Value> value = wnd_tmpl_obj->Get(isolate->GetCurrentContext(), key).ToLocalChecked();
        const char* str_key = toString(isolate, key);
        if (str_key == NULL) {
            continue; //TODO error
        }

        if (strcmp(str_key, "children") == 0
                && value->IsArray()) {
            Local<Array> children = Local<Array>::Cast(value);
            int count = children->Length();
            NCS_WND_TEMPLATE * ctrls = new NCS_WND_TEMPLATE[count];
            for (int chl = 0; chl < count; chl ++) {
                Local<Value> ctrl_tmpl_obj = children->Get(isolate->GetCurrentContext(), chl).ToLocalChecked();
                init_wnd_tmpl(&ctrls[chl], isolate, Local<Object>::Cast(ctrl_tmpl_obj));
            }
            pwnd_tmpl->ctrls = ctrls;
            pwnd_tmpl->count = count;
        } else if(strncmp(str_key, "on", 2) == 0
                && value->IsFunction()) {
            int message = GetEventIdByName(str_key);
            if (pevent_handler_mgr == NULL) {
                pevent_handler_mgr = new EventHandlerManager(isolate);
            }
            pevent_handler_mgr->addEventHanler(message, isolate, Local<Function>::Cast(value));
        } else {
            Property * prop = Property::getProperty(str_key);
            DWORD dwVal = toPropValue(isolate, prop->type, value);
            int id = prop->id;
            if (id < 1000) {
                props[id] = dwVal;
            } else if(id < 2000) {
                switch(id) {
                case 1000: //className
                    pwnd_tmpl->class_name = (const char*)dwVal;
                    break;
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
        }
    }

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

    if (pevent_handler_mgr) {
        pwnd_tmpl->handlers = pevent_handler_mgr->createHandlers();
    }
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
    if (args.Length() != 0) {
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

    NCS_MNWND_TEMPLATE wnd_tmpl;

    HWND host = HWND_DESKTOP;

    mWidget* widget = ncsCreateMainWindowIndirect(&wnd_tmpl, host);

    Local<Object> widget_obj = wrap_widget_object(isolate, widget);

    args.GetReturnValue().Set(widget_obj);

}


static bool init_global_objects(Isolate *isolate, Local<Context> & context) {
    HandleScope handle_scope(isolate);

    Local<Object> mgobj = Object::New(isolate);

    mgobj->Set(context, toV8String(isolate, "createMainWindow"), Function::New(context, mg_createMainWindow).ToLocalChecked());

    context->Global()->Set(context, toV8String(isolate, "mg"), mgobj);
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


static Isolate* sIsolate;
static Persistent<Context> sContext;
static ShellArrayBufferAllocator array_buffer_allocator;


static void Print(const FunctionCallbackInfo<v8::Value>& args) {
    HandleScope handle_scope(args.GetIsolate());

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
    //create context
    Local<ObjectTemplate> global = ObjectTemplate::New(sIsolate);
    Local<Context> context = Context::New(sIsolate, NULL, global);

    global->Set(toV8String(sIsolate, "print"), FunctionTemplate::New(sIsolate, Print));

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

    Local<String> v8_file_name = toV8String(isolate, filename);

    Local<String> v8_source = toV8String(isolate, script_source);

    HandleScope handle_scope (isolate);
    TryCatch try_catch(isolate);
    ScriptOrigin origin(v8_file_name);
    Local<Context> context(isolate->GetCurrentContext());
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

    return TypeValue<unsigned long>::From(isolate, result);

}


