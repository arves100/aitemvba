#include "stdafx.h"
#include "Stz3AIScript.h"
#include <time.h>

using namespace std;

// This AutoBattle module should seperate into another class, But not now...
int
Stz3AIScript::RunAutoBattle()
{
	int res;

	RunFrameRoutine();


	// This for randomly collect pic for QC, every 120f or 2 sec ---------------------------------------------------------------------------------------
	if (m_autoBattleState == 5)
	{
		if ((m_autoBattleStateF % 120) == 1)
		//if ((m_autoBattleStateF % 1) == 0)
		{
			//char name[64];
			//sprintf(name, "AutoBattle/Random_%03d_%06d_%06d_F%06d.png", m_pAutoBattleDataAni[m_lineCounter].m_p2, m_p2Ani, m_p1Ani, m_autoBattleStateF);
			//theApp.emulator.emuWritePNG(name);
		}
	}
	// -------------------------------------------------------------------------------------------------------------------------------------------------


	res = CheckAutoBattleKeyBuffer();
	if (res > 0) return res;

	switch (m_autoBattleState)
	{
		case 0: // press button for select char
		{
			AutoBattle0();
		}
		break;
		case 1: // wait for loading
		{
			AutoBattle1();
		}
		break;
		case 2: // wait for begin fight
		{
			AutoBattle2();
		}
		break;
		case 3: // begin to do...
		{
			AutoBattle3();
		}
		break;
		case 4:
		{
			AutoBattle4();
		}
		break;
		case 5:
		{
			AutoBattle5();
		}
		break;
		case 6: // press button to quit
		{
			AutoBattle6();
		}
		break;
		case 7: // wait to back at state 0
		{
			AutoBattle7();
		}
		break;

		default:
		break;
	}

	return 0;
}


int
Stz3AIScript::CheckAutoBattleKeyBuffer()
{
	if (m_autoBattleState != 5) // Another state, just normal
	{
		if (m_p1.m_busy)
		{
			return CheckKeyBuffer();
		}
		return -1;
	}	
	else // State 5, Fighting 
	{
		m_autoBattleStateF++; // This state need to count every frame
		if (m_autoBattleAI == AUTO_BATTLE_AI_HARDCODE) // Ugly code !!!!
		{
			if (m_p1.m_busy)
			{
				return CheckKeyBuffer();
			}
		}
		else // AUTO_BATTLE_AI_PYSCRIPT
		{
			if (theApp.pPython->m_p1.m_busy)
			{
				return theApp.pPython->CheckKeyBuffer();
			}
		}
	}
	return -1;
}


int
Stz3AIScript::AutoBattle0()
{
	int result = 0;
	m_charBaseValueP1 = 0;
	m_charBaseValueP2 = 0;
	m_charStaticDelta = 0;	

	if (m_lineCounter >= m_autoBattleLine) 
	{
		m_autoBattleState = AUTO_SYSTEM_END; // Finish Auto Battle
		
		// Call code to end Auto Battle -------------------------------
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
		// -------------------------------------------------------------

		return 0;
	}
	if (m_autoBattleStateF == 0)
	{
		// Prepare to move cursor to p1 char
		m_selP_R = m_pAutoBattleDataAni[m_lineCounter].m_p1 % 7;
		m_selP_D = m_pAutoBattleDataAni[m_lineCounter].m_p1 / 7;		
		m_autoBattleStateF++;
	}
	else if (m_autoBattleStateF == 1)
	{
		// Move Cursor rigt to select 1P character
		if (m_selP_R > 0)
		{
			result = PressButtonNFrame(PRESS_RIGHT, 30);
			m_selP_R--;
		}
		else {m_autoBattleStateF++;}
	}
	else if (m_autoBattleStateF == 2)
	{
		// Move Cursor down to select 1P character
		if (m_selP_D > 0)
		{
			result = PressButtonNFrame(PRESS_DOWN, 30);
			m_selP_D--;
		}
		else {m_autoBattleStateF++;}
	}
	else if (m_autoBattleStateF == 3)
	{
		// Press to select 1P Character (LB = Original Color)
		result = PressButtonNFrame(PRESS_L | PRESS_B, 30);
		m_autoBattleStateF++;

		// Cal next ISM Press button
		m_selP_D = m_pAutoBattleDataAni[m_lineCounter].m_ISMp1;
	}
	else if (m_autoBattleStateF == 4)
	{
		// Move Cursor down to select ISM 1P
		if (m_selP_D > 0)
		{
			result = PressButtonNFrame(PRESS_DOWN, 30);
			m_selP_D--;
		}
		else {m_autoBattleStateF++;}
	}
	else if (m_autoBattleStateF == 5)
	{
		// Press to select 1P Fighting Style
		result = PressButtonNFrame(PRESS_B, 30);
		m_autoBattleStateF++;
		
		// Prepare to move cursor to p2 char
		m_selP_R = m_pAutoBattleDataAni[m_lineCounter].m_p2 % 7;
		m_selP_D = m_pAutoBattleDataAni[m_lineCounter].m_p2 / 7;
		m_selP_R++; // Because now control 2p
	}
    // -------------------------------- FIN P1 -----------------------------------------------------------------
	else if (m_autoBattleStateF == 6)
	{
		// Move Cursor rigt to select 2P character
		if (m_selP_R > 0)
		{
			result = PressButtonNFrame(PRESS_RIGHT, 30);
			m_selP_R--;
		}
		else {m_autoBattleStateF++;}
	}
	else if (m_autoBattleStateF == 7)
	{
		// Move Cursor down to select 2P character
		if (m_selP_D > 0)
		{
			result = PressButtonNFrame(PRESS_DOWN, 30);
			m_selP_D--;
		}
		else {m_autoBattleStateF++;}
	}

	else if (m_autoBattleStateF == 8)
	{
		// Press to select 2P Character (LB = Original Color)
		result = PressButtonNFrame(PRESS_L | PRESS_B, 30);
		m_autoBattleStateF++;

		// Cal next ISM Press button
		m_selP_D = m_pAutoBattleDataAni[m_lineCounter].m_ISMp2;
	}
	else if (m_autoBattleStateF == 9)
	{
		// Move Cursor down to select ISM 2P
		if (m_selP_D > 0)
		{
			result = PressButtonNFrame(PRESS_DOWN, 30);
			m_selP_D--;
		}
		else {m_autoBattleStateF++;}
	}
	else if (m_autoBattleStateF == 10)
	{
		// Press to select 2P Fighting Style
		result = PressButtonNFrame(PRESS_B, 30);
		m_autoBattleStateF++;
	}
	else // After finish
	{
		m_autoBattleStateF = 0;
		m_autoBattleState++;
	}
	return result;
}


int
Stz3AIScript::AutoBattle1()
{
	if (m_p2.m_animation != 0) // Criteria to check state that game had come in to battle screen.
	{
		// This code use for checking the offset of animation data before fight -------------------------------
		// Writing begining tag
		sprintf(m_autoBattleLogBuffer, "Begin Auto battle between %d_%d %d_%d", 
			    m_pAutoBattleDataAni[m_lineCounter].m_p1, m_p1.m_charID, 
			    m_pAutoBattleDataAni[m_lineCounter].m_p2, m_p2.m_charID);
		fwrite(m_autoBattleLogBuffer, sizeof(char), strlen(m_autoBattleLogBuffer), m_autoBattlePStream);
		fwrite("\n", sizeof(char), 1, m_autoBattlePStream);
		// -----------------------------------------------------------------------------------------------------

		m_autoBattleState++;
		m_autoBattleStateF = 0;

		m_charBaseValueP1 = m_p1.m_animation;
		m_charBaseValueP2 = m_p2.m_animation;
	}
	return 0;
}


int
Stz3AIScript::AutoBattle2()
{
	// Find the minmun stand position to be base
	m_autoBattleStateF++;
	if (m_p1.m_animation < m_charBaseValueP1) {m_charBaseValueP1 = m_p1.m_animation;}
	if (m_p2.m_animation < m_charBaseValueP2) {m_charBaseValueP2 = m_p2.m_animation;}

	if (m_autoBattleStateF > 500) // 500f is waiting until the labe 'start', 'figt' finish.
	{
		// This code use for checking animation data between the fight -----------------------------------------
		m_p1.m_charIDStatic = m_p1.m_charID; // Copy value for checking is there change
		m_p2.m_charIDStatic = m_p2.m_charID;

		// P1
		int valueInDB = m_characterArrayOffsetType2[m_p1.m_charIDStatic];
		int valueInNow = m_p1.m_refAnimation - m_charBaseValueP1;
		if (valueInDB != valueInNow)
		{
			sprintf(m_autoBattleLogBuffer, "Error, P1 base animation not match with database: DB is %d: Now is %d", valueInDB,valueInNow);
			fwrite(m_autoBattleLogBuffer, sizeof(char), strlen(m_autoBattleLogBuffer), m_autoBattlePStream);
			fwrite("\n", sizeof(char), 1, m_autoBattlePStream);
		}
		// P2
		valueInDB = m_characterArrayOffsetType2[m_p2.m_charIDStatic];
		valueInNow = m_p2.m_refAnimation - m_charBaseValueP2;
		if (valueInDB != valueInNow)
		{
			sprintf(m_autoBattleLogBuffer, "Error, P2 base animation not match with database: DB is %d: Now is %d", valueInDB,valueInNow);
			fwrite(m_autoBattleLogBuffer, sizeof(char), strlen(m_autoBattleLogBuffer), m_autoBattlePStream);
			fwrite("\n", sizeof(char), 1, m_autoBattlePStream);
		}
		// ------------------------------------------------------------------------------------------------------
		m_autoBattleState++;
		m_autoBattleStateF = 0;
	}
	return 0;
}

int
Stz3AIScript::AutoBattle3()
{
	// Must press start button to open CPU mode
	int result = 0;

	if(m_autoBattleStateF == 0) // First initial
	{
		m_hasGameStopCounter = 0;
		m_tmpGameStopFailCounter = 0;
		m_tmpCharAni = m_p1.m_animation;
		m_autoBattleStateF++;

		result = PressButtonNFrame(PRESS_START, 30);
	}
	else
	{
		int ans = GameHasStop();
		if (ans == 0) // Comand to press start again
		{
			result = PressButtonNFrame(PRESS_START, 30);
		}
		else if (ans == 1) // Command to wait
		{
			return result;
		}
		else // Game has stop already, ready to do next
		{
			m_autoBattleState++;
			m_autoBattleStateF = 0;
		}
	}

	return result;
}


int
Stz3AIScript::AutoBattle4()
{
	int result = 0;

	if (m_autoBattleStateF == 0)
	{
		result = PressButtonNFrame(PRESS_DOWN, 30);
		m_autoBattleStateF++;
	}
	else if (m_autoBattleStateF == 1)
	{
		result = PressButtonNFrame(PRESS_A, 30);
		m_autoBattleStateF++;
	}
	else if (m_autoBattleStateF == 2)
	{
		result = PressButtonNFrame(PRESS_LEFT, 30);
		m_autoBattleStateF++;
	}

	else if (m_autoBattleStateF == 3)
	{
		result = PressButtonNFrame(PRESS_B, 30);
		m_autoBattleStateF++;
	}
	else if (m_autoBattleStateF == 4)
	{
		result = PressButtonNFrame(PRESS_B, 30);
		m_autoBattleStateF++;
	}
	else
	{
		m_autoBattleStateF = 0;
		m_autoBattleState++;

		if (m_autoBattleAI == AUTO_BATTLE_AI_HARDCODE)
		{
			theApp.myAIPlaying = true;
		}
		else
		{
			theApp.pythonPlaying = true;
		}
	}
	return result;
}


int
Stz3AIScript::AutoBattle5()
{
	//m_autoBattleStateF++; // Had already increment in CheckAutoBattleKeyBuffer();

	if (m_autoBattleStateF > m_pAutoBattleDataAni[m_lineCounter].m_time)
	{
		m_autoBattleState++;
		m_autoBattleStateF = 0;
		if (m_autoBattleAI == AUTO_BATTLE_AI_HARDCODE)
		{
			theApp.myAIPlaying = false;
		}
		else
		{
			theApp.pythonPlaying = false;
		}

		m_pAutoBattleDataAni[m_lineCounter].m_round--;
		if (m_pAutoBattleDataAni[m_lineCounter].m_round <= 0)
		{
			m_lineCounter++;
		}
	}

	// --------------------------------------------------------------------------------------------------------------------
	// This code use for checking animation data between the fight --------------------------------------------------------
	// P1
	int minAni = 0; if (m_p1.m_charID == C_VEGA) {minAni = -1392;} // If VEGA
	int maxAni = m_characterArrayMaxAniType2[m_p1.m_charID];
	char name[128];
	if (m_p1.m_charIDStatic == C_GEN) {m_p1Ani = 0;} // Cannot do this with Gen... Exception again
	if ((m_p1Ani < minAni) || (m_p1Ani > maxAni))
	{
		sprintf(m_autoBattleLogBuffer, "P1 Abnormally Found !!! %d %d", m_p1Ani, m_p1.m_animation);
		fwrite(m_autoBattleLogBuffer, sizeof(char), strlen(m_autoBattleLogBuffer), m_autoBattlePStream);
		fwrite("\n", sizeof(char), 1, m_autoBattlePStream);

		sprintf(name, "AutoBattle/%06d.png", m_p1Ani);
		theApp.emulator.emuWritePNG(name);
	}
	if ((m_p1.m_charID != m_p1.m_charIDStatic) &&
		(m_p1.m_charIDStatic != C_GEN)) // GEN is exception, he can change fighting mode
	{
		sprintf(m_autoBattleLogBuffer, "P1 Abnormally CharID change Found !!! %d  %d", m_p1.m_charID, m_p1.m_charIDStatic);
		fwrite(m_autoBattleLogBuffer, sizeof(char), strlen(m_autoBattleLogBuffer), m_autoBattlePStream);
		fwrite("\n", sizeof(char), 1, m_autoBattlePStream);
	}
	// P2
	minAni = 0; if (m_p2.m_charID == 10) {minAni = -1392;} // If VEGA
	maxAni = m_characterArrayMaxAniType2[m_p2.m_charID];
	if (m_p2.m_charIDStatic == C_GEN) {m_p2Ani = 0;} // Cannot do this with Gen... Exception again
	if ((m_p2Ani < minAni) || (m_p2Ani > maxAni))
	{
		sprintf(m_autoBattleLogBuffer, "P2 Abnormally Found !!! %d %d", m_p2Ani, m_p2.m_animation);
		fwrite(m_autoBattleLogBuffer, sizeof(char), strlen(m_autoBattleLogBuffer), m_autoBattlePStream);
		fwrite("\n", sizeof(char), 1, m_autoBattlePStream);

		sprintf(name, "AutoBattle/%06d.png", m_p2Ani);
		theApp.emulator.emuWritePNG(name);
	}
	if ((m_p2.m_charID != m_p2.m_charIDStatic) &&
		(m_p2.m_charIDStatic != C_GEN)) // GEN is exception, he can change fighting mode
	{
		sprintf(m_autoBattleLogBuffer, "P2 Abnormally CharID change Found !!! %d  %d", m_p2.m_charID, m_p2.m_charIDStatic);
		fwrite(m_autoBattleLogBuffer, sizeof(char), strlen(m_autoBattleLogBuffer), m_autoBattlePStream);
		fwrite("\n", sizeof(char), 1, m_autoBattlePStream);
	}
    // ---------------------------------------------------------------------------------------------------------------------
	// ---------------------------------------------------------------------------------------------------------------------
	return 0;
}


int
Stz3AIScript::AutoBattle6()
{
	if(theApp.emulator.emuReadState)
	{
		theApp.emulator.emuReadState("AI-TEM_Stz3Reload.sgm");
		m_autoBattleStateF = 0;
		m_autoBattleState++;

		sprintf(m_autoBattleLogBuffer, "End auto battle");
		fwrite(m_autoBattleLogBuffer, sizeof(char), strlen(m_autoBattleLogBuffer), m_autoBattlePStream);
		fwrite("\n", sizeof(char), 1, m_autoBattlePStream);
		fwrite("\n", sizeof(char), 1, m_autoBattlePStream);
	}
	return 0;
}

int
Stz3AIScript::AutoBattle7()
{
	m_autoBattleStateF++;
	if (m_autoBattleStateF > 250)
	{
		m_autoBattleState = 0; // Return to state 0
		m_autoBattleStateF = 0;
	}
	return 0;
}


int
Stz3AIScript::GameHasStop()
{
	if (m_tmpGameStopFailCounter > 120)
	{
		m_tmpGameStopFailCounter = 0;
		m_hasGameStopCounter = 0;
		return 0; // Press again
	}
	if (m_hasGameStopCounter > 30)
	{
		m_tmpGameStopFailCounter = 0;
		m_hasGameStopCounter = 0;
		return 2; // OK, pass
	}

	if (m_tmpCharAni != m_p1.m_animation)
	{
		m_tmpGameStopFailCounter++;
		m_tmpCharAni = m_p1.m_animation;
		m_hasGameStopCounter = 0;
	}
	else
	{
		m_hasGameStopCounter++;
	}

	return 1; // Wait
}


int
Stz3AIScript::AutoBattleINIParse()
{
	FILE* pStream;

	pStream = fopen("AutoBattleINI.txt", "rb");
	if (pStream == NULL)
	{
		return -1;
	}

	bool running = true;
	bool result = true;
	short sizeLine;
	char line[32];

	// Read Auto battle AI type (Hardcode, Python)
	result = fgets(line, 32, pStream);
	m_autoBattleAI = atoi(line);

	// Read number of line of auto battle script
	result = fgets(line, 32, pStream);
	m_autoBattleLine = atoi(line);
	int nCounter = 0;
	m_pAutoBattleDataAni = new AUTOBATTLE_INIDATA[m_autoBattleLine];
	
	while(running)
	{
		result = fgets(line, 32, pStream);
		if (result == NULL)
		{
			running = false;
		}
		else
		{			
			CString cs = line;
			CString resToken;
			int curPos = 0;

			if (nCounter < m_autoBattleLine)
			{
				resToken = cs.Tokenize("     ", curPos); // five space bar
				m_pAutoBattleDataAni[nCounter].m_p1 = atoi(resToken);
				resToken = cs.Tokenize("     ", curPos); // five space bar
				m_pAutoBattleDataAni[nCounter].m_ISMp1 = atoi(resToken);
				resToken = cs.Tokenize("     ", curPos); // five space bar
				m_pAutoBattleDataAni[nCounter].m_p2 = atoi(resToken);
				resToken = cs.Tokenize("     ", curPos); // five space bar
				m_pAutoBattleDataAni[nCounter].m_ISMp2 = atoi(resToken);
				resToken = cs.Tokenize("     ", curPos); // five space bar
				m_pAutoBattleDataAni[nCounter].m_round = atoi(resToken);
				resToken = cs.Tokenize("     ", curPos); // five space bar
				m_pAutoBattleDataAni[nCounter].m_time = atoi(resToken);
			}
			nCounter++;
		}
	}

	return 0;
}













