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

// USB buffer
uint8_t usb_buffer[64];

// Create an LED blink timer object.
IntervalTimer led_timer;
int led_state = LOW;
void led_blink_func(void);

int main(void)
{
	// Variables
	uint8_t delay_val = 255;
	uint8_t nbytes;

	// Start timer.
	//led_timer.begin(led_blink_func, delay_val);

	// Initialize USB.
	//usb_init();

	// Initialize motor peripherals.
	motor_init();

	// I want to control an LED.
	pinMode(LED, OUTPUT);

	// Halt till the device configures itself.
	//while(!usb_rawhid_available());

	// Give some time for the host to settle as well.
	//delay(1000);

	while (1)
	{
		/*
		// Receive and echo the packet.
		nbytes = usb_rawhid_recv(usb_buffer, TIMEOUT_RECV);
		
		if (nbytes > 0)
		{
			// Read the delay between blinks
			delay_val = usb_buffer[0];

			// Restart timer.
			led_timer.end();
			led_timer.begin(led_blink_func, delay_val*1000);

		}
		*/
		// I want to test X motor.
		if (digitalRead(MOTOR_Y_SW1) == LOW)
			digitalWrite(MOTOR_Y_DIR, LOW);
		
		if (digitalRead(MOTOR_Y_SW2) == LOW)
			digitalWrite(MOTOR_Y_DIR, HIGH);

		digitalWrite(MOTOR_Y_STP, HIGH);
		delayMicroseconds(400);
		digitalWrite(MOTOR_Y_STP, LOW);
		delayMicroseconds(400);
	}
}

void led_blink_func(void)
{
	if(led_state == LOW)
		led_state = HIGH;
	else
		led_state = LOW;

	digitalWrite(LED, led_state);
}
