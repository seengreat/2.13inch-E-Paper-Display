import os
import sys
import numpy as np
import epd_2inch13
from PIL import Image, ImageFont

FILL_EMPTY = 0
FILL_FULL  = 1

LINE_SOLID = 0
LINE_DOTTED = 1
FONT_SIZE_16 = 16
FONT_SIZE_24 = 24
FONT_SIZE_28 = 28
class EPD_GUI():
    def __init__(self):
        self.epd = epd_2inch13.EPD_2Inch13()
        self.epd.reset()
        self.epd.epd_init()
        self.img = [0xff for i in range(4000)]

    def set_pixel(self,x,y):
        if x>self.epd.dis_w or y>self.epd.dis_h:
            print("draw point out of the screen area!")
            return 0
        y += 6 #128-122=6
        byte_offset = int((y-1)/8+(x-1)*16)
        bit_offset = int((8-(y-1)%8)-1)
        temp = ~(1<<(bit_offset))
        self.img[byte_offset] &= temp
        return 1
    
    def clear(self):
        for i in range(len(self.img)):
            self.img[i] = 0xff
            
    def draw_point(self,x,y,width):
        if x>self.epd.dis_w or y>self.epd.dis_h:
            print("ERR:Gui_draw_point out of screen area!\r\n")
            return 0
        for xd in range(0,width):
            for yd in range(width):
                if (x+xd)>self.epd.dis_w or (y+yd)>self.epd.dis_h:
                   continue
                self.set_pixel(x+xd, y+yd)

    #Bresenham algorithm draw line
    def draw_line(self,x1, y1, x2, y2, width, line_type):
        x = x1
        y = y1
        dx = x2-x1 if x2-x1>=0 else x1-x2
        dy = y2-y1 if y2-y1>=0 else y1-y2
        p = dx-dy
        
        xdir = -1 if x1>x2 else 1
        ydir = -1 if y1>y2 else 1
        dot_cnt = 0
        while(True):
            dot_cnt += 1
            if line_type==LINE_DOTTED and dot_cnt%3==0:
                self.draw_point(x, y, width)
            elif line_type==LINE_SOLID:
                self.draw_point(x, y, width)
            if x==x2 and y==y2:
                break
            e2 = 2*p
            if e2>= -dy:
                p -= dy
                x += xdir
            if e2<=dx:
                p += dx
                y += ydir
        
    def draw_rectangle(self,x1, y1, x2, y2, width, fill):
        if x1>self.epd.dis_w or y1 > self.epd.dis_h or x2 > self.epd.dis_w or y2 > self.epd.dis_h:
            printf("ERR:line point out of screen area!\r\n")
            return 0
        if fill == FILL_FULL:
            for i in range(y1,y2):
                self.draw_line(x1, i, x2, i, width, LINE_SOLID)
        elif fill==FILL_EMPTY:
            self.draw_line(x1, y1, x2, y1, width, LINE_SOLID)
            self.draw_line(x1, y1, x1, y2, width, LINE_SOLID)
            self.draw_line(x1, y2, x2, y2, width, LINE_SOLID)
            self.draw_line(x2, y1, x2, y2, width, LINE_SOLID)
            
    #Bresenham  algorithm draw circle
    def draw_circle(self, x, y, r, width, fill):
        dx=0
        dy=r
        d = 1-r
        if x>self.epd.dis_w  or y > self.epd.dis_h:
            print("ERR:circle center point out of screen area!\r\n")
            return 0
        while(dy>dx):
            if fill==FILL_EMPTY:
                self.draw_point(x+dx, y+dy, width)
                self.draw_point(x+dy, y+dx, width)
                self.draw_point(x-dx, y+dy, width)
                self.draw_point(x-dy, y+dx, width)
                self.draw_point(x-dx, y-dy, width)
                self.draw_point(x-dy, y-dx, width)
                self.draw_point(x+dx, y-dy, width)
                self.draw_point(x+dy, y-dx, width)
            elif fill==FILL_FULL:
                for i in range(dx,dy):
                    self.draw_point(x+dx, y+i, width)
                    self.draw_point(x+i, y+dx, width)
                    self.draw_point(x-dx, y+i, width)
                    self.draw_point(x-i, y+dx, width)
                    self.draw_point(x-dx, y-i, width)
                    self.draw_point(x-i, y-dx, width)
                    self.draw_point(x+dx, y-i, width)
                    self.draw_point(x+i, y-dx, width)
            if d<0:
                d += 2*dx+3
            else:
                d += 2*(dx-dy)+5
                dy -= 1
            dx += 1
    def draw_str(self, x, y, text_str, font_size, font):
        str_list = list(font.getmask(text=text_str,mode="1"))
        str_size = font.getsize(text=text_str)
        font_act_h = int(len(str_list)/(str_size[0]))
        font_h = font_size
        top_add_row = int((font_h-font_act_h)/2)
        bot_add_row = int((font_h-font_act_h)) - top_add_row 
        for i in range(0,top_add_row*str_size[0]):
            str_list.insert(0,0)
        for i in range(0,bot_add_row*str_size[0]):
            str_list.append(0)
        for i in range(len(str_list)):
            if str_list[i]==0:
                str_list[i] = 1
            else:
                str_list[i] = 0
        n = 0
        for i in range(y-1,y+font_h-1):
            for j in range(x-1,x+str_size[0]-1):
                if str_list[n] == 0:
                    self.set_pixel(j,i)
                n += 1
