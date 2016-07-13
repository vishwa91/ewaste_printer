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

volatile uint8_t x_dir = DIR1;
volatile uint8_t y_dir = DIR1;
volatile uint8_t z_dir = DIR1;

uint8_t x_test = DISABLE;
uint8_t y_test = DISABLE;
uint8_t z_test = DISABLE;

volatile int x_pos = 0;
volatile int y_pos = 0;
volatile int z_pos = 0;

volatile int z_max = 0;
volatile int z_pos_cur = 0;

IntervalTimer pos_timer_z;

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

	// Reset position
	x_pos = 0;

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

	// Reset position
	y_pos = 0;

	return nsteps;
}

uint16_t motor_z_calib(void)
{
	// Debugging
	busy();

	// Start with shutting down both pins
	analogWrite(MOTOR_Z_PLS, 0);
	analogWrite(MOTOR_Z_MNS, 0);

	// Shut down position timer now.
	pos_timer_z.end();

	// Move towards SW1 and then halt.
	z_dir = DIR2;
	while(digitalRead(MOTOR_Z_SW2) == 1)
	{
		analogWrite(MOTOR_Z_PLS, MOTOR_Z_PWM_VAL);
		delay(MOTOR_Z_CALIB_TIME);
	}
	analogWrite(MOTOR_Z_PLS, 0);

	// Reset variables.
	z_pos_cur = 0;
	z_dir = DIR1;

	// Now the motor is at switch 1. We can start count.
	while(digitalRead(MOTOR_Z_SW1) == 1)
	{
		analogWrite(MOTOR_Z_MNS, MOTOR_Z_PWM_VAL);
		delay(MOTOR_Z_CALIB_TIME);
	}
	analogWrite(MOTOR_Z_MNS, 0);

	// Wait for half a second and then we are done.
	delay(500);
	idle();

	// Now write the global variables.
	z_pos = z_max = z_pos_cur;

	// Switch on the position polling timer.
	pos_timer_z.begin(pos_func, POS_TIMER);

	return z_max;
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

	// Update position
	x_pos -= 2*dir - 1;

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

	// Update position
	y_pos -= 2*dir - 1;

	// Return switch status
	return get_y_state();
}

uint8_t _motor_z_move(int dir)
{
	// Just change the current position. Position timer will take care.
	if (dir == DIR1)
		z_pos -= 1;
	else
		z_pos += 1;

	if (z_pos > z_max)
		z_pos = z_max;
	if (z_pos < 0)
		z_pos = 0;

	delayMicroseconds(MOTOR_Z_INTERVAL);

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

	delay(MOTOR_X_CALIB_TIME);
}

void enc_isr(void)
{
	// Shut down the motor outputs.
	analogWrite(MOTOR_Z_PLS, 0);
	analogWrite(MOTOR_Z_MNS, 0);

	if (z_dir == DIR1)
		z_pos_cur += 1;
	else if (z_dir == DIR2)
		z_pos_cur -= 1;
}

void pos_func(void)
{
	// Regularly poll for position update.
	if (z_pos_cur > z_pos)
	{
		analogWrite(MOTOR_Z_PLS, MOTOR_Z_PWM_VAL);
		analogWrite(MOTOR_Z_MNS, 0);
		z_dir = DIR2;
	}
	else if (z_pos_cur < z_pos)
	{
		analogWrite(MOTOR_Z_PLS, 0);
		analogWrite(MOTOR_Z_MNS, MOTOR_Z_PWM_VAL);
		z_dir = DIR1;
	}
	else
	{
		analogWrite(MOTOR_Z_PLS, 0);
		analogWrite(MOTOR_Z_MNS, 0);
	}

}
