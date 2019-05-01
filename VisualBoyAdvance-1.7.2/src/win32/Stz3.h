#if !defined(AI_STZ3)
#define AI_STZ3

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif


#define STZ3_UPDATE_CHAR_OK        0
#define LEFT_SIDE    0
#define RIGHT_SIDE   1

#define PRESS_A         1
#define PRESS_B         2
#define PRESS_SELECT    4
#define PRESS_START     8
#define PRESS_RIGHT    16
#define PRESS_LEFT     32
#define PRESS_UP       64
#define PRESS_DOWN    128
#define PRESS_R       256
#define PRESS_L       512

#define STAND_GUARD     0
#define CROUNCH_GUARD   1

#define KEY_BUFFER_SIZE   1024

struct AUTOBATTLE_INIDATA
{
	int m_p1;
	int m_ISMp1;
	int m_p2;
	int m_ISMp2;
	int m_round;
	int m_time;
};

struct AUTOANIREC_INIDATA
{
	int m_p1;
	int m_ISMp1;
	int m_limitFrame;
};


class Stz3Character
{
public:
	Stz3Character();
	~Stz3Character();
	
	int SetGameStateAddress(short p);
	int UpdateGameStateData();
	int ReadAndSwapByte(u32 addr, u16 nBytes, void* pAns);
	int UpdateGameCalData();

	char m_charID;
	char m_ISMType;     // Fighting Style (A, X, V)

	int m_charIDStatic; // Due to that stupid problem about value change	
	int m_hitPoint;     // HP
	int m_guardGauge;   // Guard damage guage
	int m_ismBar;       // Special move gauge
	int m_posX;         // Position in X axis
	int m_posY;         // Position in Y axis
	int m_wPosX;        // Position of his weapon bullet (Ex.Hadoken, SonicBoom)
	int m_wPosY;        // Position of his weapon bullet (Ex.Hadoken, SonicBoom)
	int m_animation;    // Animation of character before normalize
	int m_refAnimation; // Base value (not base animation) that will use in normalization
	int m_aniOffset;    // Offset value that also use in normalization
	int m_wEnd;
	int m_damage;       // It wil be accumulate combo damage ***

	u32 m_addrCharID;
	u32 m_addrHitPoint;  // Address of those data
	u32 m_addrISMBar;    // Address of those data
	u32 m_addrPosX;
	u32 m_addrPosY;
	u32 m_addrAnimation;
	u32 m_addrRefAnimation;
	u32 m_addrWPosX;
	u32 m_addrWPosY;    
	u32 m_addrWEnd;      // 0 if no weapon, or ot will countdown
	u32 m_addrDamage;    // It wil be accumulate combo damage

	u8 m_data[8]; // buffer data for get game state
	Stz3Character* m_pRival;

	int   m_deltaX;
	int   m_deltaY;
	int   m_deltaWX;
	int   m_deltaWY;
	bool  m_flag;      // 0 = Left, 1 = Right side
	bool  m_busy;      // 0=not busy-idle, 1=busy
	short m_forward;   // Which button is backward direction Left or Right, depend on your side
	short m_backward;  // Which button is backward direction Left or Right, depend on your side
	
	int m_nextKBI;        // Key buffer index, tell what index will use next;
	int m_KBStock;        // Key buffer stock, tell how many KB left
	int m_keyBuffer[KEY_BUFFER_SIZE]; // Key buffer for long command, 1024 should enought?

	short m_state;
};

#endif // End #if !defined(AI_STZ3)