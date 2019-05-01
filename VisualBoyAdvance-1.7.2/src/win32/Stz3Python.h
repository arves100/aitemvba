#if !defined(AI_STZ3Python)
#define AI_STZ3Python

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

#define P_QCF      1
#define P_QCB      2
#define P_HCF      3
#define P_HCB      4
#define P_DPF      5

#define GET_ANIMATION  1
#define GET_POSX       2
#define GET_POSY       3
#define GET_GAUGE      4
#define GET_DAMAGE     5
#define GET_FRAME      6

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
	int initstz3();
	int Initial();
	int Destroy();
	int RunFrameRoutine();
	int CheckKeyBuffer();
	int RunPythonScript();

	unsigned char ProcessLink(unsigned char in);

	int ToPython();
	int QCF(int button);
	int QCB(int button);
	int HCF(int button);
	int HCB(int button);
	int DPF(int button);
	int PressCustomCombo(int nButton, int* pButton, int* pNFrame);

	int GetAnimation(int objID);
	int GetPosX(int objID);   // 1P, 2P, 1Pw, 2pw
	int GetPosY(int objID);   // 1P, 2P, 1Pw, 2pw
	int GetGauge(int barID);  // HP, ISM, Guard
	int GetDamage(int objID); // 1P, 2P
	int GetFrameCounter();    // Get Number of frame that are running

	int m_characterArrayOffset[N_CHAR];       // Order by select char in training mode screen
	int m_characterArrayOffsetType2[N_CHAR];  // Order by real game charID
	int m_characterArrayMaxAni[N_CHAR];       // Order by real game charID
	int m_characterArrayMaxAniType2[N_CHAR];  // Order by real game charID
	int LoadCharacterOffset();

	PyMethodDef m_stz3Method[4];
	PyObject* m_pName;
	PyObject* m_pModule;
	PyObject* m_pDict;
	PyObject* m_pFunc;
	PyObject* m_pValue;
	PyObject* m_stz3MethodError;

	int m_frameNo;
	Stz3Character  m_p1;
	Stz3Character  m_p2;
	int m_p1Ani;
	int m_p2Ani;
};

#endif // End #if !defined(AI_STZ3Python)