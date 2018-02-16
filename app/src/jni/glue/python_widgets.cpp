
//define the property and methods for widgets

typedef struct {
    PyObject_HEAD
    mWidget * widget;
}PyWidgetWrap;

static PyTypeObject widget_class_base_type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "Widget",
    sizeof(PyWidgetWrap),
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_compare */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
    Py_TPFLAGS_BASETYPE | Py_TPFLAGS_TYPE_SUBCLASS,
                                /* tp_flags */
    "Widget objects",           /* tp_doc */

};

static mWidget* getWrapWidget(PyObject* self) {
    PyWidgetWrap * wrap = (PyWidgetWrap*)self;
    return wrap ? wrap->widget : NULL;
}

static PyObject* fromPropValue(PropType* prop_type, PValue &val) {
    switch(prop_type->getType()) {
    case PropType::INT:
    case PropType::GROUP:
        return PyInt_FromLong(val.d.lval);
    case PropType::STRING:
    case PropType::TEXT:
    case PropType::IMAGE:
    case PropType::FONT:
        return PyString_FromString(val.d.strval);
    case PropType::ENUM: {
        string enum_value = ((EnumType*)prop_type)->getNameByValue((DWORD)val.d.uval);
        return PyString_FromString(enum_value.c_str());
    }
    case PropType::COLOR:
        return PyInt_FromSize_t(val.d.uval);
    }
    return Py_None;
}

static PyObject* fromValue(int val) {
    return PyInt_FromLong(val);
}
static PyObject* fromValue(unsigned int val) {
    return PyInt_FromLong((long)val);
}
static PyObject* fromValue(long val) {
    return PyInt_FromLong(val);
}
static PyObject* fromValue(unsigned long val) {
    return PyInt_FromSize_t((size_t)val);
}
static PyObject* fromValue(const string& str) {
    return PyString_FromString(str.c_str());
}
static PyObject* fromValue(float f) {
    return PyFloat_FromDouble(f);
}
static PyObject* fromValue(double d) {
    return PyFloat_FromDouble(d);
}
static PyObject* fromValue(bool b) {
    return PyBool_FromLong(b);
}

static void toValue(PyObject* py_val, int& val) {
    val = (int)PyInt_AsLong(py_val);
}
static void toValue(PyObject* py_val, long& val) {
    val = PyInt_AsLong(py_val);
}
static void toValue(PyObject* py_val, float& f) {
    f = (float)PyFloat_AsDouble(py_val);
}
static void toValue(PyObject* py_val, double& d) {
    d = PyFloat_AsDouble(py_val);
}
static void toValue(PyObject* py_val, string& str) {
    str = PyString_AsString(py_val);
}
static void toValue(PyObject* py_val, bool& b) {
    b = PyInt_AsLong(py_val) != 0;
}

class ObjectWrap {
    PyObject *obj;
public:
    ObjectWrap() : obj(NULL) { }

    void set(PyObject* ob) {
        obj = ob;
    }

    template<typename T>
    vector<T> toArray() {
        if (!obj || !PyList_Check(obj)) {
            return vector<T>();
        }
        vector<T> vec;
        for (int i = 0; i < PyList_Size(obj); i ++) {
            T t;
            toValue(PyList_GetItem(obj, i), t);
            vec.push_back(t);
        }
        return vec;
    }
};

static void toValue(PyObject* py_val, ObjectWrap& w) {
    w.set(py_val);
}


class MethodArgWrap {
    PyObject *unpacked_args[10];
    int count;
    bool is_obj_args;

    PyObject* get(int index, const char* name) {
        if (is_obj_args) {
            return PyDict_GetItem(unpacked_args[0], PyString_FromString(name));
        }

        if (index < count)
            return unpacked_args[index];
        return Py_None;
    }

public:
    MethodArgWrap(PyObject* args) {
        memset(unpacked_args, 0, sizeof(unpacked_args));
        is_obj_args = false;
        count = 0;
        bool bok = PyArg_UnpackTuple(args, "mWidget Paramter", 0, \
                sizeof(unpacked_args)/sizeof(unpacked_args[0]),
                &unpacked_args[0],
                &unpacked_args[1],
                &unpacked_args[2],
                &unpacked_args[3],
                &unpacked_args[4],
                &unpacked_args[5],
                &unpacked_args[6],
                &unpacked_args[7],
                &unpacked_args[8],
                &unpacked_args[9]);

        if (!bok)
            return;

        while (count < sizeof(unpacked_args)/sizeof(unpacked_args[0])
                && unpacked_args[count]) count ++;

        is_obj_args = (count == 1 && PyDict_Check(unpacked_args[0]));
    }

    template<typename T>
    void getArg(int index, const char* name, T& val, const T& def) {
        PyObject* py_val = get(index, name);
        if (py_val == NULL || py_val == Py_None) {
            val = def;
        } else {
            toValue(py_val, val);
        }
    }
    void getArg(int index, const char* name, string& val, const char* def) {
        getArg(index, name, val, string(def));
    }
    void getArg(int index, const char* name, ObjectWrap& value, const ObjectWrap* pdef) {
        getArg(index, name, value, ObjectWrap());
    }
};


////////////////////////////////////////////////
//include the methods
#define BEGIN_REGISTER_CLASSES \
static void register_widget_classes_methods(map<mWidgetClass*, PyMethodDef*>& maps) {

#define END_REGISTER_CLASSES }

#define BEGIN_REGISTER_CLASS(name) \
{ \
    namespace x = name; \
    static PyMethodDef methods[] = {

#define REGISTER_METHOD(method_name) \
        {#method_name, (PyCFunction)x::mg_##method_name, METH_VARARGS, "wrap mWidget method " #method_name},

#define END_REGISTER_CLASS(name) \
        {NULL, NULL}\
    }; \
    maps[(mWidgetClass*)(&Class(m##name))] = methods; \
}

#define BEGIN_IMPLEMENT_CLASS_METHOD(classname, methodname) \
namespace classname { \
static PyObject* mg_##methodname(PyObject* py_self, PyObject* args) { \
    MethodArgWrap arg_wrap(args); \
    m##classname *self = (m##classname*) getWrapWidget(py_self); \
    PyObject* py_ret = Py_None;

#define END_IMPLEMENT_CLASS_METHOD \
    return py_ret; \
}}

#define DECLARE_RETURN(ret_type) ret_type ret;

#define SET_RETURN(ret_type) py_ret = fromValue(ret);

#define DECLARE_PARAMTER(Type, index, name, defval) \
    Type name; arg_wrap.getArg(index, #name, name, defval);


#include "widget_methods_define.cpp"

typedef map<mWidgetClass*, PyMethodDef*> WidgetMethodMap;
static WidgetMethodMap  *gWidgetMethodEntries;

static void init_widget_classes_methods() {
    if (gWidgetMethodEntries == NULL) {
        gWidgetMethodEntries = new WidgetMethodMap;
        register_widget_classes_methods(*gWidgetMethodEntries);
    }
}
static PyMethodDef* get_widget_method_entries(mWidgetClass* widget_class) {
    init_widget_classes_methods();
    if (gWidgetMethodEntries == NULL) {
        return NULL;
    }

    WidgetMethodMap::iterator it = gWidgetMethodEntries->find(widget_class);
    if (it == gWidgetMethodEntries->end()) {
        return NULL;
    }

    return it->second;
}

//////////////////////////////////////////////


static PyObject* py_widget_get_prop(PyObject* self, void* closure) {
    mWidget * widget = getWrapWidget(self);
    if (!widget) return Py_None;
    Property* prop = (Property*)closure;

    PValue val;

    if (getWidgetProperty(widget, prop, val)) {
        return fromPropValue(prop->type, val);
    }
}

static int py_widget_set_prop(PyObject* self, PyObject * value, void* closure) {
    mWidget * widget = getWrapWidget(self);
    if (!widget) return -1;
    Property* prop = (Property*)closure;

    ThreadLocalHolder localHolder;

    DWORD dwVal = toPropValue(prop->type, value);

    setWidgetProperty(widget, prop, dwVal);

    return 0;
}

static PyTypeObject * createWidgetClass(WidgetClassDefine* widget);

static PyTypeObject* getWidgetClass(WidgetClassDefine* widget) {
    return widget ? widget->getGlueObject<PyTypeObject>()
            : NULL;
}

static PyTypeObject* getWidgetClass(mWidgetClass* widget_class) {
    return widget_class ? getWidgetClass(
            WidgetClassDefine::getClassDefine(widget_class))
            : NULL;
}

static PyTypeObject* getWidgetClass(mWidget* widget) {
    return widget ? getWidgetClass(
            WidgetClassDefine::getClassDefine(_c(widget)))
            : NULL;
}

static PyTypeObject* getAndInstallWidgetClass(WidgetClassDefine *widget) {
    PyTypeObject* py_type = getWidgetClass(widget);
    if (!py_type) {
        py_type = createWidgetClass(widget);
        widget->setGlueObject(py_type);
    }
    return py_type;
}

static PyTypeObject* getAndInstallWidgetClass(mWidgetClass* widget_class) {
    return getAndInstallWidgetClass(WidgetClassDefine::getClassDefine(widget_class));
}


static PyTypeObject * createWidgetClass(WidgetClassDefine* widget) {
    PyTypeObject * type_obj = new PyTypeObject;

    memcpy(type_obj, &widget_class_base_type, sizeof(PyTypeObject));

    type_obj->tp_name = strdup(widget->getClassName());

    if (widget->getParent()) {
        type_obj->tp_base = getAndInstallWidgetClass(widget->getParent());
    }
    map<string,Property*> &props = widget->getProperties();

    PyGetSetDef * set_get_defs = NULL;

    if (props.size() > 0) {
        set_get_defs = new PyGetSetDef[props.size()+1];
        memset(set_get_defs, 0, sizeof(PyGetSetDef)*(props.size()+1));
    }
    int prop_idx = 0;
    for (map<string,Property*>::iterator it = props.begin(); it != props.end(); ++it) {
        Property* prop = it->second;

        PyGetSetDef * def = &set_get_defs[prop_idx ++];
        def->name = strdup(prop->name.c_str());
        def->get = py_widget_get_prop;
        def->set = py_widget_set_prop;
        def->closure = prop;
    }

    type_obj->tp_getset = set_get_defs;

    PyMethodDef* methods = get_widget_method_entries(widget->getOwnerClass());

    //ALOGE("==DJJ", "%s methods = %p", widget->getClassName(), methods);

    /*if (methods) {
        for (int i = 0; methods[i].ml_name; i++) {
            PyMethodDef &m = methods[i];
            ALOGE("==DJJ", "%s: method[%d]={%s, %p, %d, %s}", widget->getClassName(), i, m.ml_name, m.ml_meth, m.ml_flags, m.ml_doc);
        }
    }*/

    type_obj->tp_methods = methods;

    type_obj->tp_new = PyType_GenericNew;
    if (PyType_Ready(type_obj) < 0) {
        ALOGE("PyGlue", "Cannot ready the widget class:%s", widget->getClassName());
        return NULL;
    }


    Py_INCREF(type_obj);

    PyModule_AddObject(py_mg_module, widget->getClassName(), (PyObject*)type_obj);

    return type_obj;
}

static PyObject* wrap_widget_object(mWidget* widget) {
    PyTypeObject* py_type = getAndInstallWidgetClass(_c(widget));
    if (!py_type) return Py_None;

    PyWidgetWrap* py_obj = PyObject_NEW(PyWidgetWrap, py_type);
    py_obj->widget = widget;
    return (PyObject*)py_obj;
}

static mWidget* widget_from_pyobject(PyObject* py_obj) {
    if (py_obj == NULL
            ||!PyObject_TypeCheck(py_obj, getAndInstallWidgetClass(&Class(mWidget)))) {
        return NULL;
    }
    PyWidgetWrap* wrap = (PyWidgetWrap*)py_obj;
    return wrap ? wrap->widget : NULL;
}

