#ifndef  __EPD_GUI_H
#define  __EPD_GUI_H 

#include "fonts.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct{
  unsigned char img[4000];
  uint16_t w;
  uint16_t h;
}IMAGE;

typedef enum{
    EMPTY = 0,
    FULL, 
} FILL;

typedef enum{
    SOLID = 0,  //_______________
    DOTTED,     //---------------
} LINE_STYLE;

typedef enum{
    ROUND = 0,             
    SQUAER,     
} DOT_STYLE;

extern IMAGE Image;

void Image_init( uint16_t img_w, uint16_t img_h);
uint8_t Draw_point(uint16_t x, uint16_t y);
void Gui_clear(void);
void Gui_set_pix(uint16_t x, uint16_t y);
void Gui_draw_point(uint16_t x, uint16_t y, uint16_t width);  
void Gui_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t width, LINE_STYLE type);
void Gui_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t width, FILL fill);
void Gui_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t width, FILL fill);
void Gui_draw_char(uint16_t x, uint16_t y, const char str_char, FONT* Font);
void Gui_draw_str(uint16_t x, uint16_t y, const char *str_char, FONT* Font);

#ifdef __cplusplus
}
#endif

#endif
