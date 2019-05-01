'''py_function.py - Python source designed to '''
'''demonstrate the use of python embedding'''

import stz3

QCF  = 1
QCB  = 2
HCF  = 3
HCB  = 4
DPF  = 5

GET_ANIMATION = 1
GET_POSX      = 2
GET_POSY      = 3
GET_GAUGE     = 4
GET_DAMAGE    = 5
GET_FRAME     = 6

STZ3_P1  =  1
STZ3_P2  =  2
STZ3_WP1 =  3
STZ3_WP2 =  4
STZ3_HP_BAR_P1    = 1
STZ3_ISM_BAR_P1   = 2
STZ3_GUARD_BAR_P1 = 3
STZ3_HP_BAR_P2    = 4
STZ3_ISM_BAR_P2   = 5
STZ3_GUARD_BAR_P2 = 6

PRESS_A      =   1
PRESS_B      =   2
PRESS_SELECT =   4
PRESS_START  =   8
PRESS_RIGHT  =  16
PRESS_LEFT   =  32
PRESS_UP     =  64
PRESS_DOWN   = 128
PRESS_R      = 256
PRESS_L      = 512

def multiply():
    c = 12345*6789
    print 'The result of 12345 x 6789 :', c
    return c

def walk():
    return 16

def Main(ptr):
    p1x = stz3.CPGetData(ptr, GET_POSX, STZ3_P1)
    p2x = stz3.CPGetData(ptr, GET_POSX, STZ3_P2)
    delta = abs(p1x - p2x)
    delta = delta - 50
    result = 0    

    '''Hadoken'''
    '''mytuple = ((PRESS_DOWN, 2), (PRESS_DOWN|PRESS_RIGHT, 2), (PRESS_RIGHT, 2), (PRESS_B, 2), (0, 10))'''

    '''Patthai'''
    '''mytuple = ((PRESS_DOWN, 2), (PRESS_DOWN|PRESS_LEFT, 2), (PRESS_LEFT, 2), (PRESS_A, 2), (0, 60))'''
    
    '''Ryu combo'''
    mytuple = ((PRESS_UP|PRESS_LEFT, 24), (PRESS_B, 6), (0, 10), 
               (PRESS_L|PRESS_B, 2), 
               (PRESS_L|PRESS_B|PRESS_DOWN, 3), (PRESS_B|PRESS_DOWN|PRESS_RIGHT, 3), (PRESS_RIGHT, 3), (PRESS_A, 3),
               (0, 54), (PRESS_L, 6), (0, 6),
               (PRESS_DOWN|PRESS_L, 3),(PRESS_DOWN|PRESS_RIGHT|PRESS_L, 3), (PRESS_RIGHT, 3), (PRESS_B, 3),
               (0, 60))
    
    result = stz3.CPPressCustomCombo(ptr, mytuple)
	
    return result