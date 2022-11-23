/***************************************************************************************
 * Project  :
 * Experimental Platform :Arduino Mega 2560 + 2.13inch E-Paper Display
 * Hardware Connection :Arduino Mega 2560-> 2.13inch E-Paper
 *      D53 --> CS
 *      D52 --> CLK
 *      D51 --> MOSI
 *      D8  --> DC
 *      D9  --> RST
 *      D7  --> BUSY
 * Author      : Andy Li
 * Web Site      :www.seengreat.com
***************************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <SPI.h>
#include "epd_2inch13.h"
#include "epd_gui.h"
#include "fonts.h"

/******************************
 * image :
 * 0------------> x (1~250) 
 * |
 * |
 * |
 * |
 * y (1~122)
 * 
 * Pin wiring definion
 * VCC---->5V
 * GND---->GND
 * RST---->D9
 * BUSY--->D7
 * D/C---->D8
 * MOSI--->D51
 * CLK---->D52
 * CS----->D53
 * */

void setup()
{
    Epd_gpio_init();
    Epd_reset();
    Epd_init();
    Gui_clear();
    Image_init(EPD_MEM_W, EPD_MEM_H);
    PIC_display_GC(Image.img);
}

void loop()
{  
    Gui_clear();
    Gui_draw_line(1, 1, 250, 1, 2, SOLID);
    Gui_draw_line(1, 1, 1, 122, 1, SOLID);
    Gui_draw_rectangle(190, 8, 205, 28, 1, EMPTY);
    Gui_draw_rectangle(210, 8, 230, 28, 1, FULL);
    Gui_draw_circle(210, 60, 15, 1, FULL);
    Gui_draw_circle(210, 60, 20, 1, EMPTY);
    Gui_draw_str(10, 20, "876543210!", &Font24);
    Gui_draw_str(10, 50, "seengreat!", &Font24);
    Gui_draw_str(10, 80, "SEENGREAT!", &Font24);  
    PIC_display_DU(Image.img);//Partial  refresh display screen, it is recommended to conduct full refresh every 10 DU refresh
    delay(1000);
    delay(1000);
    Gui_clear();  
    Gui_draw_str(45, 5, "SEENGREAT!", &Font24);     
    Gui_draw_str(1, 35, "2inch13", &Font24);
    Gui_draw_str(130, 35, "E-Paper", &Font24);
    Gui_draw_str(1, 65, "250x122 pixels", &Font24); 
    Gui_draw_str(1, 95, "Black", &Font24);  
    Gui_draw_str(92, 95, "and", &Font24);
    Gui_draw_str(150, 95, "White", &Font24);        
    PIC_display_GC(Image.img);//Globalfull refresh display screen, it is recommended to conduct full refresh every 10 DU refresh
    Epd_sleep();//enter the sleep mode to lock the image display, Otherwise, unexpected stripes may appear on the screen when the power is cut off
    delay(1000);
    delay(1000);   
    delay(1000);
    delay(1000);
    while(1);  
}
