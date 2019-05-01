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
Stz3Python::CmdQFW(int button)
{
	return 0;
}


int
Stz3Python::CmdQBW(int button)
{
	return 0;
}


int
Stz3Python::CmdHFW(int button)
{
	return 0;
}


int
Stz3Python::CmdHBW(int button)
{
	return 0;
}


int
Stz3Python::CmdFDFD(int button)
{
	return 0;
}


int
Stz3Python::PressButtonNFrame(short button1, short nFrame1,
							  short button2, short nFrame2,
						      short button3, short nFrame3)
{
	return 0;
}


int
Stz3Python::GetPosX(int objID)
{
	return 0;
}


int
Stz3Python::GetPosY(int objID)
{
	return 0;
}


int
Stz3Python::GetGauge(int barID)
{
	return 0;
}


int
Stz3Python::Initial()
{
    Py_Initialize();

	m_MyMethods.ml_name = "fromC";
	m_MyMethods.ml_meth = CToPython;
	m_MyMethods.ml_flags = METH_VARARGS;
	m_MyMethods.ml_doc = "Call from C.";
	Py_InitModule("emb", &m_MyMethods);

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