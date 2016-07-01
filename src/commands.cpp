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

		case CMD_STS:
			cmd_stat();
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
	steps = 0;
	state = 0;

	dir = usb_in_buffer[2];
	nsteps = usb_in_buffer[3];

	// Flag busy.
	busy();
	
	// Find out which axis to move
	switch(usb_in_buffer[1])
	{
		case CMD_MOV_X:
			steps = motor_x_move(dir, nsteps);
			state = get_x_state();
			break;

		case CMD_MOV_Y:
			steps = motor_y_move(dir, nsteps);
			state = get_y_state();
			break;

		case CMD_MOV_Z:
			steps = motor_z_move(dir, nsteps);
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


void cmd_stat(void)
{
	// Load all motor statuses
	usb_out_buffer[0] = get_x_state();
	usb_out_buffer[1] = get_y_state();
	usb_out_buffer[2] = get_z_state();

	// TODO: Might load extruder information later.
}
