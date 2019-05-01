#if !defined(AI_STZ3Script)
#define AI_STZ3Script

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "Stz3.h"
#include "WinResUtil.h"
//#include <stdarg.h>
#include <deque>

typedef struct tagAniFrame_Pair {
	int frameNo; // character who is onwer of this message
	int animation;
} ANIFRAME_PAIR;

typedef std::deque<ANIFRAME_PAIR>  TAniDeque;
typedef TAniDeque::iterator	TAniDequeItor;

#define  MY_SUCCESS    0
#define  MY_UPDATE_GAME_STATE_NBYTE_ERROR    -1

#define  WAIT_DRAW_BEFORE_CAPTURE     5
#define  ALL_CHAR_SIZE    50
#define  AUTO_SYSTEM_END  99


// Define char name
#define  C_RYU         0 //  0
#define  C_KEN         1 //  1
#define  C_GOKI        2 //  2
#define  C_NASH        3 //  3
#define  C_CHUNLI      4 //  4
#define  C_ADON        5 //  5
#define  C_SODOM       6 //  6
#define  C_GUY         7 //  7
#define  C_BIRDIE      8 //  8
#define  C_ROSE        9 //  9
#define  C_VEGA       10 //  A
#define  C_SAGAT      11 //  B
#define  C_DAN        12 //  C
#define  C_SAKURA     13 //  D
#define  C_ROLENTO    14 //  E
#define  C_DHALSIM    15 //  F
#define  C_ZANGIEF    16 // 10
#define  C_GEN        17 // 11
//          ???       18 // 12
#define  C_GEN2       19 // 13
//          ???       20 // 14
//          ???       21 // 15
#define  C_CAMMY      22 // 16
#define  C_EVILRYU    23 // 17
#define  C_EHONDA     24 // 18
#define  C_BLANKA     25 // 19
#define  C_MIKA       26 // 1A
#define  C_CODY       27 // 1B
#define  C_BALOG      28 // 1C
#define  C_KARIN      29 // 1D
#define  C_JULI       30 // 1E
#define  C_JUNI       31 // 1F
#define  C_GUILE      32 // 20
#define  C_FEILONG    33 // 21
#define  C_DJ         34 // 22
#define  C_THAWK      35 // 23
//    SHIN_GOKI       36 // 24
#define  C_MBISON     37 // 25
#define  C_EAGLE      38 // 26
#define  C_MAKI       39 // 27
#define  C_YUN        40 // 28

#define  N_CHAR     41

#define  AUTO_BATTLE_AI_HARDCODE  0 // Open Hardcode AI when run AutoBattle
#define  AUTO_BATTLE_AI_PYSCRIPT  1 // Open Python script when run AutoBattle
  

class Stz3AIScript
{
public:
	Stz3AIScript();
	~Stz3AIScript();

	int Initial();
	int RunAIScript();

	int RunFrameRoutine();
	int CheckKeyBuffer();

	int m_frameNo;
	int m_p1Ani;
	int m_p2Ani;

	Stz3Character  m_p1;
	Stz3Character  m_p2;

	int DPF(short button);     // Dragon Punch Forward
	int QCF(short button);	   // Quarter Circle Front
	int QCB(short button);     // Quarter Circle Back
	int HCF(short button);     // Half Circle Front
	int HCB(short button);     // Half Circle Back
	int Throw(short button);
	int Guard(short state, short nFrame);
	int PressButtonNFrame(short button, short nFrame);	
	int PressCustomCombo(int nButton, int* pButton, int* pNFrame);

	int LoadCharacterOffset();
	int m_characterArrayOffset[N_CHAR];       // Order by select char in training mode screen
	int m_characterArrayOffsetType2[N_CHAR];  // Order by real game charID

	int m_characterArrayMaxAni[N_CHAR];       // Order by real game charID
	int m_characterArrayMaxAniType2[N_CHAR];  // Order by real game charID
	

	// *********************************************************************************************
	// **** These function use in Stz3AIScript_myEx, just for example ai (Stz3AIScript_myEx.cpp)****
	// *********************************************************************************************
	int ExampleAIMain();
	int CheckEnemyBullet();
	int CalCharState();
	int ExecuteCharState(short state);

	int ExecuteStClose0();
	int ExecuteStClose1();
	int ExecuteStClose2();

	int ExecuteStNear0();
	int ExecuteStNear1();
	int ExecuteStNear2();

	int ExecuteStMiddle0();
	int ExecuteStMiddle2();

	int ExecuteStFar0();
	int ExecuteStFar2();

	int CheckEnemyAttack();
	// *********************************************************************************************


	// *********************************************************************************************
	// ******** These member and function use in Animation Collection function (Stz3AutoCollectAnimation.cpp)
	// *********************************************************************************************

	int m_round;
	int m_captureCount;

	int m_toolState;
	int m_toolStateF;
	int m_selP_R;
	int m_selP_D;

	int  m_aniRecLine;
	char m_aniRecordFolder[256];
	char m_aniRecordMissFolder[256];
	
	AUTOANIREC_INIDATA* m_pAutoAniRecord;

	int m_waitDrawBeforeCapture;
	int m_charBaseValueP1;
	int m_charBaseValueP2;
	int m_charStaticDelta;
	int m_charStaticDeltaArray[ALL_CHAR_SIZE];

	int CollectAnimationData();
	int PostCollectAnimationData();

	int RunAutoAnimationRecord();
	int AutoCollectAnimate0();
	int AutoCollectAnimate1();
	int AutoCollectAnimate2();
	int AutoCollectAnimate3();
	int AutoCollectAnimate4();
	int AutoCollectAnimate5();

	int AutoAniCollectionINIParse();
	// *********************************************************************************************


	// *********************************************************************************************
	// **** These member and function use in AutoBattle function (Stz3AutoBattle.cpp) **************
	// *********************************************************************************************
	FILE*  m_autoBattlePStream;
	char   m_autoBattleLogBuffer[512];
	int    m_hasGameStopCounter;
	int    m_tmpCharAni;
	int    m_tmpGameStopFailCounter;
	int    m_autoBattleAI;

	int m_autoBattleLine;
	int m_lineCounter;
	int m_autoBattleState;
	int m_autoBattleStateF;

	AUTOBATTLE_INIDATA* m_pAutoBattleDataAni;

	int RunAutoBattle();
	int CheckAutoBattleKeyBuffer();
	int AutoBattle0();
	int AutoBattle1();
	int AutoBattle2();
	int AutoBattle3();
	int AutoBattle4();
	int AutoBattle5();
	int AutoBattle6();
	int AutoBattle7();

	int GameHasStop();
	int AutoBattleINIParse();

	// *********************************************************************************************
};

#endif // End #if !defined(AI_STZ3Script)
