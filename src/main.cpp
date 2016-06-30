/* Project: Ewaste 3D Printer
 * Module: main.cpp
 * Functionality: The main execution file
 */

// System includes
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <avr_functions.h>
#include <avr_emulation.h>

#include <IntervalTimer.h>

// Custom includes
#include <motor.h>
#include <usb.h>

int main(void)
{
	// Variables
	uint8_t delay_val = 1;
	uint8_t nbytes;

	uint8_t x_dir = 0, y_dir = 0, z_dir = 0;
	uint8_t x_state = 0, y_state = 0, z_state = 0;

	uint16_t calib_steps = 0;

	// Switch on LED to say that it's idle.
	pinMode(LED, OUTPUT);
	digitalWrite(LED, HIGH);

	// Initialize USB.
	usb_init();

	// Initialize motor peripherals.
	motor_init();

	// Halt till the device configures itself.
	while(!usb_rawhid_available());

	// Give some time for the host to settle as well.
	delay(1000);

	while (1)
	{
		// Receive and echo the packet.
		nbytes = usb_recv();

		// Do nothing if no bytes are received.
		if (nbytes == 0)
			continue;		

		// Calibration check
		if (usb_in_buffer[0] == CMD_CALIB)
		{
			if (usb_in_buffer[1] == CMD_CALIB_X)
				calib_steps = motor_x_calib();

			// Load data into buffer.
			usb_out_buffer[0] = (uint8_t)(calib_steps & 0xff);
			usb_out_buffer[1] = (uint8_t)((calib_steps >> 8) & 0xff);

			if (usb_in_buffer[1] == CMD_CALIB_Y)
				calib_steps = motor_y_calib();

			// Load data into buffer.
			usb_out_buffer[0] = (uint8_t)(calib_steps & 0xff);
			usb_out_buffer[1] = (uint8_t)((calib_steps >> 8) & 0xff);

			if (usb_in_buffer[1] == CMD_CALIB_Z)
				calib_steps = motor_z_calib();

			// Load data into buffer.
			usb_out_buffer[0] = (uint8_t)(calib_steps & 0xff);
			usb_out_buffer[1] = (uint8_t)((calib_steps >> 8) & 0xff);

		}
		// Send USB data
		usb_send();

		/*
		// See if we need to switch direction.
		if (x_state == MOTOR_SW1_ON)
			x_dir = DIR2;
		if (x_state == MOTOR_SW2_ON)
			x_dir = DIR1;

		if (y_state == MOTOR_SW1_ON)
			y_dir = DIR2;
		if (y_state == MOTOR_SW2_ON)
			y_dir = DIR1;

		if (z_state == MOTOR_SW1_ON)
			z_dir = DIR2;
		if (z_state == MOTOR_SW2_ON)
			z_dir = DIR1;

		// Move the motors a step.
		x_state = _motor_x_move(x_dir);
		y_state = _motor_y_move(y_dir);
		z_state = _motor_z_move(z_dir);

		delay(delay_val);
		*/
	}
}
