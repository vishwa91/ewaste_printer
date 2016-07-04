/* Project: Ewaste 3D Printer
 * Module: commands.cpp
 * Functionality: Defines constants and functions for decoding USB commands
 */

#include <motor.h>
#include <usb.h>
#include <commands.h>

void cmd_exec(void)
{
	switch(usb_in_buffer[0])
	{
		case CMD_CAL:
			cmd_cali();
			break;

		case CMD_MOV:
			cmd_move();
			break;

		case CMD_TST:
			cmd_test();
			break;

		case CMD_HLT:
			cmd_halt();
			break;

		case CMD_QRY:
			cmd_query();
			usb_send();
			break;

		default:
			break;
	}
}

void cmd_cali(void)
{
	uint16_t calib_steps = 0;

	// Find out which axis to calibrate.
	switch(usb_in_buffer[1])
	{
		case CMD_CAL_X:
			calib_steps = motor_x_calib();
			break;

		case CMD_CAL_Y:
			calib_steps = motor_y_calib();
			break;

		case CMD_CAL_Z:
			calib_steps = motor_z_calib();
			break;
	}

	// Load the data.
	usb_out_buffer[0] = (uint8_t)(calib_steps & 0xff);
	usb_out_buffer[1] = (uint8_t)((calib_steps >> 8) & 0xff);
}

void cmd_move(void)
{
	// Extract direction and number of steps.
	uint8_t dir, nsteps, steps, state;
	uint16_t step_delay = 0;

	steps = 0;
	state = 0;

	dir = usb_in_buffer[2];
	nsteps = usb_in_buffer[3];
	step_delay = usb_in_buffer[4] + 256*usb_in_buffer[5];

	// Flag busy.
	busy();
	
	// Find out which axis to move
	switch(usb_in_buffer[1])
	{
		case CMD_MOV_X:
			steps = motor_x_move(dir, nsteps, step_delay);
			state = get_x_state();
			break;

		case CMD_MOV_Y:
			steps = motor_y_move(dir, nsteps, step_delay);
			state = get_y_state();
			break;

		case CMD_MOV_Z:
			steps = motor_z_move(dir, nsteps, step_delay);
			state = get_z_state();
			break;
	}
	// Flag free.
	idle();

	// Load return data.
	usb_out_buffer[0] = state;
	usb_out_buffer[1] = steps;
}

void cmd_test(void)
{
	// Find out which axis's test mode to enable.
	switch(usb_in_buffer[1])
	{
		case CMD_TST_X:
			x_test = ENABLE;
			break;
			
		case CMD_TST_Y:
			y_test = ENABLE;
			break;

		case CMD_TST_Z:
			z_test = ENABLE;
			break;
	}
}

void cmd_halt(void)
{
	// Find out which axis's test mode to halt.
	switch(usb_in_buffer[1])
	{
		case CMD_HLT_X:
			x_test = DISABLE;
			break;

		case CMD_HLT_Y:
			y_test = DISABLE;
			break;

		case CMD_HLT_Z:
			z_test = DISABLE;
			break;
	}
}


void cmd_query(void)
{
	switch(usb_in_buffer[1])
	{
		case CMD_QRY_S:
			// Load all motor statuses
			usb_out_buffer[0] = get_x_state();
			usb_out_buffer[1] = get_y_state();
			usb_out_buffer[2] = get_z_state();
			break;

		case CMD_QRY_P:
			// First two bytes for X position
			usb_out_buffer[0] = (uint8_t)(x_pos & 0xff);
			usb_out_buffer[1] = (uint8_t)((x_pos >> 8) & 0xff);

			// Next two for Y position
			usb_out_buffer[2] = (uint8_t)(y_pos & 0xff);
			usb_out_buffer[3] = (uint8_t)((y_pos >> 8) & 0xff);

			// And finally Z position
			usb_out_buffer[4] = (uint8_t)(z_pos & 0xff);
			usb_out_buffer[5] = (uint8_t)((z_pos >> 8) & 0xff);
			break;

		case CMD_QRY_C:
			// Nothing to do, as data was loaded when calibration was done.
			break;
	}
}
