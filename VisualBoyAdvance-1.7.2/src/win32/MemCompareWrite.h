#if !defined(MCT)
#define MCT

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "../CheatSearch.h"


class CMemCompareWrite
{
public:
	CMemCompareWrite();
	~CMemCompareWrite();

	void WriteFile();
	void Cleanup();
	void Initial();

	CheatSearchData  m_MCTSearchData;
	//CheatSearchBlock m_block1;
	//CheatSearchBlock m_block0;

	CString m_fileName;
	CString m_fullFileName;
};

#endif // End #if !defined(MCT)