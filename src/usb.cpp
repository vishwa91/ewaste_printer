/* Project: Ewaste 3D Printer
 * Module: usb.c
 * Functionality: Defines USB constants, variables and functions
 */

#include <usb.h>

// Buffers
uint8_t usb_in_buffer[BUF_SIZE];
uint8_t usb_out_buffer[BUF_SIZE];

uint8_t usb_recv(void)
{
	return usb_rawhid_recv(usb_in_buffer, TIMEOUT_RECV);
}

void usb_send(void)
{
	usb_rawhid_send(usb_out_buffer, TIMEOUT_SEND);
}
