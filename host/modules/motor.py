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
IDVENDOR = 0x16c0           # USB device vendor ID
IDPRODUCT = 0x0486          # USB device product ID
NBYTES = 64                 # Number of bytes to read from the HID device
TIMEOUT_READ = 500          # Read timeout in milliseconds

# Global position variables
max_x = 0                   # Maximum X steps
max_y = 0                   # Maximum Y steps
max_z = 0                   # Maximum Z steps

x_pos = 0                   # Current position of X
y_pos = 0                   # Current position of Y
z_pos = 0                   # Current position of Z

# Constants from firmware
MOTOR_OK        = 3
MOTOR_SW1_ON    = 2
MOTOR_SW2_ON    = 1
DIR1 = 0                    # Direction towards switch 1
DIR2 = 1                    # Direction towards swithc 2

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

    # Also set the global variables
    global max_x, max_y, max_z
    max_x = xsteps;
    max_y = ysteps;
    max_z = zsteps;

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
    t = dev.read(NBYTES, TIMEOUT_READ)

    return 256*ord(t[1]) + ord(t[0])

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

    dev.write('S')

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
        [sx, sy, sz] = get_status();
        if axis == 'X':
            status = sx
        elif axis == 'Y':
            status = sy
        else:
            status = sz

        if (direction == DIR1 and status == MOTOR_SW1_ON) or (
            direction == DIR2 and status == MOTOR_SW2_ON):
            return moved_steps

        moved_steps += 1
        time.sleep(delay)

    return moved_steps

