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
IDVENDOR = 0x16c0
IDPRODUCT = 0x0486

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
    t = dev.read(64)

    return 256*ord(t[1]) + ord(t[0])

