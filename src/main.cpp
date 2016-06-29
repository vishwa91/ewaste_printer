/* Project: Ewaste 3D Printer
 * Module: main.cpp
 * Functionality: The main execution file
 */

// System includes
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <avr_functions.h>
#include <avr_emulation.h>

#include <usb_dev.h>
#include <usb_rawhid.h>

#include <IntervalTimer.h>

// Custom includes
#include <motor.h>

// Preprocessor macros
#define TIMEOUT_RECV 	0 		// USB receive timeout.
#define TIMEOUT_SEND 	50 		// USB send timeout.
#define LED 			13 		// LED pin number.
#define MOTOR_DELAY 	600  	// Stepper motor delay time.

// USB buffer
uint8_t usb_buffer[64];

int main(void)
{
	// Variables
	uint8_t delay_val = 1;
	uint8_t nbytes;

	uint8_t x_dir = 0, y_dir = 0, z_dir = 0;
	uint8_t x_state = 0, y_state = 0, z_state = 0;

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
		nbytes = usb_rawhid_recv(usb_buffer, TIMEOUT_RECV);
		
		if (nbytes > 0)
		{
			// Read the delay between blinks
			delay_val = usb_buffer[0];
		}

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

	}
}
