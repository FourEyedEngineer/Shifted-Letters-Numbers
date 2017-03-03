#include "address_map_nios2.h"

/* function prototypes */
void put_jtag(volatile int *, char);
int change_button(int);

int main(void)
{
	/* Declare volatile pointers to I/O registers (volatile means that IO load
	   and store instructions will be used to access these pointer locations, 
	   instead of regular memory loads and stores) */
	volatile int * JTAG_UART_ptr 	= (int *) JTAG_UART_BASE;	// JTAG UART address

	int data, i;
	char text_string[] = "\nJTAG UART example code\n> \0";

	/* print a text string */
	for (i = 0; text_string[i] != 0; ++i)
		put_jtag (JTAG_UART_ptr, text_string[i]);

	/* read and echo characters */
	while(1)
	{
		data = *(JTAG_UART_ptr);		 		// read the JTAG_UART data register
		if (data & 0x00008000)					// check RVALID to see if there is new data
		{
			data = data & 0x000000FF;			// the data is in the least significant byte
			data = change_button(data);			// data will be changed according to it's value
			/* echo the character */
			put_jtag (JTAG_UART_ptr, (char) data & 0xFF );
		}
	}
}

/********************************************************************************
 * Subroutine to change the value of keyboard data
********************************************************************************/
int change_button(int c) 
{
	if ((c >= 65) && (c <= 90)) { //If the kay pressed is an upper case letter
		c = (c==90)?(c - 25):(c + 1); //Shifts letter to proceeding letter
		return c;
	}
	else if ((c >= 97) && (c <= 122)) { //If the key is a lower case letter
		c = (c==122)?(c - 25):(c + 1); //Shifts letter to proceeding letter
		return c;
	}
	else if((c >= 48) && (c <= 57)) { //If the key pressed is a numerical digit
		c = c + (9 - 2*(c - 48)); //Flips the value of digit pressed
		return c;
	}
	
	return c; //Return changed value
}

/********************************************************************************
 * Subroutine to send a character to the JTAG UART
********************************************************************************/
void put_jtag( volatile int * JTAG_UART_ptr, char c )
{
	int control;
	control = *(JTAG_UART_ptr + 1);			// read the JTAG_UART control register
	if (control & 0xFFFF0000)					// if space, then echo character, else ignore 
		*(JTAG_UART_ptr) = c;
}
