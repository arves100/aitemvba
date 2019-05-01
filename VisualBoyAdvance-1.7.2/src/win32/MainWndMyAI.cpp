#include "stdafx.h"
#include "MainWnd.h"
#include "WinResUtil.h"
#include "Stz3AIScript.h"
#include "Stz3Python.h"




void MainWnd::OnStartMyAI() 
{
	// Checking is this ROM is STZ3 or other case... !!!! -----------
	if (strcmp(theApp.romName, "STREET FIGHT") != 0)
	{
		systemScreenMessage("ROM may mismatch");
		//return;
	}
	// --------------------------------------------------------------
	if (theApp.pythonPlaying)return;

	if (!theApp.pAIScript)
	{
		theApp.pAIScript = new Stz3AIScript; // Must change to global AI !!!!
		if (theApp.pAIScript->Initial() != MY_SUCCESS)
		{
			delete theApp.pAIScript;
			theApp.pAIScript = NULL;
			return;
		}
	}

	//systemScreenMessage(winResLoadString(IDS_START_MYAI));
	systemScreenMessage(winResLoadString(IDS_Start_MyAI));	
	theApp.myAIPlaying = true;
}


void MainWnd::OnUpdateStartMyAI(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!theApp.myAIPlaying);
}


void MainWnd::OnStopMyAI() 
{
	if ((theApp.pAIScript) && (!theApp.autoBattle))
	{
		delete theApp.pAIScript;
		theApp.pAIScript = NULL;
	}
	//systemScreenMessage(winResLoadString(IDS_STOP_MYAI));
	systemScreenMessage(winResLoadString(IDS_Stop_MyAI));
	theApp.myAIPlaying = false;
}


void MainWnd::OnUpdateStopMyAI(CCmdUI* pCmdUI) 
{	
	pCmdUI->Enable(theApp.myAIPlaying);
}


void MainWnd::OnStartPythonScript()
{
	// Checking is this ROM is STZ3 or other case... !!!! -----------
	if (strcmp(theApp.romName, "STREET FIGHT") != 0)
	{
		systemScreenMessage("ROM may mismatch");
		//return;
	}

	if (theApp.myAIPlaying)return;

	theApp.pPython = new Stz3Python; // Must change to global AI !!!!
	if (theApp.pPython->Initial() != MY_SUCCESS)
	{
		delete theApp.pPython;
		theApp.pPython = NULL;
		return;
	}

	systemScreenMessage(winResLoadString(IDS_START_PYTHONSCRIPT));	
	theApp.pythonPlaying = true;
}


void MainWnd::OnUpdateStartPythonScript(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!theApp.pythonPlaying);
}


void MainWnd::CallOnStopPythonScript()
{
	OnStopPythonScript();
}


void MainWnd::OnStopPythonScript()
{
	if ((theApp.pPython) && (!theApp.autoBattle))
	{
		delete theApp.pPython;
		theApp.pPython = NULL;
	}	
	systemScreenMessage(winResLoadString(IDS_STOP_PYTHONSCRIPT));
	theApp.pythonPlaying = false;
}


void MainWnd::OnUpdateStopPythonScript(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(theApp.pythonPlaying);
}


void MainWnd::OnStartWriteGMDFile()
{
	CFileDialog dlg(TRUE, "File", "*.gmd", NULL, "GMD File (*.gmd)|*.gmd||");	
	if (dlg.DoModal() == IDCANCEL)  return;	
	theApp.memCompareWrite.m_fullFileName = dlg.GetPathName();
	theApp.memCompareWrite.m_fileName = dlg.GetFileName();

	theApp.memCompareWrite.Initial();
	theApp.memCompareWrite.WriteFile();
}


void MainWnd::OnStartAutoBattle()
{
	// Checking is this ROM is STZ3 or other case... !!!! -----------
	if (strcmp(theApp.romName, "STREET FIGHT") != 0)
	{
		systemScreenMessage("ROM may mismatch");
		//return;
	}
	if (theApp.aniRecording)
	{
		systemScreenMessage("Cannot start, Another auto system is running");
		return;
	}

	if (!theApp.pAIScript)
	{
		theApp.pAIScript = new Stz3AIScript; // Must change to global AI !!!!
		if (theApp.pAIScript->Initial() != MY_SUCCESS)
		{
			delete theApp.pAIScript;
			theApp.pAIScript = NULL;
			return;
		}

		// Automatic load state to the prompt state
		if(theApp.emulator.emuReadState)
		{
			theApp.emulator.emuReadState("AI-TEM_Stz3Reload.sgm");
		}
	}

	theApp.pAIScript->m_autoBattlePStream = fopen("AutoBattleLog.txt", "wt");
	if (!theApp.pAIScript->m_autoBattlePStream)
	{
		return;
	}
	theApp.pAIScript->AutoBattleINIParse();
	// After parse the INI file, if user want pyscript...create it
	if (theApp.pAIScript->m_autoBattleAI == AUTO_BATTLE_AI_PYSCRIPT)
	{
		theApp.pPython = new Stz3Python;
		if (theApp.pPython->Initial() != MY_SUCCESS)
		{
			delete theApp.pPython;
			theApp.pPython = NULL;
			return;
		}
	}

	systemScreenMessage(winResLoadString(IDS_START_AUTOBATTLE));	
	theApp.autoBattle = true;
}


void MainWnd::OnUpdateStartAutoBattle(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!theApp.autoBattle);
}


void MainWnd::OnStopAutoBattle()
{
	if (theApp.myAIPlaying)
	{
		OnStopMyAI(); // Stop AI Playing before
	}
	if (theApp.pythonPlaying)
	{
		OnStopPythonScript(); // Stop AI Playing before
	}

	if (theApp.pAIScript)
	{
		fclose(theApp.pAIScript->m_autoBattlePStream);
		delete theApp.pAIScript;
		theApp.pAIScript = NULL;
	}
	if (theApp.pPython)
	{
		delete theApp.pPython;
		theApp.pPython = NULL;
	}
	
	systemScreenMessage(winResLoadString(IDS_STOP_AUTOBATTLE));
	theApp.autoBattle = false;
}


void MainWnd::OnUpdateStopAutoBattle(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(theApp.autoBattle);
}


void MainWnd::OnStartAnimationRecord()
{
	// Checking is this ROM is STZ3 or other case... !!!! -----------
	if (strcmp(theApp.romName, "STREET FIGHT") != 0)
	{
		systemScreenMessage("ROM may mismatch");
		//return;
	}
	if (theApp.autoBattle)
	{
		systemScreenMessage("Cannot start, Another auto system is running");
		return;
	}

	if (!theApp.pAIScript)
	{
		theApp.pAIScript = new Stz3AIScript; // Must change to global AI !!!!
		if (theApp.pAIScript->Initial() != MY_SUCCESS)
		{
			delete theApp.pAIScript;
			theApp.pAIScript = NULL;
			return;
		}

		theApp.pAIScript->AutoAniCollectionINIParse();
		CreateDirectory(theApp.pAIScript->m_aniRecordFolder, 0);
		CreateDirectory(theApp.pAIScript->m_aniRecordMissFolder, 0);

		for (int i = 0; i < ALL_CHAR_SIZE; i++)
		{		
			theApp.pAIScript->m_charStaticDeltaArray[i] = 0;
		}

		// Automatic load state to the prompt state
		if(theApp.emulator.emuReadState)
		{
			theApp.emulator.emuReadState("AI-TEM_Stz3Reload.sgm");
		}
	}

	systemScreenMessage(winResLoadString(IDS_START_ANIRECORD));	
	theApp.aniRecording = true;
}


void MainWnd::OnUpdateStartAnimationRecord(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!theApp.aniRecording);
}


void MainWnd::OnStopAnimationRecord()
{
	if (theApp.pAIScript)
	{
		delete theApp.pAIScript;
		theApp.pAIScript = NULL;
	}
	
	systemScreenMessage(winResLoadString(IDS_STOP_ANIRECORD));
	theApp.aniRecording = false;
}


void MainWnd::OnUpdateStopAnimationRecord(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(theApp.aniRecording);
}