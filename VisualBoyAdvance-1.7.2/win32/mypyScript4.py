'''py_function.py - Python source designed to '''
'''demonstrate the use of python embedding'''

import stz3

QFW  = 1
QBW  = 2
HFW  = 3
HBW  = 4
FDFD = 5

GET_ANIMATION = 1
GET_POSX      = 2
GET_POSY      = 3
GET_GAUGE     = 4

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
    #CPPressButtonNFrame(ptr, PRESS_RIGHT, 10, 0, 0, 0, 0)
    result = 0    
    if delta > 60:
        result = stz3.CPPressButtonNFrame(ptr, PRESS_RIGHT, 10, 0, 0, 0, 0)
    else:
	    #result = stz3.CPPressPackageCommand(ptr, QFW, PRESS_B)
	    result = stz3.CPPressButtonNFrame(ptr, PRESS_RIGHT | PRESS_B, 10, 0, 0, 0, 0)
    return result