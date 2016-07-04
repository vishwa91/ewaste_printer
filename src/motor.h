/* Project: Ewaste 3D Printer
 * Module: motor.h
 * Functionality: Defines the various pin addresses and routines for motor
 *  			  control
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>
#include <avr_emulation.h>

#define LED 				13 		// LED for debugging purposes

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

#define MOTOR_STP_INTERVAL  100		// Duration of pulse in microseconds
#define MOTOR_Z_INTERVAL 	400		// Short burst time for Z axis

#define MOTOR_X_CALIB_TIME  600		// X and Y calibration step interval
#define MOTOR_Z_CALIB_TIME 	10 		// Z calibration step interval
#define MOTOR_Z_PWM_VAL 	255 	// Z axis PWM value

#define DIR1 				0 		// Approaching SW1
#define DIR2 				1 		// Approaching SW2

#define ENABLE 				1 		// Enable a functionality
#define DISABLE 			0 		// Disable a functionality

void motor_init(void); 							// Motor initialization routines

void busy(void); 								// System busy
void idle(void); 								// System free

uint8_t get_x_state(void);						// Status of X motor
uint8_t get_y_state(void);						// Status of Y motor
uint8_t get_z_state(void);						// Status of Z motor

uint16_t motor_x_calib(void); 					// X axis calibration
uint16_t motor_y_calib(void); 					// Y axis calibration
uint16_t motor_z_calib(void); 					// Z axis calibration

uint8_t _motor_x_move(int dir); 				// Single step X motion
uint8_t _motor_y_move(int dir); 				// Single step Y motion
uint8_t _motor_z_move(int dir); 				// Single step Z motion

// Multi step motion
uint8_t motor_x_move(int dir, uint8_t nsteps, uint16_t step_delay);
uint8_t motor_y_move(int dir, uint8_t nsteps, uint16_t step_delay);
uint8_t motor_z_move(int dir, uint8_t nsteps, uint16_t step_delay);

void test_exec(void);							// Test mode execution

void enc_isr(void); 							// Encoder ISR

// Global motor related variables
extern uint8_t x_state, y_state, z_state;
extern uint8_t x_test, y_test, z_test;
extern volatile uint8_t x_dir, y_dir, z_dir;
extern volatile uint16_t x_pos, y_pos, z_pos;

// ISR variable
extern volatile uint8_t is_running;
#endif
