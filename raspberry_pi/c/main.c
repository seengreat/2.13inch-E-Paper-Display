/***************************************************************************************
 * Project  :
 * Experimental Platform :Raspberry Pi 4B + 2.13inch E-Paper Display
 * Hardware Connection :Raspberry Pi 4B -> 2.13inch E-Paper in wiringpi number
 *		CE0  --> CS
 *		SCK  --> CLK
 *		MOSI --> MOSI
 *		P6   --> DC
 *      P0   --> RST
 *      P5 --> BUSY
 * Author		   : Andy Li
 * Web Site		   : www.seengreat.com
***************************************************************************************/
#include <string.h>
#include <stdlib.h>		
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "2in13_epd.h"
#include "2in13_epd_conf.h"
#include "epd_gui.h"
#include "font.h"

/******************************
 * Definition of screen coordinates:
 * 0------------> x (1~250) 
 * |
 * |
 * |
 * |
 * y (1~122)
 * */
 
int main(void)
{
	printf("\r\n-- 2.13inch epaper demo --\r\n");
	printf("Raspberry Pi 4B\r\n");

    Epd_Gpio_config();
	Epd_Reset();//Electronic paper IC reset
	Epd_init(); //EPD init			
    Image_Init(EPD_MEM_W, EPD_MEM_H);
    Gui_Clear();
	PIC_display_GC(Image.img);	
	Gui_Clear();
	Gui_draw_line(1, 1, 250, 1, 2, SOLID);
	Gui_draw_line(1, 1, 1, 122, 1, SOLID);
	Gui_draw_rectangle(190, 8, 205, 28, 1, EMPTY);
	Gui_draw_rectangle(210, 8, 230, 28, 1, FULL);
	Gui_draw_circle(210, 60, 15, 1, FULL);
	Gui_draw_circle(210, 60, 20, 1, EMPTY);
	Gui_draw_str(10, 20, "876543210!", &Font24);
	Gui_draw_str(10, 50, "seengreat!", &Font24);
	Gui_draw_str(10, 80, "SEENGREAT!", &Font24);	
	PIC_display_DU(Image.img);//GC full refresh display screen, it is recommended to conduct full refresh every 10 DU refresh
	delay(1000);
	delay(1000);
	Gui_Clear();	
	Gui_draw_str(45, 5, "SEENGREAT!", &Font24);		
	Gui_draw_str(1, 35, "2inch13", &Font24);
	Gui_draw_str(130, 35, "E-Paper", &Font24);
	Gui_draw_str(1, 65, "250x122 pixels", &Font24);	
	Gui_draw_str(1, 95, "Black", &Font24);	
	Gui_draw_str(92, 95, "and", &Font24);
	Gui_draw_str(150, 95, "White", &Font24);				
	PIC_display_GC(Image.img);//DU full refresh display screen, it is recommended to conduct full refresh every 10 DU refresh
	EPD_sleep();//Enter the sleep mode to lock the image display, otherwise, unexpected stripes may appear on the screen when the power is cut off
	delay(1000);
	delay(1000);
	delay(1000);
	delay(1000);			
			 
}

