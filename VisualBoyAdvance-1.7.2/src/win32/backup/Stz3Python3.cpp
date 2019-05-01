#include "stdafx.h"
#include "Stz3Python.h"
#include <time.h>

#undef WANT_DEBUG_PYTHON

Stz3Python::Stz3Python()
{
	m_pName = 0;
	m_pModule = 0;
	m_pDict = 0;
	m_pFunc = 0;
	m_pValue = 0;	
}


Stz3Python::~Stz3Python()
{
	Destroy();
}


PyObject*
CToPython(PyObject *self, PyObject *args)    
{
	PyObject pp;
	Stz3Python* pd;
	const char *command;

	if(!PyArg_ParseTuple(args, "O", &pd)) return NULL;	
	int a = pd->ToPython();

	return Py_BuildValue("i", a);
}


int
Stz3Python::ToPython()
{
	return 16;
}


int
Stz3Python::Initial()
{
    Py_Initialize();

	PyMethodDef MyMethods[] = 
	{
		{"fromC", CToPython, METH_VARARGS, "Call from C."}, {NULL, NULL, 0, NULL}
	};
	Py_InitModule("emb", MyMethods);

	m_pName = PyString_FromString("mypyScript");
    m_pModule = PyImport_Import(m_pName);
    m_pDict = PyModule_GetDict(m_pModule);
	m_pFunc = PyDict_GetItemString(m_pDict, "walk");

	if (PyCallable_Check(m_pFunc)) 
    {
		// OK !!!!
    }
	else 
    {
        PyErr_Print();
    }	
	return 0;
}


int
Stz3Python::Destroy()
{
	// Clean up
    Py_DECREF(m_pModule);
    Py_DECREF(m_pName);

    // Finish the Python Interpreter
    Py_Finalize();

	return 0;
}


int
Stz3Python::RunPythonScript()
{
	int result = 0;

	PyMethodDef MyMethods[] = 
	{
		{"fromC", CToPython, METH_VARARGS, "Call from C."}, {NULL, NULL, 0, NULL}
	};
	Py_InitModule("emb", MyMethods);

	PyObject* pArgs2 = PyTuple_New(1);
	m_pValue = PyLong_FromVoidPtr(this);
	PyTuple_SetItem(pArgs2, 0, m_pValue);

	m_pFunc = PyDict_GetItemString(m_pDict, "walk2");
	if (PyCallable_Check(m_pFunc))
    {
		m_pValue = PyObject_CallObject(m_pFunc, pArgs2);
		result = PyInt_AsLong(m_pValue);
	}

	return result;
}