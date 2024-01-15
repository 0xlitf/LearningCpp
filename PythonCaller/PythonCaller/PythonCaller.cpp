#include "PythonCaller.h"
#include <QDebug>

PythonCaller::PythonCaller(const char* pyfile) {
    Py_Initialize();
    m_moduleName = PyUnicode_DecodeFSDefault(pyfile);
    m_module = PyImport_Import(m_moduleName);
    if (m_module) {
        qInfo("[Info] Python get module succeed!");
    } else {
        qFatal("Python get module failed!");
    }
}

PythonCaller::~PythonCaller(void) {
    Py_Finalize();
    free(m_argv);
}

void PythonCaller::setArgc(const int& n) {
    m_argsCount = n;
    m_args = PyTuple_New(m_argsCount);
    m_argv = (PyObject**)calloc(m_argsCount, sizeof(PyObject*));
}

void PythonCaller::feedArgv(const int& i, const int& v) {
    m_argv[i] = PyLong_FromLong(v);
    PyTuple_SetItem(m_args, i, m_argv[i]);
}

void PythonCaller::executePythonFunction(const char* pyfunction) {
    m_function = PyObject_GetAttrString(m_module, pyfunction);
    m_ret = PyObject_CallObject(m_function, m_args);
    if (m_ret) {
        long result = PyLong_AsLong(m_ret);
        qInfo("result: %ld", result);
    }
}
