
#define SET_CONST(name, value) \
	global->Set(context, toV8String(isolate, name), TypeValue<int>::To(isolate, value));


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
	m##classname *self = (m##classname*) getWrapWidget(args.Holder());

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
		};\
		maps[(mWidgetClass*)(&Class(m##name))] = method_entries;\
	}

#include "widget_methods_define.cpp"


static void init_global_const_values(Isolate *isolate, Local<Context> context, Local<Object> global) {
	DEF_CONST(SET_CONST)
}


