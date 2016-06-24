/* Project: Ewaste 3D Printer
 * Module: motor.h
 * Functionality: Defines the various pin addresses and routines for motor
 *  			  control
 */

#include <motor.h>

void motor_init(void)
{
	// Set directions for all pins
	
	// All limiting switches are inputs
	pinMode(MOTOR_X_SW1, INPUT);
	pinMode(MOTOR_X_SW2, INPUT);

	pinMode(MOTOR_Y_SW1, INPUT);
	pinMode(MOTOR_Y_SW2, INPUT);

	pinMode(MOTOR_Z_SW1, INPUT);
	pinMode(MOTOR_Z_SW2, INPUT);

	// Encoder is input as well
	pinMode(MOTOR_Z_ENC, INPUT);

	// Steps and directions are outputs
	pinMode(MOTOR_X_STP, OUTPUT);
	pinMode(MOTOR_X_DIR, OUTPUT);

	pinMode(MOTOR_Y_STP, OUTPUT);
	pinMode(MOTOR_Y_DIR, OUTPUT);

	pinMode(MOTOR_Z_PLS, OUTPUT);
	pinMode(MOTOR_Z_MNS, OUTPUT);

	pinMode(MOTOR_E_STP, OUTPUT);
	pinMode(MOTOR_E_DIR, OUTPUT);

	// And so are DC motor pins
	pinMode(MOTOR_Z_PLS, OUTPUT);
	pinMode(MOTOR_Z_MNS, OUTPUT);
}

// Calibration routines
uint16_t motor_x_calib(void)
{
	return 0;
}

uint16_t motor_y_calib(void)
{
	return 0;
}

uint16_t motor_z_calib(void)
{
	return 0;
}

// Motion routines
void motor_x_move(int dir, int nsteps)
{
}

