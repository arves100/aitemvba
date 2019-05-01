#include "stdafx.h"
#include "Stz3WpjScript.h"
#include <time.h>
#include "WinResUtil.h"

Stz3WpjScript::Stz3WpjScript()
{
	m_frameNo = 0;	
	m_subcounter = 0;

	m_p1Ani = 0;
	m_p2Ani = 0;
}


Stz3WpjScript::~Stz3WpjScript()
{
}


int
Stz3WpjScript::Initial()
{
	if (theApp.vbaLinkID > 2) theApp.vbaLinkID = 2;
	if (theApp.vbaLinkID < 1) theApp.vbaLinkID = 1;
	
	int rivalID = 2;
	if (theApp.vbaLinkID == 2) rivalID = 1;

	m_p1.SetGameStateAddress(theApp.vbaLinkID);
	m_p2.SetGameStateAddress(rivalID);
	m_p1.m_pRival = &m_p2;
	m_p2.m_pRival = &m_p1;

	srand((unsigned)time(NULL));

	return MY_SUCCESS;
}


/*
int
Stz3WpjScript::RunWpjScript()
{	
	int result = 0;

	// Do all of your script here
	// Use only function in Stz3.h only
	m_p1.UpdateGameStateData();
	m_p2.UpdateGameStateData();
	m_p1.UpdateGameCalData();
	m_p2.UpdateGameCalData();

	if (m_p1.m_busy)
	{
		result = m_p1.m_keyBuffer[m_p1.m_nextKBI];
		m_p1.m_nextKBI++;
		m_p1.m_KBStock--;
		if (m_p1.m_KBStock <= 0)
		{
			m_p1.m_busy = !m_p1.m_busy;
		}
		return result;
	}
	
	int ran = rand() % 10;
	
	int p2Ani = m_p1.m_pRival->m_animation - m_p1.m_pRival->m_aniOffset;
	if ((p2Ani >= 340) && (p2Ani <= 348))
	{
		ran = 11; // Debug
	}

	if (ran == 0)
	{
		if (m_p1.m_deltaX > 10) // i will move forward
		{
			if (m_p1.m_flag == LEFT_SIDE)
			{result = PRESS_RIGHT;}
			else
			{result = PRESS_LEFT;}
		}
	}
	else if (ran == 1)
	{
		if (m_p1.m_deltaX > 10) // i will jump forward
		{			
			if (m_p1.m_flag == LEFT_SIDE)
			{result = PRESS_RIGHT | PRESS_UP;}
			else
			{result = PRESS_LEFT | PRESS_UP;}
		}
	}
	else if (ran == 2)
	{
		// i will jump backward
		if (m_p1.m_flag == LEFT_SIDE)
		{result = PRESS_LEFT | PRESS_UP;}
		else
		{result = PRESS_RIGHT | PRESS_UP;}
	}
	else if (ran == 3)
	{
		Hadoken(PRESS_B);		
	}
	else if (ran == 4)
	{		
		Shoryken(PRESS_B);
	}
	else if (ran == 5)
	{		
		Patthai(PRESS_A);
	}
	else if (ran == 11) // guard
	{
		Guard();
	}


	return result; // Down, Return controller input
}
//*/


int
Stz3WpjScript::RunFrameRoutine1()
{
	m_p1.UpdateGameStateData();
	m_p2.UpdateGameStateData();
	m_p1.UpdateGameCalData();
	m_p2.UpdateGameCalData();

	m_p1Ani = m_p1.m_animation - m_p1.m_aniOffset;
	m_p2Ani = m_p2.m_animation - m_p2.m_aniOffset;

	return 0;
}


int
Stz3WpjScript::CheckKeyBuffer()
{
	int result = m_p1.m_keyBuffer[m_p1.m_nextKBI];
	m_p1.m_nextKBI++;
	m_p1.m_KBStock--;
	if (m_p1.m_KBStock <= 0)
	{
		m_p1.m_busy = !m_p1.m_busy;
	}
	return result;
}


int
Stz3WpjScript::CheckEnemyBullet()
{
	int result = 0;

	// if farther than 80% screen will counter shot
	if (m_p1.m_flag == LEFT_SIDE)
	{
		if (m_p1.m_posX > m_p2.m_wPosX)
		{
			return result;
		}
	}
	else
	{
		if (m_p1.m_posX < m_p2.m_wPosX)
		{
			return result;
		}
	}

	if ((m_p1.m_posY > 110)) // If i am in bullet way
	{
		return result;
	}

	if ((m_p1.m_deltaX <= 270) || (m_p1.m_deltaWX <= 150))
	{
		result = Guard(STAND_GUARD, 1);
	}
	else
	{
		result = Hadoken(PRESS_L);
	}
	return result;
}


int
Stz3WpjScript::CalCharState()
{
	int state = 0;
	// Whati will do will depend on distance
	if (m_p1.m_deltaX > 150) // Far 180-360
	{
		state = state + 12;
	}
	else if (m_p1.m_deltaX > 75) // Middle 115-180
	{
		state = state + 8;
	}
	else if (m_p1.m_deltaX > 20)// Near 20-75
	{
		state = state + 4;
	}
	else // < 20 Close
	{
		state = state + 0;
	}

	// 40 is ground
	if ((m_p1.m_posY == 40) && (m_p2.m_posY == 40)) // Grond - Ground
	{
		state = state + 0;
	}
	else if ((m_p1.m_posY == 40) && (m_p2.m_posY > 40)) // Ground - Air
	{
		state = state + 1;
	}
	else if ((m_p1.m_posY > 40) && (m_p2.m_posY == 40)) // Air - Ground
	{
		state = state + 2;
	}
	else // ((m_p1.m_posY > 40) && (m_p2.m_posY > 40)) // Air - Air
	{
		state = state + 3;
	}

	return state;
}


int
Stz3WpjScript::ExecuteCharState(short state)
{
	int result = 0;

	switch (state)
	{
		//------ Close -------------
		case 0:
		{
			result = ExecuteStClose0();
		}
		break;
		case 1:
		{
			result = ExecuteStClose1();
		}
		break;
		case 2:
		{
			result = ExecuteStClose2();
		}
		break;
		case 3:
		{
			result = ExecuteStClose0();
		}
		break;
		//------ Near -------------
		case 4:
		{
			result = ExecuteStNear0();
		}
		break;
		case 5:
		{
			result = ExecuteStNear1();
		}
		break;
		case 6:
		{
			result = ExecuteStNear2();
		}
		break;
		case 7:
		{
			result = ExecuteStNear0();
		}
		break;
		//------ Middle -------------
		case 8:
		{
			result = ExecuteStMiddle0();
		}
		break;
		case 9:
		{
			result = ExecuteStMiddle0();
		}
		break;
		case 10:
		{
			result = ExecuteStMiddle2();
		}
		break;
		case 11:
		{
			result = ExecuteStMiddle0();
		}
		break;
		//------ Far -------------
		case 12:
		{
			result = ExecuteStFar0();
		}
		break;
		case 13:
		{
			result = ExecuteStFar0();
		}
		break;
		case 14:
		{
			result = ExecuteStFar2();
		}
		break;
		case 15:
		{
			result = ExecuteStFar0();
		}
		break;

		default:
		break;
	}

	return result;
}


int
Stz3WpjScript::ExecuteStClose0()
{
	int result = 0;
	int ran = rand() % 10;

	if (m_p1Ani <= 732) // 732 is maximum of normal action
	{
		if ((m_p2Ani >= 2916) && (m_p2Ani <= 2988)) // if enemy is riversal
		{
			result = PressButtonNFrame(m_p1.m_forward, 6);
		}
		else
		{
			if (m_p1.m_deltaX <= 10)
			{
				result = Throw(PRESS_B);
			}

			else if (ran == 0)
			{
				//result = Hadoken(PRESS_B);
				result = PressButtonNFrame(PRESS_B, 6);
			}
			else if (ran == 1)
			{
				//result = Patthai(PRESS_A);
				result = PressButtonNFrame(PRESS_A, 6);
			}
			else if (ran == 2)
			{
				result = PressButtonNFrame(m_p1.m_forward | PRESS_B, 6);
			}
			else //if (ran <= 9)
			{
				result = PressButtonNFrame(PRESS_DOWN | PRESS_A, 6);
			}
		}
	}
	else if ((m_p1Ani >= 2424) && (m_p1Ani <= 2628)) // Counter Attack
	{
		result = PressButtonNFrame(m_p1.m_backward, 6);
	}
	
	if ((m_p1Ani >= 2940) && (m_p1Ani <= 2952)) // Riversal
	{
		if (ran < 5)
		{
			return Shoryuken(PRESS_B | PRESS_L);
		}
		else
		{
			return Guard(CROUNCH_GUARD, 8);
		}
	}

	return result;
}


int
Stz3WpjScript::ExecuteStClose1() // Ground-Air
{
	int result = 0;
	int ran = rand() % 10;

	if (m_p1Ani <= 732) // 732 is maximum of normal action
	{
		if ((m_p2Ani >= 2916) && (m_p2Ani <= 2988)) // if enemy is riversal
		{
			result = PressButtonNFrame(m_p1.m_forward, 6);
		}
		else
		{
			result = Shoryuken(PRESS_B | PRESS_L);
		}
	}
	else if ((m_p1Ani >= 2424) && (m_p1Ani <= 2628)) // Counter Attack
	{
		if (ran < 5)
		{
			result = PressButtonNFrame(m_p1.m_backward, 6);
		}
		else
		{
			result = PressButtonNFrame(PRESS_A, 6);
		}
	}

	return result;
}


int
Stz3WpjScript::ExecuteStClose2() // Air - Ground
{
	int result = 0;

	if (m_p1Ani <= 732) // 732 is maximum of normal action
	{
		result = PressButtonNFrame(PRESS_A, 6);
	}
	return result;
}


int
Stz3WpjScript::ExecuteStNear0()
{
	int result = 0;
	int ran = rand() % 10;

	if (m_p1Ani <= 732) // 732 is maximum of normal action
	{
		if ((m_p2Ani >= 2916) && (m_p2Ani <= 2988)) // if enemy is riversal
		{
			result = PressButtonNFrame(m_p1.m_forward, 6);
		}
		else
		{
			if (ran <= 0)
			{
				result = Hadoken(PRESS_B);
			}
			else if (ran <= 1)
			{
				result = HadokenFire(PRESS_B);
			}
			else if (ran <= 3)
			{
				result = Patthai(PRESS_A);
			}
			else //if (ran <= 9)
			{
				result = PressButtonNFrame(PRESS_DOWN | PRESS_A, 6);
			}
		}
	}
	else if ((m_p1Ani >= 2424) && (m_p1Ani <= 2628)) // Counter Attack
	{
		if (m_p1.m_deltaX <= 50)
		{
			result = PressButtonNFrame(m_p1.m_backward, 6);
		}
	}
	
	if ((m_p1Ani >= 2940) && (m_p1Ani <= 2952)) // Riversal
	{
		if (ran < 8)
		{
			return Guard(CROUNCH_GUARD, 8);	
		}
		else
		{			
			return HadokenFire(PRESS_B);
		}
	}

	return result;
}


int
Stz3WpjScript::ExecuteStNear1() // Ground-Air
{
	int result = 0;
	int ran = rand() % 10;

	if (m_p1Ani <= 732) // 732 is maximum of normal action
	{
		if ((m_p2Ani >= 2916) && (m_p2Ani <= 2988)) // if enemy is riversal
		{
			result = PressButtonNFrame(m_p1.m_forward, 6);
		}
		else
		{
			if (m_p1.m_deltaX < 50) // Straight Jump Kick
			{
				result = PressButtonNFrame(PRESS_UP | PRESS_A, 6);
			}
		}
	}
	else if ((m_p1Ani >= 2424) && (m_p1Ani <= 2628)) // Counter Attack
	{
		if (m_p1.m_deltaX <= 50)
		{
			if (ran < 5)
			{
				result = PressButtonNFrame(m_p1.m_backward, 6);
			}
			else
			{
				result = PressButtonNFrame(PRESS_A, 6);
			}
		}
	}

	return result;
}


int
Stz3WpjScript::ExecuteStNear2() // Air - Ground
{
	int result = 0;

	if (m_p1Ani <= 732) // 732 is maximum of normal action
	{
		if (m_p1.m_deltaX < 75) // Jump Kick
		{
			result = PressButtonNFrame(PRESS_A, 6);
		}
	}
	return result;
}


int
Stz3WpjScript::ExecuteStMiddle0() // Gorund - Ground
{
	int result = 0;
	int ran = rand() % 10;

	if (m_p1Ani <= 732) // 732 is maximum of normal action
	{
		if ((m_p2Ani >= 0) && (m_p2Ani <= 216))
		{
			result = PressButtonNFrame(PRESS_UP | m_p1.m_forward, 6);
		}
		else
		{
			if (ran <= 3)
			{
				result = Hadoken(PRESS_B);
			}
			else if (ran <= 4)
			{
				result = HadokenFire(PRESS_B);
			}
			//else if (ran <= 4)
			//{
			//	result = Hadoken(PRESS_SELECT);
			//}
			else if (ran <= 5)
			{
				result = Patthai(PRESS_A);
			}
			else //if (ran <= 9)
			{		
				result = PressButtonNFrame(PRESS_UP | m_p1.m_forward, 6);
			}
		}
	}
	
	if ((m_p1Ani >= 2940) && (m_p1Ani <= 2952)) // Riversal
	{
		return HadokenFire(PRESS_B);
	}
	return result;
}


int
Stz3WpjScript::ExecuteStMiddle2() // Air - Ground
{
	int result = 0;

	if (m_p1Ani <= 732) // 732 is maximum of normal action
	{
		if (m_p1.m_deltaX < 50) // Jump Kick
		{
			result = PressButtonNFrame(PRESS_A, 6);
		}
	}
	return result;
}


int
Stz3WpjScript::ExecuteStFar0()
{
	int result = 0;
	int ran = rand() % 10;

	if (m_p1Ani <= 732) // 732 is maximum of normal action
	{
		if (ran <= 3)
		{
			result = Hadoken(PRESS_B);
		}
		else if (ran <= 5)
		{
			result = HadokenFire(PRESS_B);
		}
		//else if (ran <= 4)
		//{
		//	result = Hadoken(PRESS_SELECT);
		//}
		else if (ran <= 7)
		{
			result = Patthai(PRESS_A);
		}
		else //if (ran <= 9)
		{
			result = PressButtonNFrame(PRESS_UP | m_p1.m_forward, 6);
		}
	}
	
	if ((m_p1Ani >= 2940) && (m_p1Ani <= 2952)) // Riversal
	{
		return HadokenFire(PRESS_B);
	}
	return result;
}


int
Stz3WpjScript::ExecuteStFar2() // Air - Ground
{
	int result = 0;

	if (m_p1Ani <= 732) // 732 is maximum of normal action
	{
		if (m_p1.m_deltaX < 50) // Jump Kick
		{
			result = PressButtonNFrame(PRESS_A, 6);
		}
	}
	return result;
}


int
Stz3WpjScript::CheckEnemyAttack()
{
	int result = 0;

	if ((m_p2.m_wEnd != 0) || 
		((m_p2Ani >= 4716) && (m_p2Ani <= 5472)) ||
		((m_p2Ani >= 7608) && (m_p2Ani <= 7776)))
	{
		result = CheckEnemyBullet();
		if (result != 0)
		{
			return result;
		}
	}

	if ((m_p2Ani >= 6114) && (m_p2Ani <= 6360)) // Super Patthai
	{
		if (m_p1.m_deltaX < 75)
		{			
			return Guard(STAND_GUARD, 1);
		}
	}
	return 0;
}


///*
// Do all of your script here
// Use only function in Stz3.h only
int
Stz3WpjScript::RunWpjScript()
{
	int result = 0;
	int state = 0;

	RunFrameRoutine1();
	m_frameNo++;
	if (m_p1.m_posX < 10) return result; // If not in battle

	// Check is there have a keybuffer to continuely execute from previoue round.	
	if (m_p1.m_busy)
	{
		return CheckKeyBuffer();
	}

	result = CheckEnemyAttack();
	if (result != 0)
	{
		return result;
	}
	
	state = CalCharState();
	result = ExecuteCharState(state);

	return result; // Return controller input
}


// User of this function should know about how to input param 'button'
int
Stz3WpjScript::Hadoken(short button)
{
	// If old hadoken still live, cannot fire again (but in V-ism...) !!!!
	if (m_p1.m_wEnd != 0) {return 0;}
	
	int i = 0;

	m_p1.m_keyBuffer[i] = PRESS_DOWN;                     i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN;                     i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_forward;    i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_forward;    i++;
	m_p1.m_keyBuffer[i] = m_p1.m_forward;                 i++;
	m_p1.m_keyBuffer[i] = m_p1.m_forward;                 i++;	
	m_p1.m_keyBuffer[i] = button;                         i++;
	m_p1.m_keyBuffer[i] = button;                         i++;

	/*
	for (int j = 0; j < 3; j++)
	{
		m_p1.m_keyBuffer[i] = 0;                          i++;
	}
	*/

	m_p1.m_KBStock = i;
	m_p1.m_nextKBI = 0;
	m_p1.m_busy = true;

	return 0;
}


int
Stz3WpjScript::HadokenFire(short button)
{
	if (m_p1.m_wEnd != 0) {return 0;}
	
	int i = 0;
	m_p1.m_keyBuffer[i] = m_p1.m_backward;                 i++;
	m_p1.m_keyBuffer[i] = m_p1.m_backward;                 i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_backward;    i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_backward;    i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN;                      i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN;                      i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_forward;     i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_forward;     i++;
	m_p1.m_keyBuffer[i] = m_p1.m_forward;                  i++;
	m_p1.m_keyBuffer[i] = m_p1.m_forward;                  i++;	
	m_p1.m_keyBuffer[i] = button;                          i++;
	m_p1.m_keyBuffer[i] = button;                          i++;

	m_p1.m_KBStock = i;
	m_p1.m_nextKBI = 0;
	m_p1.m_busy = true;

	return 0;
}


int
Stz3WpjScript::Patthai(short button)
{	
	int i = 0;

	m_p1.m_keyBuffer[i] = PRESS_DOWN;                       i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN;                       i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_backward;     i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_backward;     i++;
	m_p1.m_keyBuffer[i] = m_p1.m_backward;                  i++;
	m_p1.m_keyBuffer[i] = m_p1.m_backward;                  i++;
	m_p1.m_keyBuffer[i] = button;                           i++;
	m_p1.m_keyBuffer[i] = button;                           i++;

	m_p1.m_KBStock = i;
	m_p1.m_nextKBI = 0;
	m_p1.m_busy = true;

	/*
	for (i = i; i <= 50; i++)
	{
		m_p1.m_keyBuffer[i] = 0;
	}
	m_p1.m_KBStock = i;
	*/

	return 0;
}


int
Stz3WpjScript::Shoryuken(short button)
{	
	int i = 0;

	m_p1.m_keyBuffer[i] = m_p1.m_forward;                   i++;
	m_p1.m_keyBuffer[i] = m_p1.m_forward;                   i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN;                       i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN;                       i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_forward;      i++;
	m_p1.m_keyBuffer[i] = PRESS_DOWN | m_p1.m_forward;      i++;
	m_p1.m_keyBuffer[i] = button;                           i++;
	m_p1.m_keyBuffer[i] = button;                           i++;

	m_p1.m_KBStock = i;
	m_p1.m_nextKBI = 0;
	m_p1.m_busy = true;

	/*
	for (i = i; i <= 15; i++)
	{
		m_p1.m_keyBuffer[i] = 0;
	}
	m_p1.m_KBStock = i;
	*/

	return m_p1.m_forward;
}


int
Stz3WpjScript::Throw(short button)
{	
	int i = 0;

	m_p1.m_keyBuffer[i] = m_p1.m_forward | button;          i++;
	m_p1.m_keyBuffer[i] = m_p1.m_forward | button;          i++;

	m_p1.m_KBStock = i;
	m_p1.m_nextKBI = 0;
	m_p1.m_busy = true;

	return 0;
}


int
Stz3WpjScript::PressButtonNFrame(short button, short nFrame)
{	
	int i = 0;

	for (int j = 0; j <= nFrame; j++)
	{
		m_p1.m_keyBuffer[i] = button;    i++;
	}

	m_p1.m_KBStock = i;
	m_p1.m_nextKBI = 0;
	m_p1.m_busy = true;

	return button;
}


int
Stz3WpjScript::Guard(short state, short nFrame)
{
	int i = 0;
	int button = m_p1.m_backward;

	if (state == CROUNCH_GUARD)
	{
		button |= PRESS_DOWN;
	}

	for (i = i; i <= nFrame; i++)
	{
		m_p1.m_keyBuffer[i] = button;
	}
	m_p1.m_KBStock = i;
	m_p1.m_nextKBI = 0;
	m_p1.m_busy = true;

	return button;
}