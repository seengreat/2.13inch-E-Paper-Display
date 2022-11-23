#include <string.h>
#include <stdlib.h>		
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include "2in13_epd.h"
#include "2in13_epd_conf.h"

unsigned char fHasRATS = 0;
unsigned char Border_Flag = 0;
  
//***************************************************************//
//LUT_Flag is the call waveform flag

//***************************************************************//
unsigned long LUT_Flag=0;

//define pin conection in wiringpi number
/****************************
* PIN_CS      10 
* PIN_RST     0 
* PIN_DC      6
* PIN_BBUSY   5
***************************/
 
void Epd_Gpio_config(void)
{
	printf("Init gpio for wiringpi\r\n");
    if(wiringPiSetup() < 0)//use wiringpi Pin number
    { 
        printf("wiringPi lib init failed! \r\n");
    } 
    pinMode(PIN_CS, OUTPUT);
    pinMode(PIN_RST, OUTPUT);
    pinMode(PIN_DC, OUTPUT);
    pinMode(PIN_BUSY, INPUT);
    pullUpDnControl(PIN_BUSY, PUD_UP);
    wiringPiSPISetup(0,8000000);  
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
void EPD_refresh(void)
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
void EPD_sleep(void)
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
	EPD_refresh();						
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
	EPD_refresh();					
}

//***************Epd_chkstatus***********************//	
//GPIO reads BUSY pin,
//Pin level=L indicates that the screen driver chip is busy and cannot send data and command
//Pin level=H indicates that the screen driver chip is idle and can send data and command
//***************************************************//
void Epd_chkstatus(void)
{
	while(digitalRead(PIN_BUSY) == LOW); 
}

void Epd_Reset(void)
{
	digitalWrite(PIN_RST, 1);
	delay(10);  //At least 10ms delay 
	digitalWrite(PIN_RST, 0); // Module reset
	delay(100); //At least 10ms delay 
	digitalWrite(PIN_RST, 1);
	delay(100); //At least 10ms delay 
}

void Epd_write_cmd(unsigned char command)
{
    digitalWrite(PIN_CS, 0);                  
	digitalWrite(PIN_DC, 0);		// command write
	wiringPiSPIDataRW(0,&command,1); 
    digitalWrite(PIN_CS, 1);                  
	digitalWrite(PIN_DC, 1);
}
void Epd_write_data(unsigned char data)
{
    digitalWrite(PIN_CS, 0);                  
	digitalWrite(PIN_DC, 1);		// data write
	wiringPiSPIDataRW(0,&data,1); 
    digitalWrite(PIN_CS, 1);                  
	digitalWrite(PIN_DC, 1);
}
