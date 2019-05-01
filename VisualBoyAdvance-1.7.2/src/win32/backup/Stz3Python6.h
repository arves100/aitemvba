#if !defined(WPJ_STZ3Python)
#define WPJ_STZ3Python

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "Stz3.h"

//# include <python.h>
#ifdef _DEBUG
  #undef _DEBUG
  #include <python.h>
  #define _DEBUG
#else
  #include "python.h"
#endif

#define QFW      1
#define QBW      2
#define HFW      3
#define HBW      4
#define FDFD     5

#define GET_ANIMATION  1
#define GET_POSX       2
#define GET_POSY       3
#define GET_GAUGE      4

#define STZ3_P1    1
#define STZ3_P2    2
#define STZ3_WP1   3
#define STZ3_WP2   4
#define STZ3_HP_BAR_P1    1
#define STZ3_ISM_BAR_P1   2
#define STZ3_GUARD_BAR_P1 3
#define STZ3_HP_BAR_P2    4
#define STZ3_ISM_BAR_P2   5
#define STZ3_GUARD_BAR_P2 6

class Stz3Python
{
public:
	Stz3Python();
	~Stz3Python();

	int InitialStz3();
	int InitInterfaceFunction();
	int Initial();
	int Destroy();
	int RunFrameRoutine1();
	int CheckKeyBuffer();
	int RunPythonScript();

	int ToPython();
	int CmdQFW(int button);
	int CmdQBW(int button);
	int CmdHFW(int button);
	int CmdHBW(int button);
	int CmdFDFD(int button); // Nonsense name
	int PressButtonNFrame(short button1, short nFrame1,
						  short button2, short nFrame2,
						  short button3, short nFrame3);

	int GetAnimation(int objID);
	int GetPosX(int objID); // 1P, 2P, 1Pw, 2pw
	int GetPosY(int objID);
	int GetGauge(int barID); // HP, ISM, Guard

	PyObject* m_pName;
	PyObject* m_pModule;
	PyObject* m_pDict;
	PyObject* m_pFunc;
	PyObject* m_pValue;

	int m_frameNo;
	Stz3Character  m_p1;
	Stz3Character  m_p2;
	int m_p1Ani;
	int m_p2Ani;

	//PyMethodDef m_MyMethods;
	PyMethodDef m_stz3Method[3];
	//PyMethodDef m_stz3Method2;
	//PyMethodDef m_stz3Method3;
};

#endif // End #if !defined(WPJ_STZ3Python)