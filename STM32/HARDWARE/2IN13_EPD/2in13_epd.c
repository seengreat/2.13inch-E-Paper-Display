/***************************************************************************************
 * Project  :
 * Experimental Platform :STM32F103C8T6 + 2.13inch E-Paper
 * Hardware Connection :STM32F103 -> 2.13inch E-Paper
 *			PB12 --> CS
 *			PB13 --> CLK
 *			PB15 --> MOSI
 *			PA8  --> DC
 *      PA11 --> RST
 *      PA12 --> BUSY
 * Library Version :ST_V3.5
 * Author		   : Andy Li
 * Web Site		   :www.seengreat.com
***************************************************************************************/

#include <string.h>
#include "spi.h"
#include "gpio.h"
#include "delay.h"
#include "usart.h"
#include "2in13_epd.h"
#include "2in13_epd_conf.h"
#include "spi.h"


unsigned char Border_Flag = 0;
  
//***************************************************************//
//LUT_Flag is the call waveform flag

//***************************************************************//
unsigned long LUT_Flag=0;

/****************************
* PB12-----SPI2_NSS
* PB13-----SPI2_SCK
* PB15-----SPI2_MOSI
* PA8------DC
* PA11-----RST
* PA12-----BUSY
***************************/
void Epd_gpio_config(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE );
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;  //PA8-DC  PA11-RST
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  //PA12-BUSY
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_12;  //
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);	  
}
//*******************Epd_init(void)***************************//	
//Initialize the settings. After resetting, you must reinitialize the settings
//******************************************************//				
void Epd_init(void)
{
	
    LUT_Flag = 0;

		Epd_write_cmd(0x00);       
		Epd_write_data(0xFB);//Can change the scanning direction,FB->rotation  F3->left/right F7->up/down
		Epd_write_data(0x89); 
		
		Epd_write_cmd(0x01); 
		Epd_write_data(0x03);    
		Epd_write_data(0x00);    
		Epd_write_data(0x3F);    
		Epd_write_data(0x3F);    
		Epd_write_data(0x03);    

		Epd_write_cmd(0x03);       
		Epd_write_data(0x00);    

		Epd_write_cmd(0x06);       
		Epd_write_data(0x27);    
		Epd_write_data(0x27);    
		Epd_write_data(0x2F);    

		Epd_write_cmd(0x30);       
		Epd_write_data(0x09);   

		Epd_write_cmd(0x60);       
		Epd_write_data(0x22);   

		Epd_write_cmd(0x82);  // vcom
		Epd_write_data(0x00); // 

		Epd_write_cmd(0xE3);        
		Epd_write_data(0x00);    

		Epd_write_cmd(0x41);        
		Epd_write_data(0x00);   
		
		Epd_write_cmd(0x61);    
		Epd_write_data(0x80);     
		Epd_write_data(0x00);   
		Epd_write_data(0xFA);  
  
		Epd_write_cmd(0x65);        
		Epd_write_data(0x00);    
		Epd_write_data(0x00);    
		Epd_write_data(0x00);    

		Epd_write_cmd(0X50);      
		Epd_write_data(0xB7); 
}


//****************EPD_refresh(void)************************//	
//Screen refresh instruction
//******************************************************//
void Epd_refresh(void)
{
		Border_Flag=1;	
		Epd_write_cmd(0x17);			
		Epd_write_data(0xA5);
		Epd_chkstatus();
}	


//***************EPD_sleep(void)************************//	
//enter sleep
//After exiting sleep, it is necessary to reinitialize the settings. 
//The first screen must be fully refresh by GC, and then the screen can be refresh by DU
//******************************************************//
void Epd_sleep(void)
{
		Epd_write_cmd(0X07); //deep sleep
		Epd_write_data(0xA5);
}


//***************lut_GC(void)************************//	
//Call GC waveform:Global Refresh
//***************************************************//
void lut_GC(void)
{
	  unsigned int count;	 
	
		Epd_write_cmd(0x20);							//vcom
		for(count=0;count<56;count++)
			{Epd_write_data(lut_R20_GC[count]);}
		
	  Epd_write_cmd(0x21);							//red not use
	  for(count=0;count<56;count++)
		  {Epd_write_data(lut_R21_GC[count]);}

		Epd_write_cmd(0x24);							//bb b
		for(count=0;count<56;count++)
			{Epd_write_data(lut_R24_GC[count]);}			
			
	 if (LUT_Flag==0)	
	 {
			Epd_write_cmd(0x22);							//bw r
			for(count=0;count<56;count++)
				{Epd_write_data(lut_R22_GC[count]);}

			Epd_write_cmd(0x23);							//wb w
			for(count=0;count<56;count++)
				{Epd_write_data(lut_R23_GC[count]);}
		  LUT_Flag=1;
	 }
	 else
	 {
			Epd_write_cmd(0x22);							//bw r
			for(count=0;count<56;count++)
				{Epd_write_data(lut_R23_GC[count]);}

			Epd_write_cmd(0x23);							//wb w
			for(count=0;count<56;count++)
				{Epd_write_data(lut_R22_GC[count]);}
		  LUT_Flag=0;
	 }
	 			         
}


//***************lut_DU(void)************************//	
//Call DU waveform:Partial refresh
//***************************************************//
void lut_DU(void)
{
	  unsigned int count;	 
	
		Epd_write_cmd(0x20);							//vcom
		for(count=0;count<56;count++)
			{Epd_write_data(lut_R20_DU[count]);}
		
	  Epd_write_cmd(0x21);							//red not use
	  for(count=0;count<56;count++)
		  {Epd_write_data(lut_R21_DU[count]);}

		Epd_write_cmd(0x24);							//bb b
		for(count=0;count<56;count++)
			{Epd_write_data(lut_R24_DU[count]);}			
			
    if (LUT_Flag==0)	
	  {
			Epd_write_cmd(0x22);							//bw r
			for(count=0;count<56;count++)
				{Epd_write_data(lut_R22_DU[count]);}

			Epd_write_cmd(0x23);							//wb w
			for(count=0;count<56;count++)
				{Epd_write_data(lut_R23_DU[count]);}
		  LUT_Flag=1;
	  }
	  else
	  {
			Epd_write_cmd(0x22);							//bw r
			for(count=0;count<56;count++)
				{Epd_write_data(lut_R23_DU[count]);}

			Epd_write_cmd(0x23);							//wb w
			for(count=0;count<56;count++)
				{Epd_write_data(lut_R22_DU[count]);}
		 LUT_Flag=0;
	 }	 			         
}

//***************PIC_display_DU**********************//	
//Call DU waveform to refresh screen
//***************************************************//
void PIC_display_DU(const unsigned char* picData)
{
    unsigned int i;
	
		Epd_write_cmd(0X50);   // 
		Epd_write_data(0xD7);  // Border	
	 
		Epd_write_cmd(0x13);	  //Transfer new data
		for(i=0;i<(EPD_MEM_W*EPD_MEM_H/8);i++)	     
		{
				Epd_write_data(*picData);
				picData++;
		}	
		lut_DU(); 					
		Epd_refresh();						
}

//***************PIC_display_GU**********************//	
//Call GC waveform to refresh screen
//***************************************************//
void PIC_display_GC(const unsigned char* picData)
{
    unsigned int i;
//***********************************************************************//
//Border_Flag refers to the border mark. Only the first screen after initialization will be painted with Border, 
//and other times will not be painted
    if (Border_Flag==1)	
		{
				Epd_write_cmd(0X50);  // 
				Epd_write_data(0xD7); // not Border
		}	
		Epd_write_cmd(0x13);		      
		for(i=0;i<(EPD_MEM_W*EPD_MEM_H/8);i++)	     
		{
				Epd_write_data(*picData);
				picData++;
		}	
		lut_GC(); 				
		Epd_refresh();					
}

//***************Epd_chkstatus***********************//	
//GPIO reads BUSY pin,
//Pin level=L indicates that the screen driver chip is busy and cannot send data and command
//Pin level=H indicates that the screen driver chip is idle and can send data and command
//***************************************************//
void Epd_chkstatus(void)
{
	 while(isEPD_W21_BUSY==0); 
}

void Epd_Reset(void)
{
	EPD_W21_RST=1;
	delay_ms(10);	 //At least 10ms delay 
	EPD_W21_RST=0; // Module reset
	delay_ms(100); //At least 10ms delay 
	EPD_W21_RST=1;
	delay_ms(100); //At least 10ms delay 
}

void Epd_write_cmd(unsigned char command)
{
  EPD_W21_CS=0;                   
	EPD_W21_DC=0;		// command write
	SPI2_WriteByte(command);
	EPD_W21_CS=1;
	EPD_W21_DC=1;		//
}
void Epd_write_data(unsigned char data)
{
  EPD_W21_CS=0;                   
	EPD_W21_DC=1;		// data write
	SPI2_WriteByte(data);
	EPD_W21_CS=1;
	EPD_W21_DC=1;		//
}
