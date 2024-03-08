#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "xil_printf.h"
#include "xgpio.h"
#include "xparameters.h"
#include "sleep.h"
#include "PmodTC1.h"

PmodTC1 myDevice;

// Initialize the LCD
void LCD_init();

// Send Commands to LCD
void LCD_Send_Command (unsigned char);

// Write Data to LCD
void LCD_Write_Data (unsigned char);

// Write a String to LCD (uses LCD_Write_Data function)
void LCD_Send_String (char*, int);

// Write a Number to LCD (uses LCD_Send_string function)
void LCD_Send_Number (int);

// Global Declaration of LCD GPIO Objects
XGpio lcd_lines, lcd_rs_en, btns;

int main() {

    init_platform();


    while(1){

    LCD_init(); // Initialize LCD

    LCD_Send_Command (0x80); // Place cursor at Home (0,0)
    LCD_Send_String ("  Hello World!  ", 16);
    LCD_Send_Command (0xC0); // Place cursor at second line (1,0)
    LCD_Send_String ("  EEL4740", 9);

    sleep(2);
    LCD_Send_Command (0x01); // Clear Display
    LCD_Send_Command (0x80); // Place cursor at Home (0,0)
    LCD_Send_String (" Temperature:   ", 16);
    int state = 1; /* Initial state is Celsius */

    while(1) {

    	int btn = XGpio_DiscreteRead(&btns, 1);

    	double temp;
    	char s[16];

    	if (btn != 0) {
    		state = btn;
    	}
    	else {
    		//state = state;
    	}

    	if (state == 1){ /* Celsius Measurement */
    		temp = TC1_getTemp(&myDevice);
    		sprintf(s,"%0.2f  ", temp);


    		 LCD_Send_Command (0xC0); // Place cursor at second line (1,0)
    		 LCD_Send_String(s, 7);  // Write count value
    		 LCD_Send_String("Celsius   ",10);
    		 sleep(1);

    	}
    	else if(state == 2){ /* Fahrenheit measurement */

    		temp = TC1_getTemp(&myDevice);
    		temp = TC1_tempC2F(temp);

    		sprintf(s,"%0.2f  ", temp);

    		LCD_Send_Command (0xC0); // Place cursor at second line (1,0)
    		LCD_Send_String(s, 7);  // Write count value
    		LCD_Send_String("Fahrenheit",10);
    		sleep(1);
    	}
    	else if (state == 4){ /* Kelvin Measurement */

    		temp = TC1_getTemp(&myDevice) + 273.15;
    		sprintf(s,"%0.2f  ", temp);

    		LCD_Send_Command (0xC0); // Place cursor at second line (1,0)
    		LCD_Send_String(s, 7);  // Write count value
    		LCD_Send_String("Kelvin    ",10);
    		sleep(1);
    	}
    	else { /* Exit */
    		break;
    	}

    }

    }
    cleanup_platform();
    return 0;

}


void LCD_init() {

	XGpio_Initialize(&lcd_lines, XPAR_AXI_GPIO_0_DEVICE_ID);
	XGpio_Initialize(&lcd_rs_en, XPAR_AXI_GPIO_1_DEVICE_ID);
	XGpio_Initialize(&btns, XPAR_AXI_GPIO_2_DEVICE_ID);

	TC1_begin(&myDevice, XPAR_PMODTC1_0_AXI_LITE_SPI_BASEADDR);

	XGpio_SetDataDirection(&lcd_lines, 1, 0x0);
	XGpio_SetDataDirection(&lcd_rs_en, 1, 0x0);
	XGpio_SetDataDirection(&btns, 1, 0x1);

	LCD_Send_Command (0x02); // Initialize 16*2 LCD in 4-bit mode
	LCD_Send_Command (0x28); // Configure LCD in 2-line, 4-bit mode, and 5x8 dots
	LCD_Send_Command (0x0C); // Display ON Cursor OFF
	LCD_Send_Command (0x01); // Clear Display
	LCD_Send_Command (0x06); // Auto Increment Cursor

}

void LCD_Send_Command (unsigned char cmnd) {

	unsigned char un, ln; //Upper Nibble and Lower Nibble

	un = cmnd >> 4;  // Shift right 4 bits and store upper nibble
	ln = cmnd & 0x0F; // Mask and store the lower nibble

	//register select = 0 for sending command

	XGpio_DiscreteWrite(&lcd_rs_en, 1, 0x0); // en=0, rs=0
	XGpio_DiscreteWrite(&lcd_lines, 1, 0x0); // Write 0 to LCD Data Lines

	XGpio_DiscreteWrite(&lcd_lines, 1, un);  // Send upper nibble
	XGpio_DiscreteWrite(&lcd_rs_en, 1, 0x2); // en=1, rs=0

	usleep(2000);
	XGpio_DiscreteWrite(&lcd_rs_en, 1, 0x0); // en=0, rs=0

	XGpio_DiscreteWrite(&lcd_lines, 1, ln);  // Send lower nibble
	XGpio_DiscreteWrite(&lcd_rs_en, 1, 0x2); // en=1, rs=0

	usleep(2000);
	XGpio_DiscreteWrite(&lcd_rs_en, 1, 0x0); // en=0, rs=0

}

void LCD_Write_Data (unsigned char data) {

	unsigned char un, ln; //Upper Nibble and Lower Nibble

	un = data >> 4;  // Shift right 4 bits and store upper nibble
	ln = data & 0x0F; // Mask and store the lower nibble

	//register select = 1 for sending data

	XGpio_DiscreteWrite(&lcd_rs_en, 1, 0x1); // en=0, rs=1
	XGpio_DiscreteWrite(&lcd_lines, 1, 0x0); // Write 0 to LCD Data Lines

	XGpio_DiscreteWrite(&lcd_lines, 1, un);  // Send upper nibble
	XGpio_DiscreteWrite(&lcd_rs_en, 1, 0x3); // en=1, rs=1

	usleep(2000);
	XGpio_DiscreteWrite(&lcd_rs_en, 1, 0x1); // en=0, rs=1

	XGpio_DiscreteWrite(&lcd_lines, 1, ln);  // Send lower nibble
	XGpio_DiscreteWrite(&lcd_rs_en, 1, 0x3); // en=1, rs=1

	usleep(2000);
	XGpio_DiscreteWrite(&lcd_rs_en, 1, 0x1); // en=0, rs=1

}

void LCD_Send_String (char *s, int len) {

	while( len > 0 ) {

		LCD_Write_Data (*s++); // Send each character in the string
		len--;

	}

}

void LCD_Send_Number (int number) {

	int l, temp;
	char buffer[5];

	if(number == 0){

		LCD_Send_String ("0", 1); // Send the number as string

	}

	else {

	itoa(number, buffer, 10); // Convert number to string
	temp = number;

	for(l = 0; temp != 0; temp /= 10, l++); // Find length of the number

	LCD_Send_String (buffer, l); // Send the number as string

	}
}
