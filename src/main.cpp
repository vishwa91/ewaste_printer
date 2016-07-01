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
#include <commands.h>

int main(void)
{
	// Variables
	uint8_t nbytes;

	// Initialize USB.
	usb_init();

	// Initialize motor peripherals.
	motor_init();

	// Halt till the device configures itself.
	usb_wait();
	
	// System is idle.
	idle();

	while (1)
	{
		// Receive and echo the packet.
		nbytes = usb_recv();

		// Do nothing if no bytes are received.
		if (nbytes == 0)
			continue;		

		// Execute the command
		cmd_exec();

		// If any test mode is on, complete the routine.
		if (x_test || y_test || z_test)
			test_exec();

		// Send USB data only if status is asked
		if (usb_in_buffer[1] == CMD_STS)
			usb_send();

	}
}
