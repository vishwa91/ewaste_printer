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
	// Find out which axis to move
	switch(usb_in_buffer[1])
	{
		case CMD_MOV_X:
			x_state = _motor_x_move(usb_in_buffer[2]);
			break;

		case CMD_MOV_Y:
			y_state = _motor_y_move(usb_in_buffer[2]);
			break;

		case CMD_MOV_Z:
			z_state = _motor_z_move(usb_in_buffer[2]);
			break;
	}
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
	usb_out_buffer[0] = 2*digitalRead(MOTOR_X_SW1) + digitalRead(MOTOR_X_SW2);
	usb_out_buffer[1] = 2*digitalRead(MOTOR_Y_SW1) + digitalRead(MOTOR_Y_SW2);
	usb_out_buffer[2] = 2*digitalRead(MOTOR_Z_SW1) + digitalRead(MOTOR_Z_SW2);

	// TODO: Might load extruder information later.
}
