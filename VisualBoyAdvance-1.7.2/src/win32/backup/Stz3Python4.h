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

class Stz3Python
{
public:
	Stz3Python();
	~Stz3Python();

	int Initial();
	int Destroy();
	int RunPythonScript();
	int ToPython();

	PyObject* m_pName;
	PyObject* m_pModule;
	PyObject* m_pDict;
	PyObject* m_pFunc;
	PyObject* m_pValue;

	//PyMethodDef m_MyMethods[];
	PyMethodDef m_MyMethods;
};

#endif // End #if !defined(WPJ_STZ3Python)