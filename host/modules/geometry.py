#!/usr/bin/env python

'''
Project: Ewaste 3D Printer
Module: geometry.py
Functionality: Contains routines for geometry of drawing on 2D surfaces.

Notes:
    1. All coordinates are normalized between 0 and 1. Hence, if X-axis is
       limited to 1000 steps, then 0 corresponds to 0th step and 1 to 1000th
       step.
'''

# System imports
import os
import sys
import time
import cPickle

# Scientific computing
import numpy as np
import scipy as sp
import scipy.linalg as lin
import scipy.ndimage as ndim

# Custom imports
import motor

def goto2D(pos, delay=0.01):
    '''
        Function to move the XY stage to a given point.

        Inputs:
            pos: 2-tuple position of X and Y coordinates, each between 0 and 1.
            delay: Delay between steps.

        Outputs:
            None
    '''
    # Get previous coordinates in steps
    x1 = motor.pos['X']*motor.max_x
    y1 = motor.pos['Y']*motor.max_y

    # Get current coordinates
    x2 = pos[0]*motor.max_x
    y2 = pos[1]*motor.max_y

    # Compute step intervals
    dx = 1.0/motor.max_x
    dy = 1.0/motor.max_y

    # Compute coordinates using Bresenham line
    [x, y] = bresenham(x1, x2, y1, y2, dx, dy)

    # Convert to steps
    xint = [int(i) for i in x]
    yint = [int(i) for i in y]

    # Convert to incremental steps
    xsteps = xint[1:] - xint[:-1]
    ysteps = yint[1:] - yint[:-1]


