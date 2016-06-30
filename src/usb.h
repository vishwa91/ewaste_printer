/* Project: Ewaste 3D Printer
 * Module: usb.h
 * Functionality: Defines USB constants, variables and functions
 */

#ifndef USB_H_
#define USB_H_

#include <usb_dev.h>
#include <usb_rawhid.h>

#define TIMEOUT_RECV 	0 		// USB receive timeout
#define TIMEOUT_SEND 	50 		// USB send timeout
#define BUF_SIZE 		64 		// Size of USB buffers

// USB buffers
extern uint8_t usb_in_buffer[BUF_SIZE]; 		// Input buffer
extern uint8_t usb_out_buffer[BUF_SIZE]; 		// Output buffer

uint8_t usb_recv(void); 		// Wrapper for receiving
void usb_send(void); 			// Wrapper for sending

#endif
