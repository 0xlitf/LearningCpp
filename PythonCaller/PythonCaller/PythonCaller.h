#pragma once

#include <Python.h>
#include <iostream>

class PythonCaller {
public:
    PythonCaller(const char* pyfile = nullptr);
    ~PythonCaller(void);
    void setArgc(const int& n);
    void feedArgv(const int& i, const int& v);
    void executePythonFunction(const char* pyfunction = nullptr);
private:
    PyObject* m_moduleName;
    PyObject* m_module;
    PyObject* m_function;
    PyObject* m_args;
    PyObject** m_argv;
    PyObject* m_ret;
    int m_argsCount;
};
