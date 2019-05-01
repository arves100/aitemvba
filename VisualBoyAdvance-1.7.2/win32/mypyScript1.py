'''py_function.py - Python source designed to '''
'''demonstrate the use of python embedding'''

'''import emb'''
import stz3

''' define value -------------------------'''
QFW = 1

PRESS_A         1
PRESS_B         2
PRESS_SELECT    4
PRESS_START     8
PRESS_RIGHT    16
PRESS_LEFT     32
PRESS_UP       64
PRESS_DOWN    128
PRESS_R       256
PRESS_L       512
''' --------------------------------------'''

def multiply():
    c = 12345*6789
    print 'The result of 12345 x 6789 :', c
    return c

def walk():
    return 16

''' def walk2(ptr):'''
'''    c = emb.fromC(ptr, QFW)'''
'''    return c'''
    
def Main(ptr):
    c = stz3.CPressPackageCommand(ptr, QFW, PRESS_B)
    return c