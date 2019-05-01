#include "stdafx.h"
#include "MemCompareWrite.h"

#include "../GBA.h"
#include "../Globals.h"

CMemCompareWrite::CMemCompareWrite()
{
}


CMemCompareWrite::~CMemCompareWrite()
{
	Cleanup();
}


void
CMemCompareWrite::Initial()
{
	if ((!workRAM) || (!internalRAM))
	{
		return;
	}

	if(m_MCTSearchData.count == 0)
	{
		m_MCTSearchData.blocks = new CheatSearchBlock[2];

		CheatSearchBlock *block = &m_MCTSearchData.blocks[0];
		block->size = 0x40000;
		block->offset = 0x2000000;
		block->bits = (u8 *)malloc(0x40000>>3);
		block->data = workRAM;
		block->saved = (u8 *)malloc(0x40000);
	    
		block = &m_MCTSearchData.blocks[1];
		block->size = 0x8000;
		block->offset = 0x3000000;
		block->bits = (u8 *)malloc(0x8000>>3);
		block->data = internalRAM;
		block->saved = (u8 *)malloc(0x8000);
	    
		m_MCTSearchData.count = 2;
	}	
	//GetDlgItem(IDC_SEARCH)->EnableWindow(TRUE);
}


void
CMemCompareWrite::Cleanup()
{
    int count = m_MCTSearchData.count;
    for(int i = 0; i < count; i++)
    {
      free(m_MCTSearchData.blocks[i].saved);
      free(m_MCTSearchData.blocks[i].bits);
    }
	delete m_MCTSearchData.blocks;
    m_MCTSearchData.count = 0;
}


void
CMemCompareWrite::WriteFile()
{
	cheatSearchStart(&m_MCTSearchData);
	FILE* pStream = fopen(m_fullFileName, "wb");
	if (!pStream)
	{
		return;
	}

	int count = m_MCTSearchData.count;  
	for(int i = 0; i < count; i++) 
	{
		CheatSearchBlock *block = &m_MCTSearchData.blocks[i];
		fwrite(block->saved, block->size, 1, pStream);
	}	
}

