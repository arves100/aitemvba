#include "stdafx.h"
#include "Stz3AIScript.h"
#include <time.h>
#include "WinResUtil.h"

// This is simple old example that i make for test AI-TEM system
// The constant animation value in this code can use with RYU vs RYU only
// And there are many...comment of code that i do between testing my ai,
// should not interesting it.

int
Stz3AIScript::ExampleAIMain()
{
	int result;
	int state = 0;

	result = CheckEnemyAttack();
	if (result != 0)
	{
		return result;
	}
	
	state = CalCharState();
	result = ExecuteCharState(state);

	return result;
}


int
Stz3AIScript::CheckEnemyBullet()
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
		result = Guard(STAND_GUARD, 6);
	}
	else
	{
		result = QCF(PRESS_L);
	}
	return result;
}


// Calculate state depend on X and Y position
int
Stz3AIScript::CalCharState()
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


// Simple state decision
// There are 4 distance close, near, middle and far (depend on position X axis of 2 character.)
// Each distance will have 4 situation, (depend on position Y axis of two character.)
// ground-ground(0), ground-air(1), air-ground(2), air-air(3)
// I still not complete all 16 situations, so some of them is copy from another state
int
Stz3AIScript::ExecuteCharState(short state)
{
	int result = 0;

	switch (state)
	{
		//------ Close -------------
		case 0:
		{
			result = ExecuteStClose0(); // Ground-Ground
		}
		break;
		case 1:
		{
			result = ExecuteStClose1(); // Ground-Air
		}
		break;
		case 2:
		{
			result = ExecuteStClose2(); // Air-Ground
		}
		break;
		case 3:
		{
			result = ExecuteStClose0(); // Air-Air (Just copy from 0)
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
			result = ExecuteStNear0();  // Just copy from 0
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
			result = ExecuteStMiddle0();  // Just copy from 0
		}
		break;
		case 10:
		{
			result = ExecuteStMiddle2();
		}
		break;
		case 11:
		{
			result = ExecuteStMiddle0();  // Just copy from 0
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
			result = ExecuteStFar0();  // Just copy from 0
		}
		break;
		case 14:
		{
			result = ExecuteStFar2();
		}
		break;
		case 15:
		{
			result = ExecuteStFar0();  // Just copy from 0
		}
		break;

		default:
		break;
	}

	return result;
}


int
Stz3AIScript::ExecuteStClose0()
{
	int result = 0;
	int ran = rand() % 10;

	if ((m_p1Ani >= 2424) && (m_p1Ani <= 2628)) // Counter Attack
	{
		int button[4];
		int frame[4];
		button[0] = PRESS_DOWN;           frame[0] = 6;
		button[1] = PRESS_DOWN | PRESS_A; frame[1] = 6;
		result = PressCustomCombo(2, button, frame);
	}
	else //if (m_p1Ani <= 732) // 732 is maximum of normal action
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
				result = PressButtonNFrame(PRESS_B, 6);
			}
			else if (ran == 1)
			{
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
	
	if ((m_p1Ani >= 2940) && (m_p1Ani <= 2952)) // Riversal
	{
		if (ran < 5)
		{
			return DPF(PRESS_B | PRESS_L);
		}
		else
		{
			return Guard(CROUNCH_GUARD, 8);
		}
	}

	return result;
}


int
Stz3AIScript::ExecuteStClose1() // Ground-Air
{
	int result = 0;
	int ran = rand() % 10;
	
	if ((m_p1Ani >= 2424) && (m_p1Ani <= 2628)) // Counter Attack
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

	else //if (m_p1Ani <= 732) // 732 is maximum of normal action
	{
		if ((m_p2Ani >= 2916) && (m_p2Ani <= 2988)) // if enemy is riversal
		{
			result = PressButtonNFrame(m_p1.m_forward, 6);
		}
		else
		{
			result = DPF(PRESS_B | PRESS_L);
		}
	}

	return result;
}


int
Stz3AIScript::ExecuteStClose2() // Air - Ground
{
	int result = 0;

	if (m_p1.m_deltaX <= 10)
	{
		result = Throw(PRESS_B);
	}
	//if (m_p1Ani <= 732) // 732 is maximum of normal action
	{
		result = PressButtonNFrame(PRESS_A, 6);
	}
	return result;
}


int
Stz3AIScript::ExecuteStNear0()
{
	int result = 0;
	int ran = rand() % 10;

	if ((m_p1Ani >= 2424) && (m_p1Ani <= 2628)) // Counter Attack
	{
		if (m_p1.m_deltaX <= 50)
		{
			int button[4]; 
			int frame[4];
			button[0] = PRESS_DOWN;           frame[0] = 6;
			button[1] = PRESS_DOWN | PRESS_A; frame[1] = 6;
			result = PressCustomCombo(2, button, frame);
		}
	}
	else //if (m_p1Ani <= 732) // 732 is maximum of normal action
	{
		if ((m_p2Ani >= 2916) && (m_p2Ani <= 2988)) // if enemy is riversal
		{
			result = PressButtonNFrame(m_p1.m_forward, 6);
		}
		else
		{
			if (ran <= 0)
			{
				result = QCF(PRESS_B);				
			}
			else if (ran <= 1)
			{
				result = HCF(PRESS_B);				
			}
			else if (ran <= 3)
			{
				result = QCB(PRESS_A);				
			}
			else //if (ran <= 9)
			{
				result = PressButtonNFrame(PRESS_DOWN | PRESS_A, 6);
			}
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
			return HCF(PRESS_B);			
		}
	}

	return result;
}


int
Stz3AIScript::ExecuteStNear1() // Ground-Air
{
	int result = 0;
	int ran = rand() % 10;

	if ((m_p1Ani >= 2424) && (m_p1Ani <= 2628)) // Counter Attack
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
	else //if (m_p1Ani <= 732) // 732 is maximum of normal action
	{
		if ((m_p2Ani >= 2916) && (m_p2Ani <= 2988)) // if enemy is riversal
		{
			result = PressButtonNFrame(m_p1.m_forward, 6);
		}
		else
		{
			if (m_p1.m_deltaX < 50)
			{
				result = PressButtonNFrame(m_p1.m_backward, 6);
			}
		}
	}


	return result;
}


int
Stz3AIScript::ExecuteStNear2() // Air - Ground
{
	int result = 0;

	//if (m_p1Ani <= 732) // 732 is maximum of normal action
	{
		if (m_p1.m_deltaX < 75) // Jump Kick
		{
			result = PressButtonNFrame(PRESS_A, 6);
		}
	}
	return result;
}


int
Stz3AIScript::ExecuteStMiddle0() // Gorund - Ground
{
	int result = 0;
	int ran = rand() % 10;

	//if (m_p1Ani <= 732) // 732 is maximum of normal action
	{
		if ((m_p2Ani >= 0) && (m_p2Ani <= 216))
		{
			result = PressButtonNFrame(PRESS_UP | m_p1.m_forward, 6);
		}
		else
		{
			if (ran <= 3)
			{
				result = QCF(PRESS_B);
			}
			else if (ran <= 4)
			{
				result = HCF(PRESS_B);
			}
			else if (ran <= 5)
			{
				result = QCB(PRESS_A);
			}
			else //if (ran <= 9)
			{		
				result = PressButtonNFrame(PRESS_UP | m_p1.m_forward, 6);
			}
		}
	}
	
	if ((m_p1Ani >= 2940) && (m_p1Ani <= 2952)) // Riversal
	{
		return HCF(PRESS_B);
	}
	return result;
}


int
Stz3AIScript::ExecuteStMiddle2() // Air - Ground
{
	int result = 0;

	//if (m_p1Ani <= 732) // 732 is maximum of normal action
	{
		if (m_p1.m_deltaX < 50) // Jump Kick
		{
			result = PressButtonNFrame(PRESS_A, 6);
		}
	}
	return result;
}


int
Stz3AIScript::ExecuteStFar0()
{
	int result = 0;
	int ran = rand() % 10;

	//if (m_p1Ani <= 732) // 732 is maximum of normal action
	{
		if (ran <= 3)
		{
			result = QCF(PRESS_B);
		}
		else if (ran <= 5)
		{
			result = HCF(PRESS_B);
		}
		else if (ran <= 7)
		{
			result = QCB(PRESS_A);
		}
		else //if (ran <= 9)
		{
			result = PressButtonNFrame(PRESS_UP | m_p1.m_forward, 6);
		}
	}
	
	if ((m_p1Ani >= 2940) && (m_p1Ani <= 2952)) // Riversal
	{
		return HCF(PRESS_B);
	}
	return result;
}


int
Stz3AIScript::ExecuteStFar2() // Air - Ground
{
	int result = 0;

	//if (m_p1Ani <= 732) // 732 is maximum of normal action
	{
		if (m_p1.m_deltaX < 50) // Jump Kick
		{
			result = PressButtonNFrame(PRESS_A, 6);
		}
	}
	return result;
}


int
Stz3AIScript::CheckEnemyAttack()
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
			return Guard(STAND_GUARD, 6);
		}
	}
	return 0;
}