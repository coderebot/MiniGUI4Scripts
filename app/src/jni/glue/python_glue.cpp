#include <stdio.h>
#include <string.h>
#include <Python.h>
#include "mg-base.h"
#include "common.h"
#include "minigui.h"
#include "gdi.h"
#include "window.h"
#include "mgncs.h"
#include "glue_common.h"
#include "glue_utils.h"
using namespace glue;

namespace python_glue {

static PyObject *py_mg_module = NULL;

static void dumpPythonError() {
    PyObject *exception, *v, *tb;
    PyErr_Fetch(&exception, &v, &tb);

    if (exception == NULL)
        return ;

    if (Py_FlushLine())
        PyErr_Clear();

    const char* className = NULL;
    const char* errValue = NULL;

    if (PyExceptionClass_Check(exception)) {
        className = PyExceptionClass_Name(exception);
    }

    if (v) {
        errValue = PyString_AsString(v);
    }

    ALOGE("PyGlue", "dumpPython: Exception:%s %s", className, errValue);
}

static PyObject * mg_log(PyObject* self, PyObject* args) {
    char* info = NULL;
    if (!PyArg_ParseTuple(args, "s", &info)) {
        return NULL;
    }

    ALOGI("MGLOG", "%s", info);
    return Py_None;
}

static const char* toHolderString(PyObject *value) {
    const char* str_value = PyString_AsString(value);
    if (!str_value) return NULL;
    char* dup_str = strdup(str_value);
    ThreadLocalHolder::push(dup_str);
    return dup_str;
}

static DWORD toPropValue(PropType *prop_type, PyObject* value) {
    switch(prop_type->getType()) {
    case PropType::INT:
    case PropType::GROUP:
        return (DWORD)PyInt_AsLong(value);
    case PropType::STRING:
    case PropType::TEXT:
    case PropType::IMAGE:
    case PropType::FONT:
        return prop_type->from(toHolderString(value));
    case PropType::ENUM:
        return prop_type->from(PyString_AsString(value));
    case PropType::COLOR:
        return (DWORD)PyInt_AsUnsignedLongMask(value);
    }

    return 0;
}

#include "python_widgets.cpp"

template<typename R>
struct ReturnValue {
    static R from(PyObject *py_obj) {
        R ret;
        toValue(py_obj, ret);
        return ret;
    }
};
template<>
struct ReturnValue<void> {
    static void from(PyObject*) { }
};

template<typename T> struct TypeInfo { };
#define TYPEINFO(Type, Sig) \
template<> struct TypeInfo<Type> { static const char* signature() { return Sig; }};

TYPEINFO(int, "i")
TYPEINFO(unsigned int, "I")
TYPEINFO(long, "l")
TYPEINFO(unsigned long, "l")
TYPEINFO(float, "f")
TYPEINFO(double, "d")
TYPEINFO(char*, "s")

struct PyScript {
    typedef PyObject* persistent_handler_t;
    typedef PyObject* local_handler_t;

    TEventHandlerManager<PyScript> * manager;

    PyScript(TEventHandlerManager<PyScript> * m)
    : manager(m) { }

    static bool isFunc(PyObject* func) {
        return PyCallable_Check(func) != 0;
    }

    static void addRef(PyObject* func) {
        Py_INCREF(func);
    }
    static void release(PyObject* obj) {
        Py_DECREF(obj);
    }

    static void assign(PyObject* & d, PyObject* s) {
        d = s;
    }

    static local_handler_t toNullLocal() {
        return NULL;
    }

    void setWidgetObject(mWidget* ) {
        //Do nothing
    }

    PyObject * callFunc(int code, const char* format, ...) {
        PyObject * py_ret = Py_None;
        va_list va;
        va_start(va, format);
        PyObject* func = manager->getHandler(code);
        if (func) {
            PyObject * args = Py_VaBuildValue(format, va);
            py_ret = PyObject_CallObject(func, args);
            if(PyErr_Occurred()) {
                ALOGE("PyGlue", "callFunc Error code=%d, format=\'%s\'", code, format);
                dumpPythonError();
            }
        }
        va_end(va);
        return py_ret;
    }

    template<typename R>
    R call(int code) {
        return (R)ReturnValue<R>::from(callFunc(code, "i", code));
    }
    template<typename R, typename A1>
    R call(int code, const A1& a1) {
        char signatures[60] = "i";
        strcat(signatures, TypeInfo<A1>::signature());
        return (R)ReturnValue<R>::from(
                callFunc(code, signatures, code, a1));
    }
    template<typename R, typename A1, typename A2>
    R call(int code, const A1& a1, const A2& a2) {
        char signatures[60] = "i";
        strcat(signatures, TypeInfo<A1>::signature());
        strcat(signatures, TypeInfo<A2>::signature());
        return (R)ReturnValue<R>::from(
                callFunc(code, signatures, code, a1, a2));
    }
    template<typename R, typename A1, typename A2, typename A3>
    R call(int code, const A1& a1, const A2& a2, const A3& a3) {
        char signatures[60] = "i";
        strcat(signatures, TypeInfo<A1>::signature());
        strcat(signatures, TypeInfo<A2>::signature());
        strcat(signatures, TypeInfo<A3>::signature());
        return (R)ReturnValue<R>::from(
                callFunc(code, signatures, code, a1, a2, a3));
    }
    template<typename R, typename A1, typename A2, typename A3, typename A4>
    R call(int code, const A1& a1, const A2& a2, const A3& a3, const A4 & a4) {
        char signatures[60] = "i";
        strcat(signatures, TypeInfo<A1>::signature());
        strcat(signatures, TypeInfo<A2>::signature());
        strcat(signatures, TypeInfo<A3>::signature());
        strcat(signatures, TypeInfo<A4>::signature());
        return (R)ReturnValue<R>::from(
                callFunc(code, signatures, code, a1, a2, a3, a4));
    }
    template<typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
    R call(int code, const A1& a1, const A2& a2, const A3& a3, const A4 & a4, A5 & a5) {
        char signatures[60] = "i";
        strcat(signatures, TypeInfo<A1>::signature());
        strcat(signatures, TypeInfo<A2>::signature());
        strcat(signatures, TypeInfo<A3>::signature());
        strcat(signatures, TypeInfo<A4>::signature());
        strcat(signatures, TypeInfo<A5>::signature());
        return (R)ReturnValue<R>::from(
                callFunc(code, signatures, code, a1, a2, a3, a4, a5));
    }

};

typedef TEventHandlerManager<PyScript> EventHandlerManager;



static WidgetClassDefine * get_widget_class_define(NCS_WND_TEMPLATE* pwnd_tmpl, PyObject *dict) {
    PyObject * class_name = PyString_FromString("Class");
    PyObject * py_value = PyDict_GetItem(dict, class_name);
    Py_DECREF(class_name);

    if (py_value) {
        pwnd_tmpl->class_name = PyString_AsString(py_value);
    }

    if (pwnd_tmpl->class_name) {
        return WidgetClassDefine::getClassDefine(pwnd_tmpl->class_name);
    }
    return NULL;
}


static void init_wnd_tmpl(NCS_WND_TEMPLATE* pwnd_tmpl, PyObject* dict) {
    WndTemplateBuilder builder(pwnd_tmpl);

    WidgetClassDefine* widgetClassDefine = get_widget_class_define(pwnd_tmpl, dict);

    EventHandlerManager * pevent_handler_mgr = NULL;


    if (widgetClassDefine) {
        widgetClassDefine->initWndTemplateByDefaults(&builder);
    }

    PyObject *name, *value;
    Py_ssize_t pos = 0;

    while(PyDict_Next(dict, &pos, &name, &value)) {
        const char* str_name = PyString_AsString(name);
        if (str_name == NULL) {
            continue;
        }

        if (strcmp(str_name, "children") == 0
                && PyList_Check(value)) {
            int count = PyList_Size(value);
            if (count <= 0) {
                continue;
            }

            NCS_WND_TEMPLATE* ctrls = new NCS_WND_TEMPLATE[count];
            memset(ctrls, 0, sizeof(NCS_WND_TEMPLATE)*count);
            for (int i = 0; i < count; i++) {
                init_wnd_tmpl(&ctrls[i], PyList_GetItem(value, i));
            }
            pwnd_tmpl->ctrls = ctrls;
            pwnd_tmpl->count = count;

        } else if (strcmp(str_name, "id") == 0) {
            pwnd_tmpl->id = PyInt_AsLong(value);
        } else if (strcmp(str_name, "Class") == 0) {
            continue;
        } else if(strncmp(str_name, "on", 2) == 0
                && PyCallable_Check(value)) {
            int message = widgetClassDefine->getEvent(str_name);
            if (pevent_handler_mgr == NULL) {
                pevent_handler_mgr = new EventHandlerManager();
            }

            pevent_handler_mgr->addEventHandler(message, value);

        } else {
            Property * prop = Property::getProperty(str_name);
            if (prop == NULL) {
                ALOGE("PyGlue", "Unspport property:%s", str_name);
                continue;
            }
            DWORD dwVal = toPropValue(prop->type, value);
            builder.setProp(prop->id, dwVal);
        }
    }

    if (pevent_handler_mgr) {
        pevent_handler_mgr->apply(pwnd_tmpl);
    }

    builder.build();

}


static void init_mainwnd_tmpl(NCS_MNWND_TEMPLATE* pmainwnd_tmpl, PyObject* dict) {
    memset(pmainwnd_tmpl, 0, sizeof(NCS_MNWND_TEMPLATE));
    pmainwnd_tmpl->class_name = "MainWnd";

    init_wnd_tmpl((NCS_WND_TEMPLATE*)pmainwnd_tmpl, dict);
}


static mWidget* s_last_widget;

static PyObject * mg_createMainWindow(PyObject *self, PyObject * args) {
    PyObject * dict = NULL;
    if (!PyArg_ParseTuple(args, "O", &dict)) {
        ALOGE("PyGlue:", "cannot parse object in createMainWindow");
        return NULL;
    }

    if (!PyDict_Check(dict)) {
        ALOGE("PyGlue:", "createMainWindow paramter is not a dict");
        return NULL;
    }

    ThreadLocalHolder localHolder;
    NCS_MNWND_TEMPLATE wnd_tmpl;
    HWND host = HWND_DESKTOP;

    init_mainwnd_tmpl(&wnd_tmpl, dict);

    mWidget * widget = ncsCreateMainWindowIndirect(&wnd_tmpl, host);
    s_last_widget = widget;

    return wrap_widget_object(widget);

}

static PyObject* mg_wrap(PyObject* self, PyObject* args) {
    unsigned long ptr;
    if (!PyArg_ParseTuple(args, "l", &ptr)) {
        ALOGE("PyGlue:", "cannot get widget pointer");
        return Py_None;
    }

    mWidget * widget = (mWidget*)ptr;
    if (widget) return wrap_widget_object(widget);
    return Py_None;
}

static PyObject* mg_MessageBox(PyObject* self, PyObject* args) {
    mWidget* parent = NULL;
    const char* caption = "caption";
    const char* text = "information";
    DWORD style = 0;

    PyArg_ParseTuple(args, "|lssI",&parent, &text, &caption, &style);

    MessageBox(parent ? parent->hwnd : HWND_DESKTOP,
            text, caption, style);

    return Py_None;
}

static PyObject* mg_DoModal(PyObject* self, PyObject* args) {
    PyObject * dict = NULL;
    mWidget* host = NULL;
    if (!PyArg_ParseTuple(args, "O|l", &dict, &host)) {
        ALOGE("PyGlue:", "cannot parse object in createMainWindow");
        return NULL;
    }

    if (!PyDict_Check(dict)) {
        ALOGE("PyGlue:", "createMainWindow paramter is not a dict");
        return NULL;
    }

    ThreadLocalHolder localHolder;
    NCS_MNWND_TEMPLATE wnd_tmpl;

    init_mainwnd_tmpl(&wnd_tmpl, dict);

    dumpWndTemplate(&wnd_tmpl);

    mMainWnd* main_wnd = (mMainWnd*)ncsCreateMainWindowIndirect(&wnd_tmpl, host ? host->hwnd : HWND_DESKTOP);

    if (main_wnd) {
        DWORD ret = _c(main_wnd)->doModal(main_wnd, TRUE);
        return PyInt_FromLong((int)ret);
    }

    return Py_None;
}

static mWidget * find_window(HWND hwnd, int id) {
    HWND hMainWnd = GetMainWindowHandle(hwnd);
    mWidget* main_wnd = ncsObjFromHandle(hMainWnd);
    if (main_wnd) {
        return (mWidget*)(_c(main_wnd)->getChild(main_wnd, id));
    }
    return NULL;
}

static PyObject* mg_findWndObject(PyObject* self, PyObject* args) {
    unsigned long ptr;
    int id;
    if (!PyArg_ParseTuple(args, "li", &ptr, &id)) {
        ALOGE("PyGlue:", "cannot get windget pointer");
        return Py_None;
    }

    mWidget * widget = (mWidget*)ptr;
    if (!widget) {
        return Py_None;
    }

    mWidget * found = find_window(widget->hwnd, id);

    if (found) return wrap_widget_object(found);
    return Py_None;
}


static PyMethodDef mg_methods[] = {
    {"log", mg_log, METH_VARARGS, "log the infomation"},
    {"CreateMainWindow", mg_createMainWindow, METH_VARARGS, "create the main window"},
    {"wrap", mg_wrap, METH_VARARGS, "wrap widget object"},
    {"MessageBox", mg_MessageBox, METH_VARARGS, "MessageBox"},
    {"DoModal", mg_DoModal, METH_VARARGS, "DoModal"},
    {"findWndObject", mg_findWndObject, METH_VARARGS, "find window object"},
    {NULL, NULL}
};

#define ADD_INT_CONSTEX(name, value) do { \
    PyObject* py_value = PyInt_FromLong(value); \
    PyObject* py_name = PyString_FromString(name); \
    PyObject_SetAttr(m, py_name, py_value); \
    Py_DECREF(py_value); \
    Py_DECREF(py_name); \
}while(0);

#define ADD_INT_CONST(name) ADD_INT_CONSTEX(#name, name)

static void init_constants(PyObject* m) {
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


static void initmg() {
    PyObject *m;
    PyObject* globals = NULL;
    PyObject* dict = NULL;
    globals = PyEval_GetGlobals();
    if (!globals) {
        ALOGE("PYTHON MINIGUI", "globals is null");
    }
    m = Py_InitModule3("mg", mg_methods, "minigui objects");
    if (m == NULL) {
        ALOGE("PYTHON MINIGUI","init python mg module failed");
        goto err;
    }
    init_constants(m);

    //add into globals
    if (globals) {
        dict = PyModule_GetDict(globals);
        if (PyDict_SetItemString(dict, "mg", m) < 0) {
            goto err;
        }
    }

    py_mg_module = m;
    Py_INCREF(py_mg_module);

err:
    if (globals) {
        Py_XDECREF(globals);
    }

}


extern "C" unsigned long RunPythonScript(const char* script_source, const char* filename) {
    //char* argv[3] = {"miniguipython", "-c", "print(\"hello\")"};
    //Py_Main(3, argv);
    unsigned long ret = 0;
    PyObject* py_code = NULL;
    mWidget*  widget = NULL;
    PyObject  *m, *d;
    Py_SetProgramName("minigui");

    Py_Initialize();

    initmg();

    //int ret = PyRun_SimpleString(source.c_str());
    m = PyImport_AddModule("__main__");
    if (m == NULL) {
        ALOGE("PYGLUE", "Create __main__ failed");
        return 0;
    }
    d = PyModule_GetDict(m);

    py_code = PyRun_String(script_source, Py_file_input, d, d);
    if (py_code == NULL) {
        ALOGE("PYGLUE", "Cannot execute:%s", filename);
        return 0;
    }

    widget = s_last_widget;

    if (widget == NULL) {
        ALOGE("PYGLUE", "The %s must return a validate widget object", filename);
        goto RETURN;
    }

    ret = (unsigned long)(widget->hwnd);

RETURN:
    if (py_code) {
        Py_DECREF(py_code);
    }

    return ret;

}
}

