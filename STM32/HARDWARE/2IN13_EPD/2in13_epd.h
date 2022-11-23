#ifndef __2INCH13_EPD_H
#define	__2INCH13_EPD_H	
#include "sys.h"

/****************************
* PB12-----SPI2_NSS
* PB13-----SPI2_SCK
* PB15-----SPI2_MOSI
* PA8------DC
* PA11-----RST
* PA12-----BUSY
***************************/

#define EPD_MEM_H    128  
#define EPD_MEM_W    250  

#define EPD_DIS_H    122  
#define EPD_DIS_W    250

#define	EPD_W21_CS      PBout(12)
#define	EPD_W21_DC      PAout(8)
#define	EPD_W21_RST     PAout(11)
#define	isEPD_W21_BUSY  PAin(12)

#define	TIMER_TEST_PIN  PBout(8) 

extern unsigned long LUT_Flag;
extern unsigned char Border_Flag;
//////////////////////////////////////full screen update LUT////////////////////////////////////////////



void Epd_gpio_config(void);
void Epd_write_cmd(unsigned char command);
void Epd_write_data(unsigned char data);
void lut_GC(void);//?????
void lut_DU(void);//?????
void Epd_init(void);//????
void PIC_display_GC(const unsigned char* picData);//????
void PIC_display_DU(const unsigned char* picData);//????
void Epd_sleep(void);//????
void Epd_refresh(void);//????
void Epd_chkstatus(void);//????????
void Epd_Reset(void);//????	




#endif

