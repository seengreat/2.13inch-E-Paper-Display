# Seengreat 2.13 Inch E-Paper Display demo
# Author(s):Andy Li from Seengreat

import os
import sys
import spidev
import time
import wiringpi as wpi
import numpy as np
"""the following pin definiting use wiringpi"""
PIN_CS     = 10
PIN_DC     = 6
PIN_BUSY   = 5
PIN_RST    = 0
# DU waveform

#GC refresh :800ms
lut_R20_GC = [ 0x01,0x00,0x14,0x14,0x01,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00]
lut_R21_GC = [ 0x01,0x60,0x14,0x14,0x01,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00]
lut_R22_GC = [ 0x01,0x60,0x14,0x14,0x01,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00]
lut_R23_GC = [ 0x01,0x90,0x14,0x14,0x01,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00]
lut_R24_GC = [ 0x01,0x90,0x14,0x14,0x01,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00]

#DU refreah: 400ms
lut_R20_DU = [ 0x01,0x00,0x14,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00]
lut_R21_DU = [ 0x01,0x20,0x14,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00]
lut_R22_DU = [ 0x01,0x80,0x14,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00]
lut_R23_DU = [ 0x01,0x40,0x14,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00]
lut_R24_DU = [ 0x01,0x00,0x14,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
               0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00]

class EPD_2Inch13():
    def __init__(self):
        # gpio init
        wpi.wiringPiSetup()
        wpi.pinMode(PIN_DC, wpi.OUTPUT)  # D/C pin
        wpi.pinMode(PIN_RST, wpi.OUTPUT)  # reset pin
        wpi.pinMode(PIN_CS, wpi.OUTPUT)  # cs pin
        wpi.pinMode(PIN_BUSY, wpi.INPUT)  # BUSY pin
        # spi init
        self.bus = 0
        self.dev = 0
        self.spi_speed = 8000000
        self.spi = spidev.SpiDev()
        self.spi.open(self.bus, self.dev)
        self.spi.max_speed_hz = self.spi_speed
        self.spi.mode = 0b00

        self.mem_w = 250
        self.mem_h = 128
        
        self.dis_w = 250
        self.dis_h = 122
        
        self.Border_flag = 0
        self.LUT_lfag = 0
        
    def write_cmd(self, cmd):
        """write command"""
        wpi.digitalWrite(PIN_DC,wpi.LOW)
        self.spi.writebytes([cmd])
        
    def write_data(self, value):
        """write data"""
        wpi.digitalWrite(PIN_DC,wpi.HIGH)
        self.spi.writebytes([value])
        
    def chkstatus(self):
        while wpi.digitalRead(PIN_BUSY)==wpi.LOW:
            pass
        
    def reset(self):
        """reset the epd"""
        wpi.digitalWrite(PIN_RST,wpi.HIGH)
        time.sleep(0.01)
        wpi.digitalWrite(PIN_RST,wpi.LOW)
        time.sleep(0.1)
        wpi.digitalWrite(PIN_RST,wpi.HIGH)
        time.sleep(0.1)
        
    def epd_init(self):
        """epd init..."""
        self.LUT_flag = 0
        
        self.write_cmd(0x00)
        self.write_data(0xFB)
        self.write_data(0x89)
        
        self.write_cmd(0x01)      
        self.write_data(0x03)
        self.write_data(0x00)
        self.write_data(0x3F)
        self.write_data(0x3F)
        self.write_data(0x03)        

        self.write_cmd(0x03)
        self.write_data(0x00)
        
        self.write_cmd(0x06)
        self.write_data(0x27)
        self.write_data(0x27)
        self.write_data(0x2F)
        
        self.write_cmd(0x30) 
        self.write_data(0x09)
        
        self.write_cmd(0x60) 
        self.write_data(0x22)
        
        self.write_cmd(0x82) 
        self.write_data(0x00)
        
        self.write_cmd(0xE3) 
        self.write_data(0x00)
        
        self.write_cmd(0x41) 
        self.write_data(0x00)
        
        self.write_cmd(0x61) 
        self.write_data(0x80)
        self.write_data(0x00)
        self.write_data(0xFA)
        
        self.write_cmd(0x65) 
        self.write_data(0x00)
        self.write_data(0x00)
        self.write_data(0x00)
        
        self.write_cmd(0x50) 
        self.write_data(0xB7)

    def refresh(self):
        self.Border_flag = 1
        self.write_cmd(0x17) 
        self.write_data(0xA5)
        self.chkstatus()
        
    def sleep(self):
        self.write_cmd(0x07)
        self.write_data(0xA5)
        
    #Call GC waveform:Global Refresh
    def lut_GC(self):
        self.write_cmd(0x20) # vcom
        for count in range(0,56):
            self.write_data(lut_R20_GC[count])
        self.write_cmd(0x21)# red not use
        for count in range(0,56):
            self.write_data(lut_R21_GC[count])
        self.write_cmd(0x24) # bb b
        for count in range(0,56):
            self.write_data(lut_R24_GC[count])

        if self.LUT_flag==0:
            self.write_cmd(0x22) #bw r
            for count in range(0,56):
                self.write_data(lut_R22_GC[count])
            self.write_cmd(0x23) #wb w
            for count in range(0,56):
                self.write_data(lut_R23_GC[count])
            self.LUT_flag=1
        else:
            self.write_cmd(0x22) # bw r
            for count in range(0,56):
                self.write_data(lut_R23_GC[count])
            self.write_cmd(0x23) #wb w
            for count in range(0,56):
                self.write_data(lut_R22_GC[count])
            self.LUT_flag=0
            
    #Call DU waveform:Partial Refresh
    def lut_DU(self):
        self.write_cmd(0x20) # vcom
        for count in range(0,56):
            self.write_data(lut_R20_DU[count])
        self.write_cmd(0x21)# red not use
        for count in range(0,56):
            self.write_data(lut_R21_DU[count])
        self.write_cmd(0x24) # bb b
        for count in range(0,56):
            self.write_data(lut_R24_DU[count])

        if self.LUT_flag==0:
            self.write_cmd(0x22) #bw r
            for count in range(0,56):
                self.write_data(lut_R22_DU[count])
            self.write_cmd(0x23) #wb w
            for count in range(0,56):
                self.write_data(lut_R23_DU[count])
            self.LUT_flag=1
        else:
            self.write_cmd(0x22) # bw r
            for count in range(0,56):
                self.write_data(lut_R23_DU[count])
            self.write_cmd(0x23) #wb w
            for count in range(0,56):
                self.write_data(lut_R22_DU[count])
            self.LUT_flag=0
            
    #Call DU waveform to refresh screen
    def display_DU(self,picData):
        self.write_cmd(0X50)
        self.write_data(0xD7)# Border
        self.write_cmd(0x13)#Transfer new data
        for i in range(0,int(self.mem_w*self.mem_h/8)):
            self.write_data(picData[i])
        self.lut_DU()
        self.refresh()

    #Call GC waveform to refresh screen
    def display_GC(self, picData):
        '''Border_Flag refers to the border mark. Only the first screen after initialization
        will be painted with Border,and other times will not be painted'''
        if self.Border_flag==1:
            self.write_cmd(0X50)
            self.write_data(0xD7) # not Border
        self.write_cmd(0x13)
        for i in range(0,int(self.mem_w*self.mem_h/8)):
            self.write_data(picData[i])
        self.lut_GC()
        self.refresh()


    