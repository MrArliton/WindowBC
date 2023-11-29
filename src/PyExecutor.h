#include <Python.h>
#include <optional>
#include <string>
#include <vector>
#include <queue>
#include <iostream>

// ----- For run the python scripts and get results from them. -----


namespace pyexec
{
// --- Contains external PyObject*'s ---
static std::queue<PyObject *> listOfExternalObj;

// --- Initialize python Interpreter --- 
void pyInit()
{
    Py_Initialize();
}

std::optional<PyObject*> pyExecute(std::string pNameS,std::string fNameS,std::vector<PyObject*> args)
{
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    

    pName = PyUnicode_DecodeFSDefault(pNameS.c_str());

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, fNameS.c_str());

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(args.size());
            for (int i = 0; i < args.size(); ++i) {
                pValue = args[i];
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return std::nullopt;
                }

                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            
            if (pValue != NULL) {
                return pValue;
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return std::nullopt;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", fNameS);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", pNameS);
        return std::nullopt;
    }
    return std::nullopt;
}

// --- Free memory for all returned PyObject* ---
void pyFreeResources()
{
    while(!listOfExternalObj.empty())
    {
        Py_DECREF(listOfExternalObj.front());
        listOfExternalObj.pop();
    }
}
// --- Stop python Interpreter ---
int pyFinalize()
{
    if (Py_FinalizeEx() < 0) {
        return 120;
    }
    return 0;
}
} 