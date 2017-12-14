
#define SET_CONST(name, value) \
	global->Set(context, toV8String(isolate, name), TypeValue<int>::To(isolate, value));

class ObjectWrap {
    Local<Object> obj;
    Isolate *isolate;
public:
    ObjectWrap(): isolate(NULL) { }

    void set(Isolate *i, const Local<Object>& o) {
        isolate = i;
        obj = o;
    }

    template<typename T>
    vector<T> toArray() {
        if (!obj->IsArray()) {
            return vector<T>();
        }
        Local<Array> arr = Local<Array>::Cast(obj);
        vector<T> vec;
        for (int i = 0; i < arr->Length(); i++) {
            vec.push_back(TypeValue<T>::From(isolate,
                    arr->Get(isolate->GetCurrentContext(),i).ToLocalChecked()));
        }
        return vec;
    }
};

class MethodArgWrap {
	const FunctionCallbackInfo<Value> & args;
	Local<Object> obj_args;
	bool is_obj_args;
	Isolate *isolate;

	Local<Value> getArg(int index, const char* name) {
		if (is_obj_args) {
			return obj_args->Get(isolate->GetCurrentContext(),
					toV8String(isolate, name)).ToLocalChecked();
		}
		return args[index];
	}

public:
	MethodArgWrap(const FunctionCallbackInfo<Value>& args_)
	: args(args_) {
		if(args[0]->IsObject()) {
			obj_args = Local<Object>::Cast(args[0]);
			is_obj_args = true;
		} else {
			is_obj_args = false;
		}
		isolate = args.GetIsolate();
	}

	void getArg(int index, const char* name, string& strval, const char* defval) {
		Local<Value> arg = getArg(index, name);
		if (arg.IsEmpty() || !arg->IsString()) {
			strval = defval ? defval : "";
		} else {
			strval = toString(isolate, arg);
		}
	}

    void getArg(int index, const char* name, ObjectWrap& value, const ObjectWrap* pdef ) {
        Local<Value> arg = getArg(index, name);
        if (!arg.IsEmpty() && arg->IsObject()) {
            value.set(isolate, Local<Object>::Cast(arg));
        }
    }

	template<typename T>
	void getArg(int index, const char* name, T& value, const T defval) {
		Local<Value> arg = getArg(index, name);
		if (arg.IsEmpty()) {
			value = defval;
		} else {
			value = TypeValue<T>::From(isolate, arg);
		}
	}
};


#define DECLARE_PARAMTER(Type, index, name, defval) \
	Type name; arg_wrap.getArg(index, #name, name, defval);

#define BEGIN_IMPLEMENT_CLASS_METHOD(classname, methodname) \
namespace classname { \
static void mg_##methodname (const FunctionCallbackInfo<Value> & args) { \
	MethodArgWrap arg_wrap(args); \
	m##classname *self = (m##classname*) getWrapWidget(args.This());

#define END_IMPLEMENT_CLASS_METHOD }}

#define DECLARE_RETURN(ret_type)  ret_type ret;

#define SET_RETURN(ret_type) args.GetReturnValue().Set( \
		TypeValue<ret_type>::To(args.GetIsolate(), ret));


struct MethodEntry {
	const char* methodName;
	FunctionCallback callback;
};

#define BEGIN_REGISTER_CLASSES \
static void register_widget_classes_methods(map<mWidgetClass*, MethodEntry*>& maps) {

#define END_REGISTER_CLASSES }

#define BEGIN_REGISTER_CLASS(name) \
	{\
		namespace x = name; \
		static MethodEntry method_entries [] = {

#define REGISTER_METHOD(method_name) \
			{ #method_name, x::mg_##method_name},

#define END_REGISTER_CLASS(name) \
            {NULL, NULL}};\
		maps[(mWidgetClass*)(&Class(m##name))] = method_entries;\
	}

#include "widget_methods_define.cpp"


static void init_global_const_values(Isolate *isolate, Local<Context> context, Local<Object> global) {
	DEF_CONST(SET_CONST)
}

static map<mWidgetClass*, MethodEntry*> *pWidgetMethodEntries;

static void init_widget_classes_methods() {
    if (pWidgetMethodEntries == NULL) {
        pWidgetMethodEntries = new map<mWidgetClass*, MethodEntry*>;
        register_widget_classes_methods(*pWidgetMethodEntries);
    }
}

static MethodEntry* get_widget_method_entries(mWidgetClass* widget_class) {
    if (pWidgetMethodEntries == NULL) {
        return NULL;
    }

    map<mWidgetClass*, MethodEntry*>::iterator it = pWidgetMethodEntries->find(widget_class);
    if (it == pWidgetMethodEntries->end()) {
        return NULL;
    }
    return it->second;
}

static void init_widget_methods(Isolate* isolate, mWidgetClass*  widget_class, Local<ObjectTemplate> obj_tmpl) {
    MethodEntry * methods = get_widget_method_entries(widget_class);

    if (methods == NULL) {
        return ;
    }

    for(int i = 0; methods[i].methodName; i++) {
        ALOGE("MiniGUIV8", "Register Methods: %s.%s", widget_class->typeName, methods[i].methodName);
        obj_tmpl->Set(toV8String(isolate, methods[i].methodName),
                    FunctionTemplate::New(isolate, methods[i].callback));
    }
}

