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

	// Attach interrupt to encoder pin
	attachInterrupt(digitalPinToInterrupt(MOTOR_Z_ENC), enc_isr, RISING);
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
uint8_t _motor_x_move(int dir)
{
	// Write the direction
	digitalWrite(MOTOR_X_DIR, dir);

	// Send a pulse to the motor driver
	digitalWrite(MOTOR_X_STP, HIGH);
	delayMicroseconds(MOTOR_STP_INTERVAL);
	digitalWrite(MOTOR_X_STP, LOW);

	// Return switch status
	return 2*digitalRead(MOTOR_X_SW1) + digitalRead(MOTOR_X_SW2);
}

// Motion routines
uint8_t _motor_y_move(int dir)
{
	// Write the direction
	digitalWrite(MOTOR_Y_DIR, dir);

	// Send a pulse to the motor driver
	digitalWrite(MOTOR_Y_STP, HIGH);
	delayMicroseconds(MOTOR_STP_INTERVAL);
	digitalWrite(MOTOR_Y_STP, LOW);

	// Return switch status
	return 2*digitalRead(MOTOR_Y_SW1) + digitalRead(MOTOR_Y_SW2);
}

uint8_t _motor_z_move(int dir)
{
	if (dir == 0)
		digitalWrite(MOTOR_Z_PLS, HIGH);
	else
		digitalWrite(MOTOR_Z_MNS, HIGH);

	return 2*digitalRead(MOTOR_Z_SW1) + digitalRead(MOTOR_Z_SW2);
}

void enc_isr(void)
{
	// Simply shut down the DC motor outputs.
	digitalWrite(MOTOR_Z_PLS, LOW);
	digitalWrite(MOTOR_Z_MNS, LOW);
}
