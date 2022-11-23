#ifndef __2INCH13_EPD_H
#define	__2INCH13_EPD_H	
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define EPD_MEM_H    128  
#define EPD_MEM_W    250  

#define EPD_DIS_H    122  
#define EPD_DIS_W    250

//define pin conection in wiringpi number
#define PIN_CS      10   
#define PIN_RST     0   
#define PIN_DC      6   
#define PIN_BUSY    5 

extern unsigned long LUT_Flag;
extern unsigned char Border_Flag;
//////////////////////////////////////full screen update LUT////////////////////////////////////////////



void Epd_Gpio_config(void);
void Epd_write_cmd(unsigned char command);
void Epd_write_data(unsigned char data);
void lut_GC(void);//?????
void lut_DU(void);//?????
void Epd_init(void);//????
void PIC_display_GC(const unsigned char* picData);//????
void PIC_display_DU(const unsigned char* picData);//????
void EPD_sleep(void);//????
void EPD_refresh(void);//????
void Epd_chkstatus(void);//????????
void Epd_Reset(void);//????	




#endif

