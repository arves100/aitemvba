#include "stdafx.h"
#include "Stz3.h"
#include "Stz3AIScript.h"

Stz3Character::Stz3Character()
{
}


Stz3Character::~Stz3Character()
{
}


int
Stz3Character::SetGameStateAddress(short p)
{
	// Bind Arrress
	// Hard code , this function must be correct to script, file import !!!!
	if (p == 1)
	{
		m_addrCharID    = 0x20008EA;
		m_addrHitPoint  = 0x2000818;
		m_addrISMBar    = 0x200090E;
		m_addrPosX      = 0x20007C2;
		m_addrPosY      = 0x20007c6;		
		
		m_addrAnimation    = 0x20007D0;
		m_addrRefAnimation = 0x2000828;

		m_addrWPosX     = 0x200C0DA;
		m_addrWPosY     = 0x0;

		m_flag = 0; // Left Side
		m_forward = PRESS_RIGHT;
		m_backward = PRESS_LEFT;

		m_addrWEnd      = 0x200c1c4;
		m_addrDamage    = 0x2000AAA;
	}
	else if (p == 2)
	{
		m_addrCharID    = 0x20044FA;
		m_addrHitPoint  = 0x2004428;
		m_addrISMBar    = 0x200451E;
		m_addrPosX      = 0x20043D2;
		m_addrPosY      = 0x20043D6;
		
		m_addrAnimation    = 0x20043E0;
		m_addrRefAnimation = 0x2004438;

		m_addrWPosX     = 0x20021AA;
		m_addrWPosY     = 0x0;
		m_flag = 1; // Right Side

		m_addrWEnd      = 0x2002294;
		m_addrDamage    = 0x20046BA;
	}
	m_charID = 0;
	m_charIDStatic = 0;
	m_ISMType = 0;
	m_hitPoint = 0;
	m_guardGauge = 0;
	m_ismBar = 0;
	m_posX = 0;
	m_posY = 0;
	m_wPosX = 0;
	m_wPosY = 0;
	m_animation = 0;
	m_aniOffset = 0;
	m_wEnd = 0;
	m_damage = 0;

	m_deltaX = 0;
	m_deltaY = 0;
	m_deltaWX = 0;
	m_deltaWY = 0;
	m_busy = 0;
	m_nextKBI = 0;
	m_KBStock = 0;

	UpdateGameStateData();

	return MY_SUCCESS;
}


int
Stz3Character::UpdateGameStateData()
{
	ReadAndSwapByte(m_addrCharID,    1, &m_charID);
	ReadAndSwapByte(m_addrHitPoint,  2, &m_hitPoint);
	ReadAndSwapByte(m_addrISMBar,    2, &m_ismBar);
	ReadAndSwapByte(m_addrPosX,      2, &m_posX);
	ReadAndSwapByte(m_addrPosY,      2, &m_posY);
	ReadAndSwapByte(m_addrAnimation,    4, &m_animation);
	ReadAndSwapByte(m_addrRefAnimation, 4, &m_refAnimation);
	ReadAndSwapByte(m_addrWPosX,     2, &m_wPosX);
	ReadAndSwapByte(m_addrWPosY,     0, &m_wPosY); // not use today !!!!
	ReadAndSwapByte(m_addrWEnd,      2, &m_wEnd);
	ReadAndSwapByte(m_addrDamage,    2, &m_damage);

	return MY_SUCCESS;
}


int
Stz3Character::ReadAndSwapByte(u32 addr, u16 nBytes, void* pAns)
{
	int j = 0;
	theApp.MyAIreadData(addr, nBytes, m_data);
	if (nBytes == 1)
	{
        *(int*)pAns = m_data[j];
	}
	else if (nBytes == 2)
	{
        *(int*)pAns = m_data[j] + (m_data[j+1]*256);
	}
	else if (nBytes == 4)
	{
        *(int*)pAns = (m_data[j]) + (m_data[j+1]*256) + 
			          (m_data[j+2]*65536) + (m_data[j+3]*16777216);
	}
	else
	{
		*(int*)pAns = 0;
		return MY_UPDATE_GAME_STATE_NBYTE_ERROR;
	}
	return MY_SUCCESS;
}


int
Stz3Character::UpdateGameCalData()
{
	m_deltaX = m_posX - m_pRival->m_posX;
	m_deltaY = m_posY - m_pRival->m_posY;
	if (m_deltaX < 0) // Case if my player is rightsede
	{
		m_deltaX = -1 * m_deltaX;
		m_flag = LEFT_SIDE;
		m_forward = PRESS_RIGHT;
		m_backward = PRESS_LEFT;
	}
	else
	{
		m_flag = RIGHT_SIDE;
		m_forward = PRESS_LEFT;
		m_backward = PRESS_RIGHT;
	}
	m_deltaX = m_deltaX - 50;

	m_deltaWX = m_posX - m_pRival->m_wPosX;
	if (m_deltaWX < 0) // Case if my player is rightsede
	{
		m_deltaWX = -1 * m_deltaWX;		
	}
	m_deltaWX = m_deltaWX - 50;

	return MY_SUCCESS;
}