#include "stdafx.h"
#include "Stz3AIScript.h"
#include <time.h>

using namespace std;

int
Stz3AIScript::CollectAnimationData() // for collect animation
{

	m_p1.UpdateGameStateData(); // Now i will check p1 !!!!

	char name[128];
	int baseAni = m_p1.m_refAnimation - m_charStaticDeltaArray[m_pAutoAniRecord[m_lineCounter].m_p1]; // offset min stand
	int thisAni;

	if (m_captureCount < m_pAutoAniRecord[m_lineCounter].m_limitFrame)
	{
		thisAni = baseAni + (m_captureCount * 12);
        if (m_round <= 9)
		{			
			theApp.MyAIwriteData(m_p1.m_addrAnimation, 32, thisAni);
		}
		if (m_round >= m_waitDrawBeforeCapture)
		{	
			if (thisAni == m_p1.m_animation)
			{
				//sprintf(name, "%s/%03d/%06d_%06d.png", m_aniRecordFolder, m_pAutoAniRecord[m_lineCounter].m_p1, thisAni, (m_captureCount * 12));
				sprintf(name, "%s/%03d/%06d.png", m_aniRecordFolder, m_pAutoAniRecord[m_lineCounter].m_p1, (m_captureCount * 12));
				theApp.emulator.emuWritePNG(name);
				m_round = -1;
				m_captureCount++;
				m_waitDrawBeforeCapture = WAIT_DRAW_BEFORE_CAPTURE;
			}
			else if (m_round > 30)
			{
				m_round = -1;
				m_waitDrawBeforeCapture--;
				if (m_waitDrawBeforeCapture < 0)
				{
					//sprintf(name, "%s/%06d_%06d_Miss.png", m_aniRecordMissFolder, m_p1.m_animation, (m_captureCount * 12));
					sprintf(name, "%s/%06d_Miss.png", m_aniRecordMissFolder, (m_captureCount * 12));
					theApp.emulator.emuWritePNG(name);
					m_round = -1;
					m_captureCount++;
					m_waitDrawBeforeCapture = WAIT_DRAW_BEFORE_CAPTURE;
				}
			}
		}
		m_round++;
	}
	else
	{
		int end = 0;
		return -1; // End collect character anmation of 1 character;
	}
	
	return 0;
}


int
Stz3AIScript::PostCollectAnimationData()
{
	FILE*  pOutStream;
	char value[32];

	char path[256];
	sprintf(path, "%s/RecordCharStaticDelta.txt", m_aniRecordFolder);
	pOutStream = fopen(path, "wt");
	if (pOutStream == NULL)
	{
		return -1;
	}

	for (int i = 0; i < ALL_CHAR_SIZE; i++)
	{
		if (m_charStaticDeltaArray[i] == 0)
		{
			sprintf(value, "Not Entry");
			fwrite(value, sizeof(char), strlen(value), pOutStream);
			fwrite("\n", sizeof(char), 1, pOutStream);
		}
		else
		{
			sprintf(value, "%08d", m_charStaticDeltaArray[i]);
			fwrite(value, sizeof(char), 8, pOutStream);
			fwrite("\n", sizeof(char), 1, pOutStream);
		}
	}

	fclose(pOutStream);
	return MY_SUCCESS;
}



int
Stz3AIScript::RunAutoAnimationRecord()
{
	RunFrameRoutine();	

	if (m_p1.m_busy)
	{
		return CheckKeyBuffer();
	}

	switch (m_toolState)
	{
		case 0: // press button for select char
		{
			AutoCollectAnimate0();
		}
		break;
		case 1: // wait ofr loading
		{
			AutoCollectAnimate1();
		}
		break;
		case 2: // wait for begin fight
		{
			AutoCollectAnimate2();
		}
		break;
		case 3: // begin to do...
		{
			AutoCollectAnimate3();
		}
		break;
		case 4: // press button to quit
		{
			AutoCollectAnimate4();
		}
		break;
		case 5: // wait to back at state 0
		{
			AutoCollectAnimate5();
		}
		break;

		default:
		break;
	}

	return 0;
}


int
Stz3AIScript::AutoCollectAnimate0()
{
	int result = 0;
	m_charBaseValueP1 = 0;
	m_charBaseValueP2 = 0;
	m_charStaticDelta = 0;

	if (m_lineCounter >= m_aniRecLine) 
	{
		PostCollectAnimationData();
		m_toolState = AUTO_SYSTEM_END;  // Finish Auto CollectData
		
		// Call code to end Animation collection ------------------
		if (theApp.pAIScript)
		{
			delete theApp.pAIScript;
			theApp.pAIScript = NULL;
		}	
		systemScreenMessage(winResLoadString(IDS_STOP_ANIRECORD));
		theApp.aniRecording = false;
		// --------------------------------------------------------

		return 0;		
	}

	if (m_toolStateF == 0)
	{
		// Prepare to move cursor to p1 char
		m_selP_R = m_pAutoAniRecord[m_lineCounter].m_p1 % 7;
		m_selP_D = m_pAutoAniRecord[m_lineCounter].m_p1 / 7;
		m_toolStateF++;

		// Create Directory to store animation picture
		char path[256];
		sprintf(path, "%s/%03d", m_aniRecordFolder, m_pAutoAniRecord[m_lineCounter].m_p1 );
		CreateDirectory(path, 0);
	}
	else if (m_toolStateF == 1)
	{
		// Move Cursor rigt to select 1P character
		if (m_selP_R > 0)
		{
			result = PressButtonNFrame(PRESS_RIGHT, 30);
			m_selP_R--;
		}
		else {m_toolStateF++;}
	}
	else if (m_toolStateF == 2)
	{
		// Move Cursor down to select 1P character
		if (m_selP_D > 0)
		{
			result = PressButtonNFrame(PRESS_DOWN, 30);
			m_selP_D--;
		}
		else {m_toolStateF++;}
	}
	else if (m_toolStateF == 3)
	{
		// Press to select 1P Character (LB = Original Color)
		result = PressButtonNFrame(PRESS_L | PRESS_B, 30);
		m_toolStateF++;

		// Cal next ISM Press button
		m_selP_D = m_pAutoAniRecord[m_lineCounter].m_ISMp1;
	}
	else if (m_toolStateF == 4)
	{
		// Move Cursor down to select ISM 1P
		if (m_selP_D > 0)
		{
			result = PressButtonNFrame(PRESS_DOWN, 30);
			m_selP_D--;
		}
		else {m_toolStateF++;}
	}
	else if (m_toolStateF == 5)
	{
		// Press to select 1P Fighting Style
		result = PressButtonNFrame(PRESS_B, 30);
		m_toolStateF++;
	}
	else if (m_toolStateF == 6)
	{
		// Press to select 2P Character (LB = Original Color)
		// No need to concern 2P style or ISM in animation collection
		result = PressButtonNFrame(PRESS_L | PRESS_B, 30);
		m_toolStateF++;
	}
	else if (m_toolStateF == 7)
	{
		// Press to select 2P Fighting Style
		result = PressButtonNFrame(PRESS_B, 30);
		m_toolStateF++;
	}
	else
	{
		m_toolStateF = 0;
		m_toolState++;
	}
	return result;
}


int
Stz3AIScript::AutoCollectAnimate1()
{
	if (m_p1.m_animation != 0)
	{
		m_toolState++;
		m_toolStateF = 0;
		m_charBaseValueP1 = m_p1.m_animation;
	}
	return 0;
}


int
Stz3AIScript::AutoCollectAnimate2()
{
	// Find the minmun stand position to be base
	m_toolStateF++;
	if (m_p1.m_animation < m_charBaseValueP1)
	{
		m_charBaseValueP1 = m_p1.m_animation;
	}

	if (m_toolStateF > 500)
	{
		m_charStaticDeltaArray[m_pAutoAniRecord[m_lineCounter].m_p1] = m_p1.m_refAnimation - m_charBaseValueP1;
		m_toolState++;
		m_toolStateF = 0;
	}
	return 0;
}

int
Stz3AIScript::AutoCollectAnimate3()
{
	// Real collection
	if (CollectAnimationData() != MY_SUCCESS)
	{
		m_toolState++;
		m_toolStateF = 0;
		m_lineCounter++;

		m_captureCount = 0;
	}

	return 0;
}

int
Stz3AIScript::AutoCollectAnimate4()
{
	if(theApp.emulator.emuReadState)
	{
		theApp.emulator.emuReadState("AI-TEM_Stz3Reload.sgm");
		m_toolStateF = 0;
		m_toolState++;
	}
	return 0;
}

int
Stz3AIScript::AutoCollectAnimate5()
{
	m_toolStateF++;
	if (m_toolStateF > 250)
	{
		m_toolState = 0; // Return to state 0
		m_toolStateF = 0;
	}
	return 0;
}


int
Stz3AIScript::AutoAniCollectionINIParse()
{
	FILE* pStream;

	pStream = fopen("AutoAniCollectionINI.txt", "rb");
	if (pStream == NULL)
	{
		return -1;
	}

	bool running = true;
	bool result = true;
	short sizeLine;
	char line[256];

	result = fgets(m_aniRecordFolder, 256, pStream); // Get path of recorder
	int sl = strlen(m_aniRecordFolder);
	m_aniRecordFolder[sl-2] = 0; // !!!! subtract \n newline code
	sprintf(m_aniRecordMissFolder, "%s/Miss", m_aniRecordFolder);

	result = fgets(line, 32, pStream); // Get number of character recorder
	m_aniRecLine = atoi(line);

	int nCounter = 0;
	m_pAutoAniRecord = new AUTOANIREC_INIDATA[m_aniRecLine];
	
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

			if (nCounter < m_aniRecLine)
			{
				resToken = cs.Tokenize("  ", curPos); // 2 space bar
				m_pAutoAniRecord[nCounter].m_p1 = atoi(resToken);
				resToken = cs.Tokenize("  ", curPos);
				m_pAutoAniRecord[nCounter].m_ISMp1 = atoi(resToken);
				resToken = cs.Tokenize("  ", curPos);
				m_pAutoAniRecord[nCounter].m_limitFrame = atoi(resToken);
			}
			nCounter++;
		}
	}

	return 0;
}