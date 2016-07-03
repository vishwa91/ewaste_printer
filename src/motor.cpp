/* Project: Ewaste 3D Printer
 * Module: motor.h
 * Functionality: Defines the various pin addresses and routines for motor
 *  			  control
 */

#include <motor.h>

// Global variables
uint8_t x_state = MOTOR_OK;
uint8_t y_state = MOTOR_OK;
uint8_t z_state = MOTOR_OK;

uint8_t x_dir = DIR1;
uint8_t y_dir = DIR1;
uint8_t z_dir = DIR1;

uint8_t x_test = DISABLE;
uint8_t y_test = DISABLE;
uint8_t z_test = DISABLE;

void motor_init(void)
{
	// Set directions for all pins

	// LED is output
	pinMode(LED, OUTPUT);

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
	attachInterrupt(digitalPinToInterrupt(MOTOR_Z_ENC), enc_isr, CHANGE);
}

void busy(void)
{
	digitalWrite(LED, LOW);
}

void idle(void)
{
	digitalWrite(LED, HIGH);
}

// Calibration routines
uint16_t motor_x_calib(void)
{
	uint16_t nsteps = 0;
	uint8_t state;

	// Move a step to get status
	state = _motor_x_move(DIR1);
	delayMicroseconds(MOTOR_X_CALIB_TIME);

	// Debugging
	digitalWrite(LED, LOW);

	// Move towards SW1 and then halt.
	while(state != MOTOR_SW1_ON)
	{
		state = _motor_x_move(DIR1);
		delayMicroseconds(MOTOR_X_CALIB_TIME);
	}

	// Now the motor is at switch 1. We can start count.
	while(state != MOTOR_SW2_ON)
	{
		state = _motor_x_move(DIR2);
		delayMicroseconds(MOTOR_X_CALIB_TIME);
		nsteps += 1;
	}
	digitalWrite(LED, HIGH);

	return nsteps;
}

uint16_t motor_y_calib(void)
{
	uint16_t nsteps = 0;
	uint8_t state;

	// Move a step to get status
	state = _motor_y_move(DIR1);
	delayMicroseconds(MOTOR_X_CALIB_TIME);

	// Debugging
	digitalWrite(LED, LOW);

	// Move towards SW1 and then halt.
	while(state != MOTOR_SW1_ON)
	{
		state = _motor_y_move(DIR1);
		delayMicroseconds(MOTOR_X_CALIB_TIME);
	}

	// Now the motor is at switch 1. We can start count.
	while(state != MOTOR_SW2_ON)
	{
		state = _motor_y_move(DIR2);
		delayMicroseconds(MOTOR_X_CALIB_TIME);
		nsteps += 1;
	}
	digitalWrite(LED, HIGH);

	return nsteps;
}

uint16_t motor_z_calib(void)
{
	uint16_t nsteps = 0;
	uint8_t state;

	// Move a step to get status
	state = _motor_z_move(DIR2);
	delay(MOTOR_Z_CALIB_TIME);

	// Debugging
	digitalWrite(LED, LOW);

	// Move towards SW1 and then halt.
	while(state != MOTOR_SW2_ON)
	{
		state = _motor_z_move(DIR2);
		delay(MOTOR_Z_CALIB_TIME);
	}

	// Now the motor is at switch 1. We can start count.
	while(state != MOTOR_SW1_ON)
	{
		state = _motor_z_move(DIR1);
		delay(MOTOR_Z_CALIB_TIME);
		nsteps += 1;
	}
	digitalWrite(LED, HIGH);

	return nsteps;
}

uint8_t get_x_state(void)
{
	return 2*digitalRead(MOTOR_X_SW1) + digitalRead(MOTOR_X_SW2);
}

uint8_t get_y_state(void)
{
	return 2*digitalRead(MOTOR_Y_SW1) + digitalRead(MOTOR_Y_SW2);
}

uint8_t get_z_state(void)
{
	return 2*digitalRead(MOTOR_Z_SW1) + digitalRead(MOTOR_Z_SW2);
}

// Motion routines
uint8_t motor_x_move(int dir, uint8_t nsteps, uint16_t step_delay)
{
	uint8_t steps, state;

	for(steps = 0; steps < nsteps; steps++)
	{
		// First get status
		state = get_x_state();

		if ((state == MOTOR_OK) || (state == MOTOR_SW2_ON && dir == DIR1) ||
				(state == MOTOR_SW1_ON && dir == DIR2))
		{
			_motor_x_move(dir);
			delay(step_delay);
		}
		else
			return steps+1;
	}
	return nsteps;
}

uint8_t motor_y_move(int dir, uint8_t nsteps, uint16_t step_delay)
{
	uint8_t steps, state;

	for(steps = 0; steps < nsteps; steps++)
	{
		// First get status
		state = get_y_state();

		if ((state == MOTOR_OK) || (state == MOTOR_SW2_ON && dir == DIR1) ||
				(state == MOTOR_SW1_ON && dir == DIR2))
		{
			_motor_y_move(dir);
			delay(step_delay);
		}
		else
			return steps+1;
	}
	return nsteps;
}

uint8_t motor_z_move(int dir, uint8_t nsteps, uint16_t step_delay)
{
	uint8_t steps, state;

	for(steps = 0; steps < nsteps; steps++)
	{
		// First get status
		state = get_z_state();

		if ((state == MOTOR_OK) || (state == MOTOR_SW2_ON && dir == DIR1) ||
				(state == MOTOR_SW1_ON && dir == DIR2))
		{
			_motor_z_move(dir);
			delay(step_delay);
		}
		else
			return steps+1;
	}
	return nsteps;
}

uint8_t _motor_x_move(int dir)
{
	// Write the direction
	digitalWrite(MOTOR_X_DIR, dir);

	// Send a pulse to the motor driver
	digitalWrite(MOTOR_X_STP, HIGH);
	delayMicroseconds(MOTOR_STP_INTERVAL);
	digitalWrite(MOTOR_X_STP, LOW);

	// Return switch status
	return get_x_state();
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
	return get_y_state();
}

uint8_t _motor_z_move(int dir)
{
	if (dir == DIR1)
		//digitalWrite(MOTOR_Z_MNS, HIGH);
		analogWrite(MOTOR_Z_MNS, MOTOR_Z_PWM_VAL);
	else
		//digitalWrite(MOTOR_Z_PLS, HIGH);
		analogWrite(MOTOR_Z_PLS, MOTOR_Z_PWM_VAL);

	return get_z_state();
}

void test_exec(void)
{
	if (x_test == ENABLE)
	{
		if (x_state == MOTOR_SW1_ON)
			x_dir = DIR2;
		if (x_state == MOTOR_SW2_ON)
			x_dir = DIR1;

		_motor_x_move(x_dir);
	}

	if (y_test == ENABLE)
	{
		if (y_state == MOTOR_SW1_ON)
			y_dir = DIR2;
		if (y_state == MOTOR_SW2_ON)
			y_dir = DIR1;

		_motor_y_move(y_dir);
	}

	if (z_test == ENABLE)
	{
		if (z_state == MOTOR_SW1_ON)
			z_dir = DIR2;
		if (z_state == MOTOR_SW2_ON)
			z_dir = DIR1;

		_motor_z_move(z_dir);
	}

	delay(STEP_DURATION);
}

void enc_isr(void)
{
	// Disable interrupts for a while.
	__disable_irq();
	
	// Simply shut down the DC motor outputs.
	digitalWrite(MOTOR_Z_PLS, LOW);
	digitalWrite(MOTOR_Z_MNS, LOW);

	// Reenable interrupts.
	__enable_irq();
}
