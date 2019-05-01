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

// ---------------------------------------------------------------
PyObject*
CToPython(PyObject *self, PyObject *args)
{
	PyObject pp;
	Stz3Python* pd;
	int cc;
	const char *command;

	if(!PyArg_ParseTuple(args, "Oi", &pd, &cc))
	{
		return 0;
	}
	int a = pd->ToPython();

	return Py_BuildValue("i", a);
}


int
Stz3Python::ToPython()
{
	return 16;
}
// ---------------------------------------------------------------

PyObject*
CPPressPackageCommand(PyObject *self, PyObject *args)
{
	PyObject* pp;
	Stz3Python* pStz3Python;
	int commandID = 0;
	int button = 0;
	int result = 0;

	//if(!PyArg_ParseTuple(args, "Oii", &pStz3Python, &commandID, &button))
	if(!PyArg_ParseTuple(args, "Oii", &pp, &commandID, &button))
	{
		result = 0;
	}
	else
	{
		pStz3Python = (Stz3Python*)PyCObject_AsVoidPtr(pp);

		switch(commandID)
		{
			case QFW:
			{
				result = pStz3Python->CmdQFW(button);
			}
			break;
			case QBW:
			{
				result = pStz3Python->CmdQBW(button);
			}
			break;
			case HFW:
			{
				result = pStz3Python->CmdHFW(button);
			}
			break;
			case HBW:
			{
				result = pStz3Python->CmdHBW(button);
			}
			break;
			case FDFD:
			{
				result = pStz3Python->CmdFDFD(button);
			}
			break;
		}
	}

	return Py_BuildValue("i", result);
}


PyObject*
CPGetData(PyObject *self, PyObject *args)
{
	PyObject* pp;
	Stz3Python* pStz3Python;
	int commandID = 0;
	int objID = 0;
	int result = 0;

	if(!PyArg_ParseTuple(args, "Oii", &pp, &commandID, &objID))
	{
		result = 0;
	}
	else
	{
		pStz3Python = (Stz3Python*)PyCObject_AsVoidPtr(pp);

		switch(commandID)
		{
			case GET_ANIMATION:
			{
				result = pStz3Python->GetAnimation(objID);
			}
			break;
			case GET_POSX:
			{
				result = pStz3Python->GetPosX(objID);
			}
			break;
			case GET_POSY:
			{
				result = pStz3Python->GetPosY(objID);
			}
			break;
			case GET_GAUGE:
			{
				result = pStz3Python->GetGauge(objID);
			}
			break;
		}
	}

	return Py_BuildValue("i", result);
}


PyObject*
CPPressButtonNFrame(PyObject *self, PyObject *args)
{
	PyObject* pp;
	Stz3Python* pStz3Python;
	int result = 0;

	int n1 = 0;
	int n2 = 0;
	int n3 = 0;
	int button1 = 0;	
	int button2 = 0;
	int button3 = 0;	

	if(!PyArg_ParseTuple(args, "Oiiiiii", &pp, &button1, &n1
		                                     , &button2, &n2
											 , &button3, &n3))
	{
		result = 0;
	}
	else
	{
		pStz3Python = (Stz3Python*)PyCObject_AsVoidPtr(pp);
		result = pStz3Python->PressButtonNFrame(button1, n1,
												button2, n2,
												button3, n3);
	}
	return Py_BuildValue("i", result);
}


int
Stz3Python::CmdQFW(int button)
{	
	int i = 0;
	m_p1.m_keyBuffer[i] = PRESS_DOWN;                     i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN;                     i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_forward;    i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_forward;    i++;
	m_p1.m_keyBuffer[i] = m_p1.m_forward;                 i++;
	m_p1.m_keyBuffer[i] = m_p1.m_forward;                 i++;	
	m_p1.m_keyBuffer[i] = button;                         i++;
	m_p1.m_keyBuffer[i] = button;                         i++;

	m_p1.m_KBStock = i;
	m_p1.m_nextKBI = 0;
	m_p1.m_busy = true;
	return 0;
}


int
Stz3Python::CmdQBW(int button)
{
	int i = 0;
	m_p1.m_keyBuffer[i] = PRESS_DOWN;                       i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN;                       i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_backward;     i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_backward;     i++;
	m_p1.m_keyBuffer[i] = m_p1.m_backward;                  i++;
	m_p1.m_keyBuffer[i] = m_p1.m_backward;                  i++;
	m_p1.m_keyBuffer[i] = button;                           i++;
	m_p1.m_keyBuffer[i] = button;                           i++;

	m_p1.m_KBStock = i;
	m_p1.m_nextKBI = 0;
	m_p1.m_busy = true;
	return 0;
}


int
Stz3Python::CmdHFW(int button)
{	
	int i = 0;
	m_p1.m_keyBuffer[i] = m_p1.m_backward;                 i++;
	m_p1.m_keyBuffer[i] = m_p1.m_backward;                 i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_backward;    i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_backward;    i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN;                      i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN;                      i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_forward;     i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_forward;     i++;
	m_p1.m_keyBuffer[i] = m_p1.m_forward;                  i++;
	m_p1.m_keyBuffer[i] = m_p1.m_forward;                  i++;	
	m_p1.m_keyBuffer[i] = button;                          i++;
	m_p1.m_keyBuffer[i] = button;                          i++;

	m_p1.m_KBStock = i;
	m_p1.m_nextKBI = 0;
	m_p1.m_busy = true;

	return 0;
}


int
Stz3Python::CmdHBW(int button)
{
	int i = 0;
	m_p1.m_keyBuffer[i] = m_p1.m_forward;                  i++;
	m_p1.m_keyBuffer[i] = m_p1.m_forward;                  i++;	
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_forward;     i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_forward;     i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN;                      i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN;                      i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_backward;    i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_backward;    i++;
	m_p1.m_keyBuffer[i] = m_p1.m_backward;                 i++;
	m_p1.m_keyBuffer[i] = m_p1.m_backward;                 i++;
	m_p1.m_keyBuffer[i] = button;                          i++;
	m_p1.m_keyBuffer[i] = button;                          i++;

	m_p1.m_KBStock = i;
	m_p1.m_nextKBI = 0;
	m_p1.m_busy = true;

	return 0;
}


int
Stz3Python::CmdFDFD(int button)
{
	int i = 0;
	m_p1.m_keyBuffer[i] = m_p1.m_forward;                   i++;
	m_p1.m_keyBuffer[i] = m_p1.m_forward;                   i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN;                       i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN;                       i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_forward;      i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_forward;      i++;
	m_p1.m_keyBuffer[i] = button;                           i++;
	m_p1.m_keyBuffer[i] = button;                           i++;

	m_p1.m_KBStock = i;
	m_p1.m_nextKBI = 0;
	m_p1.m_busy = true;
	return m_p1.m_forward;
}


int
Stz3Python::PressButtonNFrame(short button1, short nFrame1,
							  short button2, short nFrame2,
						      short button3, short nFrame3)
{
	int i = 0;

	for (int j = 0; j <= nFrame1; j++)
	{
		m_p1.m_keyBuffer[i] = button1;    i++;
	}
	for (int j = 0; j <= nFrame2; j++)
	{
		m_p1.m_keyBuffer[i] = button2;    i++;
	}
	for (int j = 0; j <= nFrame3; j++)
	{
		m_p1.m_keyBuffer[i] = button3;    i++;
	}

	m_p1.m_KBStock = i;
	m_p1.m_nextKBI = 0;
	m_p1.m_busy = true;

	return button1;
}


int
Stz3Python::GetAnimation(int objID)
{
	if (objID == STZ3_P1)
	{
		return m_p1Ani;
	}
	else
	{
		return m_p2Ani;
	}
	return 0;
}


int
Stz3Python::GetPosX(int objID)
{
	if (objID == STZ3_P1)
	{
		return m_p1.m_posX;
	}
	else if (objID == STZ3_P2)
	{
		return m_p2.m_posX;
	}
	else if (objID == STZ3_WP1)
	{
		return m_p1.m_wPosX;
	}
	else //(objID == STZ3_WP2)
	{
		return m_p2.m_wPosX;
	}
	return 0;
}


int
Stz3Python::GetPosY(int objID)
{
	if (objID == STZ3_P1)
	{
		return m_p1.m_posY;
	}
	else if (objID == STZ3_P2)
	{
		return m_p2.m_posY;
	}
	else if (objID == STZ3_WP1)
	{
		return m_p1.m_wPosY;
	}
	else //(objID == STZ3_WP2)
	{
		return m_p2.m_wPosY;
	}
	return 0;
}


int
Stz3Python::GetGauge(int barID)
{
	if (barID == STZ3_HP_BAR_P1)
	{
		return 0; // Under construction
	}
	else if (barID == STZ3_ISM_BAR_P1)
	{
		return m_p1.m_ismBar;
	}
	else if(barID == STZ3_GUARD_BAR_P1)
	{
		return 0; // Under construction
	}

	else if (barID == STZ3_HP_BAR_P2)
	{
		return 0; // Under construction
	}
	else if (barID == STZ3_ISM_BAR_P2)
	{
		return m_p2.m_ismBar;
	}
	else //if(barID == STZ3_GUARD_BAR_P2)
	{
		return 0; // Under construction
	}
	return 0;
}


int
Stz3Python::InitialStz3()
{
	if (theApp.vbaLinkID > 2) theApp.vbaLinkID = 2;
	if (theApp.vbaLinkID < 1) theApp.vbaLinkID = 1;
	
	int rivalID = 2;
	if (theApp.vbaLinkID == 2) rivalID = 1;

	m_p1.SetGameStateAddress(theApp.vbaLinkID);
	m_p2.SetGameStateAddress(rivalID);
	m_p1.m_pRival = &m_p2;
	m_p2.m_pRival = &m_p1;

	srand((unsigned)time(NULL));

	return MY_SUCCESS;
}


int
Stz3Python::InitInterfaceFunction()
{
	/*
	m_MyMethods.ml_name = "fromC";
	m_MyMethods.ml_meth = CToPython;
	m_MyMethods.ml_flags = METH_VARARGS;
	m_MyMethods.ml_doc = "Call from C.";
	Py_InitModule("emb", &m_MyMethods);
	*/

	m_stz3Method[0].ml_name = "CPPressPackageCommand";
	m_stz3Method[0].ml_meth = CPPressPackageCommand;
	m_stz3Method[0].ml_flags = METH_VARARGS;
	m_stz3Method[0].ml_doc = "Call from C.";

	m_stz3Method[1].ml_name = "CPGetData";
	m_stz3Method[1].ml_meth = CPGetData;
	m_stz3Method[1].ml_flags = METH_VARARGS;
	m_stz3Method[1].ml_doc = "Call from C.";

	m_stz3Method[2].ml_name = "CPPressButtonNFrame";
	m_stz3Method[2].ml_meth = CPPressButtonNFrame;
	m_stz3Method[2].ml_flags = METH_VARARGS;
	m_stz3Method[2].ml_doc = "Call from C.";
	
	Py_InitModule("stz3", m_stz3Method);

	return 0;
}


int
Stz3Python::Initial()
{
	InitialStz3();
    Py_Initialize();	
	InitInterfaceFunction();

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

/*
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
*/


int
Stz3Python::RunFrameRoutine1()
{
	m_p1.UpdateGameStateData();
	m_p2.UpdateGameStateData();
	m_p1.UpdateGameCalData();
	m_p2.UpdateGameCalData();

	m_p1Ani = m_p1.m_animation - m_p1.m_aniOffset;
	m_p2Ani = m_p2.m_animation - m_p2.m_aniOffset;

	return 0;
}


int
Stz3Python::CheckKeyBuffer()
{
	int result = m_p1.m_keyBuffer[m_p1.m_nextKBI];
	m_p1.m_nextKBI++;
	m_p1.m_KBStock--;
	if (m_p1.m_KBStock <= 0)
	{
		m_p1.m_busy = !m_p1.m_busy;
	}
	return result;
}


int
Stz3Python::RunPythonScript()
{
	int result = 0;

	RunFrameRoutine1();
	m_frameNo++;

	if (m_p1.m_posX < 10) return result; // If not in battle
	if (m_p1.m_busy) return CheckKeyBuffer(); // Check is there have a keybuffer to continuely execute from previoue round.

	PyObject* pArgs2 = PyTuple_New(1);
	//m_pValue = PyLong_FromVoidPtr(this);	
	m_pValue = PyCObject_FromVoidPtr(this, NULL);
	Stz3Python* aaa = (Stz3Python*)PyCObject_AsVoidPtr(m_pValue);

	PyTuple_SetItem(pArgs2, 0, m_pValue);

	m_pFunc = PyDict_GetItemString(m_pDict, "Main");
	if (PyCallable_Check(m_pFunc))
    {
		m_pValue = PyObject_CallObject(m_pFunc, pArgs2);
		result = PyInt_AsLong(m_pValue);
	}
	if ((result < 0) || (result >= 1024))
	{
		result = 0;
	}

	return result;
}