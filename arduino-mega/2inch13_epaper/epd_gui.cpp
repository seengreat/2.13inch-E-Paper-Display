#include <string.h>
#include "epd_2inch13.h"
#include <spi.h>
#include "epd_gui.h"
#include "fonts.h"

IMAGE Image;
void Image_init( uint16_t img_w, uint16_t img_h)
{
    memset(Image.img, 0xff, sizeof(Image.img));
    Image.w = img_w;
    Image.h = img_h;
}

//set the point(x,y) to black
//x:1~250 y:1~122
uint8_t Draw_point(uint16_t x, uint16_t y)
{
   uint8_t temp,bit_offset=0;
   uint16_t byte_offset=0;
   if(x>EPD_DIS_W || y>EPD_DIS_H)
   {
       printf("draw point out of the screen area!");
       return 0;
   }
   y += 6;//128-122=6
   byte_offset = (y-1)/8+(x-1)*16;
   bit_offset = (8-(y-1)%8)-1;
   temp = (uint8_t)(~(1<<(bit_offset)));
   Image.img[byte_offset] &= temp;
   return 1;
}

//clear the whole screen
void Gui_clear(void)
{
   memset(Image.img, 0xff, sizeof(Image.img));
}

//set pixel x:1~250 y:1~122
void Gui_set_pix(uint16_t x, uint16_t y)
{
    if(x>EPD_DIS_W || y>EPD_DIS_H)
    {
        printf("ERR:set_pix point out of screen area!\r\n");
        return ;
    }
    Draw_point( x, y);
}

void Gui_draw_point(uint16_t x, uint16_t y, uint16_t width)
{
    uint16_t xd, yd;
    if(x>EPD_DIS_W || y > EPD_DIS_H)
    {
        printf("ERR:Gui_draw_point out of screen area!\r\n");
        return ;
    }
    for(xd = 0;xd<width; xd++)
    {
        for(yd=0;yd<width; yd++)
        {
            if(x+xd>EPD_DIS_W || y+yd>EPD_DIS_H )
            {
               continue;    
            }
            Gui_set_pix(x+xd, y+yd);     
        }
    }
}
//Bresenham algorithm draw line
void Gui_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t width, LINE_STYLE type)
{
    uint16_t x = x1;
    uint16_t y = y1;
    int dx = x2-x1>=0?x2-x1:x1-x2;
    int dy = y2-y1>=0?y2-y1:y1-y2;
    int p = dx-dy;
    int e2;
    int xdir = x1>x2?-1:1;
    int ydir = y1>y2?-1:1;
    int dot_cnt = 0;
    for(;;)
    {
        dot_cnt++;
        if(type==DOTTED && dot_cnt%3==0)
        {
            Gui_draw_point(x, y, width);
        }
        else if(type==SOLID)
        {
            Gui_draw_point(x, y, width);
        }
        if(x==x2 && y==y2)
        {
            break;
        }
        e2 = 2*p;
        if(e2>= -dy)
        {
            p -= dy;
            x += xdir;
        }
        if(e2<=dx)
        {
            p += dx;
            y += ydir;
        }
    }
}

void Gui_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t width, FILL fill)
{
    uint16_t i;
    if(x1>Image.w || y1 > Image.h || x2 > Image.w || y2 > Image.h)
    {
        printf("ERR:line point out of screen area!\r\n");
        return ;
    } 
    if(fill == FULL)
    {
        for(i=y1;i<y2;i++)
        {
            Gui_draw_line(x1, i, x2, i, width, SOLID);
        }
    }
    else if(fill==EMPTY)
    {
        Gui_draw_line(x1, y1, x2, y1, width, SOLID);
        Gui_draw_line(x1, y1, x1, y2, width, SOLID);
        Gui_draw_line(x1, y2, x2, y2, width, SOLID);
        Gui_draw_line(x2, y1, x2, y2, width, SOLID);
    }
}

//Bresenham  algorithm draw circle
void Gui_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t width, FILL fill)
{
    uint16_t dx=0,dy=r;
    int d = 1-r;
    uint16_t i;
    if(x>EPD_DIS_W || y > EPD_DIS_H)
    {
        printf("ERR:circle center point out of screen area!\r\n");
        return ;
    } 
    while(dy>dx)
    {
        if(fill==EMPTY)
        {
            Gui_draw_point(x+dx, y+dy, width);
            Gui_draw_point(x+dy, y+dx, width);
            Gui_draw_point(x-dx, y+dy, width);
            Gui_draw_point(x-dy, y+dx, width);
            Gui_draw_point(x-dx, y-dy, width);
            Gui_draw_point(x-dy, y-dx, width);
            Gui_draw_point(x+dx, y-dy, width);
            Gui_draw_point(x+dy, y-dx, width);
        }
        else if(fill==FULL)
        {
            for(i=dx;i<=dy;i++)
            {
                Gui_draw_point(x+dx, y+i, width);
                Gui_draw_point(x+i, y+dx, width);
                Gui_draw_point(x-dx, y+i, width);
                Gui_draw_point(x-i, y+dx, width);
                Gui_draw_point(x-dx, y-i, width);
                Gui_draw_point(x-i, y-dx, width);
                Gui_draw_point(x+dx, y-i, width);
                Gui_draw_point(x+i, y-dx, width);                
            }
        }
        if(d<0)
        {
            d += 2*dx+3;    
        }
        else
        {
            d += 2*(dx-dy)+5;
            dy--;
        }
        dx++;
    }
}

void Gui_draw_char(uint16_t x, uint16_t y, const char str_char, FONT* Font)
{
    uint16_t row, col_bit;
    uint32_t offset;
    uint16_t char_nbytes;
    const unsigned char *ptr;

    if (x > EPD_DIS_W || y > EPD_DIS_H) 
    {
        printf("ERR:draw char point out of screen area!\r\n");
        return;
    }
    //the number of bytes occupied by a character
    char_nbytes = Font->h * (Font->w / 8 + (Font->w % 8 ? 1 : 0));
    //the offset of the character
    offset = (str_char - ' ') * char_nbytes;
    ptr = &Font->table[offset];
    for (row = 0; row < Font->h; row ++) 
    {
        for (col_bit = 0; col_bit < Font->w; col_bit++ )
        {
            if (pgm_read_byte(ptr) & (0x80 >> (col_bit % 8)))
                Gui_set_pix(x+col_bit, y+row);            
            if (col_bit % 8 == 7) //next byte
                ptr++;
        }
        if(Font->w % 8 != 0)// next line
            ptr++;
    }
}

void Gui_draw_str(uint16_t x, uint16_t y, const char *str_char, FONT* Font)
{
    uint16_t dx = x,dy = y;
    while(*str_char != '\0')
    {
        if((dx+Font->w)>EPD_DIS_W)//wrap the line if the x direction has reached the boundary
        {
            dx = x;
            dy += Font->h;
        }
        if((dy+Font->h)>EPD_DIS_H)//wrap the line if the y direction has reached the boundary
        {
            dy = y;
            dx += Font->w;
        }
        Gui_draw_char(dx, dy, *str_char++, Font);
        dx += Font->w;
    }

}
