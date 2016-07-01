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

# Constants
IDVENDOR = 0x16c0           # USB device vendor ID
IDPRODUCT = 0x0486          # USB device product ID
NBYTES = 64                 # Number of bytes to read from the HID device
DIR1 = 0                    # Direction towards switch 1
DIR2 = 1                    # Direction towards swithc 2

# Global position variables
max_x = 0                   # Maximum X steps
max_y = 0                   # Maximum Y steps
max_z = 0                   # Maximum Z steps

x_pos = 0                   # Current position of X
y_pos = 0                   # Current position of Y
z_pos = 0                   # Current position of Z

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

def _steps_calibrate(motor):
    '''
        Function to calibrate the steps for a motor.

        Inputs:
            motor: Motor to calibrate, 'X', 'Y' or 'Z'.

        Outputs:
            nsteps: Steps for the motor axis.
    '''
    dev.write('C'+motor)
    time.sleep(1)
    t = dev.read(NBYTES)

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

    t = dev.read(NBYTES)

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
        err_state = _move(axis, direction)
        time.sleep(delay)

        if err_state == -1:
            return moved_steps

        moved_steps += 1

    return moved_steps

def _move(axis, direction):
    '''
        Function to move an axis by a single step.

        Inputs:
            axis: Axis to move, 'X', 'Y', or 'Z'
            direction: Direction to move motor

        Outputs:
            err_state: 0 if axis isn't at extremum, -1 otherwise.
    '''
    global x_pos, y_pos, z_pos

    if axis == 'X':
        if (x_pos == 0 and direction == DIR1) or (
            x_pos == max_x and direction == DIR2):
            return -1

        dev.write('MX'+chr(direction)+chr(1));
        x_pos += 2*direction - 1;

    elif axis == 'Y':
        if (y_pos == 0 and direction == DIR1) or (
            y_pos == max_y and direction == DIR2):
            return -1

        dev.write('MY'+chr(direction)+chr(1));
        y_pos += 2*direction - 1;

    elif axis == 'Z':
        if (z_pos == max_z and direction == DIR1) or (
            z_pos == 0 and direction == DIR2):
            return -1

        dev.write('MZ'+chr(direction)+chr(1));
        z_pos += 2*direction - 1;

    return 0;

