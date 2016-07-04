#!/usr/bin/env python

'''
Project: Ewaste 3D Printer
Module: motor.py
Functionality: Contains routines for motor control through USB communication.

'''

# System imports
import os
import sys
import time
import cPickle

# Global variable dev.
dev = None

# Communication constants
IDVENDOR        = 0x16c0    # USB device vendor ID
IDPRODUCT       = 0x0486    # USB device product ID
NBYTES          = 64        # Number of bytes to read from the HID device
TIMEOUT_READ    = 2000      # Read timeout in milliseconds

# Global position variables
max_x           = 900       # Maximum X steps
max_y           = 900       # Maximum Y steps
max_z           = 150       # Maximum Z steps

# Store current position as a dictionary for easy access
pos = dict();
pos['X']        = 0         # Current position of X
pos['Y']        = 0         # Current position of Y
pos['Z']        = 0         # Current position of Z

# Constants from firmware
MOTOR_OK        = 3         # None of the switches are on.
MOTOR_SW1_ON    = 2         # Switch 1 is on
MOTOR_SW2_ON    = 1         # Switch 2 is on
DIR1            = 0         # Direction towards switch 1
DIR2            = 1         # Direction towards switch 2

def steps_calibrate():
    '''
        Function to calibrate the steps per each motor in the printer.

        Inputs:
            None

        Outputs:
            xsteps: Number of steps along X axis.
            ysteps: Number of steps along Y axis.
            zsteps: Number of steps along Z axis.
    '''
    # Calibrate each axis.
    xsteps = _steps_calibrate('X');
    ysteps = _steps_calibrate('Y');
    zsteps = _steps_calibrate('Z');

    return [xsteps, ysteps, zsteps]

def _steps_calibrate(axis):
    '''
        Function to calibrate the steps for a motor.

        Inputs:
            axis: Motor to calibrate, 'X', 'Y' or 'Z'.

        Outputs:
            nsteps: Steps for the motor axis.
    '''
    dev.write('C'+axis)
    time.sleep(2)
    dev.write('QC')
    t = dev.read(NBYTES, None)

    return 256*ord(t[1]) + ord(t[0])

def get_position():
    '''
        Function to get position from the firmware itself.

        Inputs:
            None.

        Outputs:
            xpos: X axis position with respect to switch 2.
            ypos: Y axis position with respect to switch 2.
            zpos: Z axis position with respect to switch 1.
    '''
    dev.write('QP')
    t = dev.read(NBYTES, TIMEOUT_READ)

    xpos = ord(t[0]) + ord(t[1])*256
    ypos = ord(t[2]) + ord(t[3])*256
    zpos = ord(t[4]) + ord(t[5])*256

    return [xpos, ypos, zpos]

def get_status():
    '''
        Function to get system status.

        Inputs:
            None.

        Outputs:
            xstate: X axis state.
            ystate: Y axis state.
            zstate: Z axis state.
    '''

    dev.write('QS')

    t = dev.read(NBYTES, TIMEOUT_READ)

    return [ord(t[0]), ord(t[1]), ord(t[2])]

def move(axis, nsteps, direction, delay=0.1):
    '''
        Function to move a motor axis for a given number of steps.

        Inputs:
            axis: Axis to move, 'X', 'Y', or 'Z'.
            nsteps: Number of steps to move.
            direction: Direction to move
            delay: Delay between steps.

        Outputs:
            moved_steps: The number of actual steps moved.
    '''
    moved_steps = 0
    for idx in range(nsteps):
        dev.write('M'+axis+chr(direction)+chr(1))

        # Get status to see if we have hit border.
        [sx, sy, sz] = get_status()
        if axis == 'X':
            status = sx
        elif axis == 'Y':
            status = sy
        else:
            status = sz

        if (direction == DIR1 and status == MOTOR_SW1_ON) or (
            direction == DIR2 and status == MOTOR_SW2_ON):
            return moved_steps

        # Update current position
        if direction == DIR1:
            pos[axis] += 1
        else:
            pos[axis] -= 1

        moved_steps += 1
        time.sleep(delay)

    return moved_steps

def move_z_down(delay=0.1):
    '''
        Function to move Z axis down. Moving down is an unreliable operation
        on the z-axis and hence we move till the first switch is on.

        Inputs:
            delay: Delay between steps.

        Outputs:
            None
    '''

    # Of course we know that the steps aren't greater than 200. So let's use
    # that.
    move('Z', 200, DIR2, delay)

def move_z_up(delay=0.1):
    '''
        Function to move Z axis up. Will be used when 2D plot testing.

        Inputs:
            delay: Delay between steps.

        Outputs:
            None
    '''

    # Of course we know that the steps aren't greater than 200. So let's use
    # that.
    move('Z', 200, DIR1, delay)


def reset(delay=0.001):
    '''
        Function to reset the position of the three axes to origin.

        Inputs:
            delay: Delay between steps.

        Outputs:
            None.
    '''

    # If the X or Y axes are touching SW2 initially, it might lose steps
    move('X', 100, DIR1, delay)
    move('Y', 100, DIR1, delay)

    # Now simply move by a large number of steps
    move('X', 1200, DIR2, delay)
    move('Y', 1200, DIR2, delay)
    move('Z', 200, DIR2, delay)

    # Reset positions
    pos['X'] = 0
    pos['Y'] = 0
    pos['Z'] = 0

def movexy(xsteps, ysteps, delay=0.01):
    '''
        Function to move the XY stage for a given x steps and y steps.
    '''
    pass
