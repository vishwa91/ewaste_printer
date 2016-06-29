/* Project: Ewaste 3D Printer
 * Module: motor.h
 * Functionality: Defines the various pin addresses and routines for motor
 *  			  control
 */

#include <stdint.h>
#include <avr_emulation.h>

#define MOTOR_X_SW2 		23 		// Limiting switch 1
#define MOTOR_X_SW1  		22 		// Limiting switch 2
#define MOTOR_X_DIR 		21 		// Motor direction pin
#define MOTOR_X_STP 		20 		// Motor step pin

#define MOTOR_Y_SW1 		18 		// Limiting switch 1
#define MOTOR_Y_SW2  		19 		// Limiting switch 2
#define MOTOR_Y_DIR 		17 		// Motor direction pin
#define MOTOR_Y_STP 		16 		// Motor step pin

#define MOTOR_Z_SW1 		14 		// Limiting switch 1
#define MOTOR_Z_SW2 		15 		// Limiting switch 2
#define MOTOR_Z_ENC 		10 		// Encoder input
#define MOTOR_Z_PLS 		12 		// Motor plus
#define MOTOR_Z_MNS 		11 		// Motor minus

#define MOTOR_E_DIR 		9 		// Motor direction pin
#define MOTOR_E_STP 		8 		// Motor step pin

#define X_AXIS 				0 		// Alias for X axis
#define Y_AXIS 				1 		// Alias for Y axis
#define Z_AXIS 				2 		// Alias for Z axis

#define MOTOR_OK 			3 		// No switches on
#define MOTOR_SW1_ON 		2 		// Limiting switch 1 is on
#define MOTOR_SW2_ON 		1 		// Limiting switch 2 is on

#define MOTOR_STP_INTERVAL  20 		// Duration of pulse in microseconds
#define STEP_DURATION 		1 		// Duration of a motor step.

#define DIR1 				0 		// Approaching SW1
#define DIR2 				1 		// Approaching SW2

void motor_init(void); 							// Motor initialization routines

uint16_t motor_x_calib(void); 					// X axis calibration
uint16_t motor_y_calib(void); 					// Y axis calibration
uint16_t motor_z_calib(void); 					// Z axis calibration

uint8_t _motor_x_move(int dir); 				// Single step X motion
uint8_t _motor_y_move(int dir); 				// Single step Y motion
uint8_t _motor_z_move(int dir); 				// Single step Z motion

uint8_t motor_x_move(int dir, int nsteps); 		// X axis motion
uint8_t motor_y_move(int dir, int nsteps); 		// Y axis motion
uint8_t motor_z_move(int dir, int nsteps); 		// Z axis motion

void enc_isr(void); 							// Encoder ISR
