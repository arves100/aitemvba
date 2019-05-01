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

// -----------------------------------------------------------------------------
// This is simple interface function, use to test is py can connect with VC
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


// This is simple interface function, use to test is py can connect with VC
int
Stz3Python::ToPython()
{
	return 16;
}
// -----------------------------------------------------------------------------


// These are real interface function that use in AI-TEM ------------------------
PyObject*
CPPressPackageCommand(PyObject *self, PyObject *args)
{
	PyObject* pp;
	Stz3Python* pStz3Python;
	int commandID = 0;
	int button = 0;
	int result = 0;

	if(!PyArg_ParseTuple(args, "Oii", &pp, &commandID, &button))
	{
		result = -1;
	}
	else
	{
		pStz3Python = (Stz3Python*)PyCObject_AsVoidPtr(pp);

		switch(commandID)
		{
			case P_QCF:
			{
				result = pStz3Python->QCF(button);
			}
			break;
			case P_QCB:
			{
				result = pStz3Python->QCB(button);
			}
			break;
			case P_HCF:
			{
				result = pStz3Python->HCF(button);
			}
			break;
			case P_HCB:
			{
				result = pStz3Python->HCB(button);
			}
			break;
			case P_DPF:
			{
				result = pStz3Python->DPF(button);
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
		result = -1;
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
			case GET_DAMAGE:
			{
				result = pStz3Python->GetDamage(objID);
			}
			break;
			case GET_FRAME:
			{
				result = pStz3Python->GetFrameCounter();
			}
			break;
		}
	}

	return Py_BuildValue("i", result);
}


PyObject*
CPPressCustomCombo(PyObject *self, PyObject *args)
{
	PyObject* pp;
	PyObject* pArgs;
	PyObject* tuple;
	Stz3Python* pStz3Python;
	int result = 0;
	int nButton = 0;

	int nFrame[128]; // Should enought
	int button[128];


	if(!PyArg_ParseTuple(args, "OO", &pp, &pArgs))
	{
		result = -1;
	}
	else
	{
		pStz3Python = (Stz3Python*)PyCObject_AsVoidPtr(pp);

		nButton = PyTuple_Size(pArgs);

		for (int i = 0; i < nButton; i++)
		{
			tuple = PyTuple_GetItem(pArgs, i);
			if(!PyArg_ParseTuple(tuple, "ii", &button[i], &nFrame[i]))
			{
				int a = 0;
			}
		}

		result = pStz3Python->PressCustomCombo(nButton, button, nFrame);
	}
	return Py_BuildValue("i", result);
}


// However this function call thaApp that dont need to use Stz3Python*
// We still need it to be parameter for the... proper, manner, standardize code
PyObject*
CPLoadState(PyObject *self, PyObject *args)
{
	PyObject* pp;
	Stz3Python* pStz3Python;
	int result = 0;
	const char *s;

	if(!PyArg_ParseTuple(args, "Os", &pp, &s))
	{
		result = -1;
	}
	else
	{
		//pStz3Python...
		result = theApp.emulator.emuReadState(s);
	}

	return Py_BuildValue("i", result);
}
// -----------------------------------------------------------------------------


int
Stz3Python::QCF(int button)
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
Stz3Python::QCB(int button)
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
Stz3Python::HCF(int button)
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
Stz3Python::HCB(int button)
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
Stz3Python::DPF(int button)
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
Stz3Python::PressCustomCombo(int nButton, int* pButton, int* pNFrame)
{
	int i = 0;

	pNFrame[0] = pNFrame[0] - 1; // immediate return in this fn
	for (int n = 0; n < nButton; n++)
	{
		for (int j = 0; j < pNFrame[n]; j++) //... not good, should memset...
		{
			m_p1.m_keyBuffer[i] = pButton[n];    i++;
		}
	}

	m_p1.m_KBStock = i;
	m_p1.m_nextKBI = 0;
	m_p1.m_busy = true;

	return pButton[0]; //!!!!
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
Stz3Python::GetDamage(int objID)
{
	if (objID == STZ3_P1)
	{
		return m_p1.m_damage;
	}
	else if (objID == STZ3_P2)
	{
		return m_p2.m_damage;
	}
	return 0;
}


int
Stz3Python::GetFrameCounter()
{
	return theApp.pAIScript->m_autoBattleStateF;
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

	LoadCharacterOffset();

	return MY_SUCCESS;
}


int
Stz3Python::initstz3()
{
	m_stz3Method[0].ml_name = "CPPressPackageCommand";
	m_stz3Method[0].ml_meth  = CPPressPackageCommand;
	m_stz3Method[0].ml_flags = METH_VARARGS;
	m_stz3Method[0].ml_doc   = "Call from C.";

	m_stz3Method[1].ml_name = "CPGetData";
	m_stz3Method[1].ml_meth  = CPGetData;
	m_stz3Method[1].ml_flags = METH_VARARGS;
	m_stz3Method[1].ml_doc   = "Call from C.";

	m_stz3Method[2].ml_name = "CPPressCustomCombo";
	m_stz3Method[2].ml_meth  = CPPressCustomCombo;
	m_stz3Method[2].ml_flags = METH_VARARGS;
	m_stz3Method[2].ml_doc   = "Call from C.";

	m_stz3Method[3].ml_name = "CPLoadState";
	m_stz3Method[3].ml_meth  = CPLoadState;
	m_stz3Method[3].ml_flags = METH_VARARGS;
	m_stz3Method[3].ml_doc   = "Call from C.";
	
	PyObject* m;
	m = Py_InitModule("stz3", m_stz3Method);

	return 0;
}


int
Stz3Python::Initial()
{
	InitialStz3();
    Py_Initialize();
	initstz3();
/*
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
*/
	m_pName = PyString_FromString("mypyScript");
    m_pModule = PyImport_Import(m_pName);
	if (!m_pModule)
	{
		PyObject* oo = PyErr_Occurred();
		if (oo)
		{

PyObject *ptype, *pvalue, *ptraceback;
PyErr_Fetch(&ptype, &pvalue, &ptraceback);
//pvalue contains error message
//ptraceback contains stack snapshot and many other information
//(see python traceback structure)

//Get error message
char *pStrErrorMessage = PyString_AsString(pvalue);
MessageBoxA(NULL, pStrErrorMessage, "ERR!", MB_OK | MB_ICONERROR);
		}
	}
	m_pDict = PyModule_GetDict(m_pModule);
	m_pFunc = PyDict_GetItemString(m_pDict, "Redirection");

	if (PyCallable_Check(m_pFunc)) 
    {
		// OK !!!!
		// Call Redirection to initial, bypass python log, stderr, stdout, etc... to txt file.
		PyObject_CallObject(m_pFunc, NULL);
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
Stz3Python::RunFrameRoutine()
{
	m_p1.UpdateGameStateData();
	m_p2.UpdateGameStateData();
	m_p1.UpdateGameCalData();
	m_p2.UpdateGameCalData();

	m_p1Ani = m_p1.m_animation - m_p1.m_refAnimation + m_characterArrayOffsetType2[m_p1.m_charID];
	m_p2Ani = m_p2.m_animation - m_p2.m_refAnimation + m_characterArrayOffsetType2[m_p2.m_charID];

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

unsigned char Stz3Python::ProcessLink(unsigned char in)
{
	if (!m_pDict)
	{
		systemScreenMessage("Python script not loaded");
		return 0xFF;
	}

	PyObject* m_linkFunc = PyDict_GetItemString(m_pDict, "Link");
	if (PyCallable_Check(m_linkFunc))
    {
		PyObject* linkObj = PyEval_CallObject(m_linkFunc, Py_BuildValue("(i)", in));
		return PyInt_AsLong(linkObj);
	}
	
	return 0xFF;
}

int
Stz3Python::RunPythonScript()
{
	int result = 0;

	RunFrameRoutine();
	m_frameNo++;

	if (m_p1.m_posX < 10) return result;      // If not in battle
	if (m_p1.m_busy) return CheckKeyBuffer(); // Check is there have a keybuffer to continuely execute from previoue round.

	PyObject* pArgs2 = PyTuple_New(1);
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
		result = -1;
	}

	return result;
}


// Load Character offset and maxanimation use for know the character animation
int
Stz3Python::LoadCharacterOffset()
{
	// Initial array
	for (int i = 0; i < N_CHAR; i++)
	{
		m_characterArrayOffset[i] = 0;
		m_characterArrayOffsetType2[i] = 0;
		m_characterArrayMaxAni[i] = 0;
		m_characterArrayMaxAniType2[i] = 0;
	}

	// ---- Loading character offset, use for calculating character animation
	// ---- Type1 is order by character slot in select char in training mode left to right, up to down
	m_characterArrayOffset[0]     = 20620; //  0 RYU
	m_characterArrayOffset[1]     = 22032; // 24 EHONDA
	m_characterArrayOffset[2]     = 28520; // 25 BLANKA
	m_characterArrayOffset[3]     = 27672; //  4 CHUNLI
	m_characterArrayOffset[4]     = 23680; // 16 ZANGIEF
	m_characterArrayOffset[5]     = 33484; // 15 DHALSIM
	m_characterArrayOffset[6]     = 23336; //  1 KEN

	m_characterArrayOffset[7]     = 26020; //  2 GOKI
	m_characterArrayOffset[8]     = 23452; //  3 NASH
	m_characterArrayOffset[9]     = 29304; // 22 CAMMY
	m_characterArrayOffset[10]    = 20648; // 37 MBISON
	m_characterArrayOffset[11]    = 30044; // 28 BALOG
	m_characterArrayOffset[12]    = 28108; // 11 SAGAT
	m_characterArrayOffset[13]    = 22940; // 10 VEGA

	m_characterArrayOffset[14]    = 21684; //  5 ADON
	m_characterArrayOffset[15]    = 39128; // 17 GEN
  //m_characterArrayOffset[15]    = 22172; // 19 GEN FORM B !!!!
	m_characterArrayOffset[16]    = 22876; //  8 BIRDIE
	m_characterArrayOffset[17]    = 28572; // 27 CODY
	m_characterArrayOffset[18]    = 23832; //  7 GUY
	m_characterArrayOffset[19]    = 27748; // 14 ROLENTO
	m_characterArrayOffset[20]    = -5616; //  6 SODOM

	m_characterArrayOffset[21]    = 26680; //  9 ROSE
	m_characterArrayOffset[22]    = 26776; // 13 SAKURA
	m_characterArrayOffset[23]    = 26444; // 29 KAREN
	m_characterArrayOffset[24]    = 20288; // 12 DAN
	m_characterArrayOffset[25]    = 26400; // 26 MIKA
	m_characterArrayOffset[26]    = 24396; // 30 JULI
	m_characterArrayOffset[27]    = 25816; // 31 JUNI

	// RANDOM P1 SLOT
	m_characterArrayOffset[29]    = 23220; // 23 EVILRYU
	m_characterArrayOffset[30]    = 23868; // 33 FEILONG
	m_characterArrayOffset[31]    = 22668; // 34 DJ
	m_characterArrayOffset[32]    = 19032; // 35 THAWK
	m_characterArrayOffset[33]    = 21348; // 32 GUILE
	// RANDOM P2 SLOT

	// BLANK
	// BLANK
	m_characterArrayOffset[37]    = 24724; // 39 MAKI
	m_characterArrayOffset[38]    = 23484; // 40 YUN
	m_characterArrayOffset[39]    = 27396; // 38 EAGLE
	// BLANK
	// BLANK


	// ---- Loading character offset, use for calculating character animation
	// ---- Type2 is order by character ID that the STZ3 game had assigned
	m_characterArrayOffsetType2[C_RYU]     = 20620; //  0 RYU
	m_characterArrayOffsetType2[C_KEN]     = 23336; //  1 KEN
	m_characterArrayOffsetType2[C_GOKI]    = 26020; //  2 GOKI
	m_characterArrayOffsetType2[C_NASH]    = 23452; //  3 NASH
	m_characterArrayOffsetType2[C_CHUNLI]  = 27672; //  4 CHUNLI
	m_characterArrayOffsetType2[C_ADON]    = 21684; //  5 ADON
	m_characterArrayOffsetType2[C_SODOM]   = -5616; //  6 SODOM
	m_characterArrayOffsetType2[C_GUY]     = 23832; //  7 GUY
	m_characterArrayOffsetType2[C_BIRDIE]  = 22876; //  8 BIRDIE
	m_characterArrayOffsetType2[C_ROSE]    = 26680; //  9 ROSE
	m_characterArrayOffsetType2[C_VEGA]    = 22940; // 10 VEGA
	m_characterArrayOffsetType2[C_SAGAT]   = 28108; // 11 SAGAT
	m_characterArrayOffsetType2[C_DAN]     = 20288; // 12 DAN
	m_characterArrayOffsetType2[C_SAKURA]  = 26776; // 13 SAKURA
	m_characterArrayOffsetType2[C_ROLENTO] = 27748; // 14 ROLENTO
	m_characterArrayOffsetType2[C_DHALSIM] = 33484; // 15 DHALSIM
	m_characterArrayOffsetType2[C_ZANGIEF] = 23680; // 16 ZANGIEF 
	m_characterArrayOffsetType2[C_GEN]     = 39128; // 17 GEN

    m_characterArrayOffsetType2[C_GEN2]    = 22172; // 19 GEN FORM B

	m_characterArrayOffsetType2[C_CAMMY]   = 29304; // 22 CAMMY
	m_characterArrayOffsetType2[C_EVILRYU] = 23220; // 23 EVILRYU
	m_characterArrayOffsetType2[C_EHONDA]  = 22032; // 24 EHONDA
	m_characterArrayOffsetType2[C_BLANKA]  = 28520; // 25 BLANKA 
	m_characterArrayOffsetType2[C_MIKA]    = 26400; // 26 MIKA
	m_characterArrayOffsetType2[C_CODY]    = 28572; // 27 CODY
	m_characterArrayOffsetType2[C_BALOG]   = 30044; // 28 BALOG
	m_characterArrayOffsetType2[C_KARIN]   = 26444; // 29 KAREN
	m_characterArrayOffsetType2[C_JULI]    = 24396; // 30 JULI
	m_characterArrayOffsetType2[C_JUNI]    = 25816; // 31 JUNI
	m_characterArrayOffsetType2[C_GUILE]   = 21348; // 32 GUILE
	m_characterArrayOffsetType2[C_FEILONG] = 23868; // 33 FEILONG
	m_characterArrayOffsetType2[C_DJ]      = 22668; // 34 DJ
	m_characterArrayOffsetType2[C_THAWK]   = 19032; // 35 THAWK
                                              // 36 SHINGOKI
	m_characterArrayOffsetType2[C_MBISON]  = 20648; // 37 MBISON	
	m_characterArrayOffsetType2[C_EAGLE]   = 27396; // 38 EAGLE
	m_characterArrayOffsetType2[C_MAKI]    = 24724; // 39 MAKI
	m_characterArrayOffsetType2[C_YUN]     = 23484; // 40 YUN
	

	//----------------------------------------------------------------------------------
	// ---- Loading character maxanimation value, use for checking something... not important
	// ---- Type1 is order by character slot in select char in training mode left to right, up to down
    // Max animation after normalize
	m_characterArrayMaxAni[0]     = 11112; //  0 RYU
	m_characterArrayMaxAni[1]     = 10788; // 24 EHONDA
	m_characterArrayMaxAni[2]     = 13200; // 25 BLANKA
	m_characterArrayMaxAni[3]     = 14772; //  4 CHUNLI
	m_characterArrayMaxAni[4]     = 11796; // 16 ZANGIEF
	m_characterArrayMaxAni[5]     = 14016; // 15 DHALSIM
	m_characterArrayMaxAni[6]     = 11088; //  1 KEN

	m_characterArrayMaxAni[7]     = 12240; //  2 GOKI
	m_characterArrayMaxAni[8]     = 11580; //  3 NASH
	m_characterArrayMaxAni[9]     = 13560; // 22 CAMMY
	m_characterArrayMaxAni[10]    = 11352; // 37 MBISON
	m_characterArrayMaxAni[11]    = 12084; // 28 BALOG
	m_characterArrayMaxAni[12]    = 12444; // 11 SAGAT
	m_characterArrayMaxAni[13]    = 10488; // 10 VEGA **** Note begin at -1392 not 0

	m_characterArrayMaxAni[14]    = 11664; //  5 ADON
	m_characterArrayMaxAni[15]    = 20712; // 17 GEN !!!!
  //m_characterArrayMaxAni[15]    =  3756; // 19 GEN FORM B !!!!
	m_characterArrayMaxAni[16]    = 11856; //  8 BIRDIE
	m_characterArrayMaxAni[17]    = 12288; // 27 CODY
	m_characterArrayMaxAni[18]    = 12456; //  7 GUY
	m_characterArrayMaxAni[19]    = 13776; // 14 ROLENTO
	m_characterArrayMaxAni[20]    =  9432; //  6 SODOM

	m_characterArrayMaxAni[21]    = 12492; //  9 ROSE
	m_characterArrayMaxAni[22]    = 13656; // 13 SAKURA
	m_characterArrayMaxAni[23]    = 14700; // 29 KAREN
	m_characterArrayMaxAni[24]    =  9900; // 12 DAN
	m_characterArrayMaxAni[25]    = 14808; // 26 MIKA
	m_characterArrayMaxAni[26]    = 11280; // 30 JULI
	m_characterArrayMaxAni[27]    = 12000; // 31 JUNI

	// RANDOM P1 SLOT
	m_characterArrayMaxAni[29]    = 12432; // 23 EVILRYU
	m_characterArrayMaxAni[30]    = 11976; // 33 FEILONG
	m_characterArrayMaxAni[31]    = 10596; // 34 DJ
	m_characterArrayMaxAni[32]    =  8616; // 35 THAWK
	m_characterArrayMaxAni[33]    =  9996; // 32 GUILE
	// RANDOM P2 SLOT

	// BLANK
	// BLANK
	m_characterArrayMaxAni[37]    = 14940; // 39 MAKI
	m_characterArrayMaxAni[38]    = 14928; // 40 YUN
	m_characterArrayMaxAni[39]    = 15192; // 38 EAGLE
	// BLANK
	// BLANK


	// ---- Loading character maxanimation value, use for checking something... not important
	// ---- Type2 is order by character ID that the STZ3 game had assigned
	m_characterArrayMaxAniType2[C_RYU]     = 11112 ; //  0 RYU
	m_characterArrayMaxAniType2[C_KEN]     = 11088 ; //  1 KEN
	m_characterArrayMaxAniType2[C_GOKI]    = 12240 ; //  2 GOKI
	m_characterArrayMaxAniType2[C_NASH]    = 11580 ; //  3 NASH
	m_characterArrayMaxAniType2[C_CHUNLI]  = 14772 ; //  4 CHUNLI
	m_characterArrayMaxAniType2[C_ADON]    = 11664 ; //  5 ADON
	m_characterArrayMaxAniType2[C_SODOM]   =  9432 ; //  6 SODOM
	m_characterArrayMaxAniType2[C_GUY]     = 12456 ; //  7 GUY
	m_characterArrayMaxAniType2[C_BIRDIE]  = 11856 ; //  8 BIRDIE
	m_characterArrayMaxAniType2[C_ROSE]    = 12492 ; //  9 ROSE
	m_characterArrayMaxAniType2[C_VEGA]    = 10488 ; // 10 VEGA **** Note begin at -1392 not 0
	m_characterArrayMaxAniType2[C_SAGAT]   = 12444 ; // 11 SAGAT
	m_characterArrayMaxAniType2[C_DAN]     =  9900 ; // 12 DAN
	m_characterArrayMaxAniType2[C_SAKURA]  = 13656 ; // 13 SAKURA
	m_characterArrayMaxAniType2[C_ROLENTO] = 13776 ; // 14 ROLENTO
	m_characterArrayMaxAniType2[C_DHALSIM] = 14016 ; // 15 DHALSIM
	m_characterArrayMaxAniType2[C_ZANGIEF] = 11796 ; // 16 ZANGIEF 
	m_characterArrayMaxAniType2[C_GEN]     = 20712 ; // 17 GEN !!!!

    m_characterArrayMaxAniType2[C_GEN2]    =  3756 ; // 19 GEN FORM B !!!!

	m_characterArrayMaxAniType2[C_CAMMY]   = 13560 ; // 22 CAMMY
	m_characterArrayMaxAniType2[C_EVILRYU] = 12432 ; // 23 EVILRYU
	m_characterArrayMaxAniType2[C_EHONDA]  = 10788 ; // 24 EHONDA
	m_characterArrayMaxAniType2[C_BLANKA]  = 13200 ; // 25 BLANKA 
	m_characterArrayMaxAniType2[C_MIKA]    = 14808 ; // 26 MIKA
	m_characterArrayMaxAniType2[C_CODY]    = 12288 ; // 27 CODY
	m_characterArrayMaxAniType2[C_BALOG]   = 12084 ; // 28 BALOG
	m_characterArrayMaxAniType2[C_KARIN]   = 14700 ; // 29 KAREN
	m_characterArrayMaxAniType2[C_JULI]    = 11280 ; // 30 JULI
	m_characterArrayMaxAniType2[C_JUNI]    = 12000 ; // 31 JUNI
	m_characterArrayMaxAniType2[C_GUILE]   =  9996 ; // 32 GUILE
	m_characterArrayMaxAniType2[C_FEILONG] = 11976 ; // 33 FEILONG
	m_characterArrayMaxAniType2[C_DJ]      = 10596 ; // 34 DJ
	m_characterArrayMaxAniType2[C_THAWK]   =  8616 ; // 35 THAWK
												     // 36 SHINGOKI
	m_characterArrayMaxAniType2[C_MBISON]  = 11352 ; // 37 MBISON
	m_characterArrayMaxAniType2[C_EAGLE]   = 15192 ; // 38 EAGLE
	m_characterArrayMaxAniType2[C_MAKI]    = 14940 ; // 39 MAKI
	m_characterArrayMaxAniType2[C_YUN]     = 14928 ; // 40 YUN

	return 0;
}