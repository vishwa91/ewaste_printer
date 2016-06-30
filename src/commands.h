/* Project: Ewaste 3D Printer
 * Module: commands.h
 * Functionality: Defines constants and functions for decoding USB commands
 */

// First byte for class of command
#define CMD_CAL 	'C' 	// Calibrations
#define CMD_MOV 	'M' 	// Move
#define CMD_TST 	'T' 	// Testing motor
#define CMD_HLT 	'H' 	// Halt motor

// Second byte for specifics of the command
#define CMD_CAL_X 	'X' 	// Calibrate X
#define CMD_CAL_Y 	'Y' 	// Calibrate Y
#define CMD_CAL_Z 	'Z' 	// Calibrate Z

#define CMD_MOV_X 	'X' 	// Move X
#define CMD_MOV_Y 	'Y' 	// Move Y
#define CMD_MOV_Z 	'Z' 	// Move Z

#define CMD_TST_X 	'X' 	// Test run X
#define CMD_TST_Y 	'Y' 	// Test run Y
#define CMD_TST_Z 	'Z' 	// Test run Z

#define CMD_HLT_X 	'X' 	// Halt motor x

