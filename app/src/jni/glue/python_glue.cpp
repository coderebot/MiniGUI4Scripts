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

static PyObject * mg_log(PyObject* self, PyObject* args) {
    char* info = NULL;
    if (!PyArg_ParseTuple(args, "s", &info)) {
        return NULL;
    }

    ALOGI("MGLOG", "%s", info);
    return NULL;
}


static PyMethodDef mg_methods[] = {
    {"log", mg_log, METH_VARARGS, "log the infomation"},
    {NULL, NULL}
};


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

    //add into globals
    if (globals) {
        dict = PyModule_GetDict(globals);
        if (PyDict_SetItemString(dict, "mg", m) < 0) {
            goto err;
        }
    }

err:
    if (globals) {
        Py_XDECREF(globals);
    }

}


extern "C" unsigned long RunPythonScript(const char* script_source, const char* filename) {
    //char* argv[3] = {"miniguipython", "-c", "print(\"hello\")"};
    //Py_Main(3, argv);
    Py_SetProgramName("minigui");

    Py_Initialize();

    initmg();

    string source = "import mg\n";
    source += script_source;

    int ret = PyRun_SimpleString(source.c_str());

    return 0;

}

