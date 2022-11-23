
from PIL import ImageDraw, Image, ImageFont
import time
import epd_2inch13
from epd_gui import *

if __name__ == '__main__':
    gui = EPD_GUI()
    gui.clear()
    gui.draw_line(1, 1, 250, 1, 2, LINE_SOLID)
    gui.draw_line(1, 1, 1, 122, 1, LINE_SOLID)
    gui.draw_rectangle(190, 8, 205, 28, 1, FILL_EMPTY)
    gui.draw_rectangle(210, 8, 230, 28, 1, FILL_FULL)
    gui.draw_circle(210, 60, 15, 1, FILL_FULL)
    gui.draw_circle(210, 60, 20, 1, FILL_EMPTY)
    
    gui.epd.display_GC(gui.img)
    time.sleep(2)
    font_16 = ImageFont.truetype("MiSans-Light.ttf",FONT_SIZE_16) # read chinese font file
    font_24 = ImageFont.truetype("MiSans-Light.ttf",FONT_SIZE_24) # read chinese font file
    font_28 = ImageFont.truetype("MiSans-Light.ttf",FONT_SIZE_28) # read chinese font file
    gui.clear()
    gui.draw_line(1, 1, 250, 1, 2, LINE_SOLID)
    gui.draw_line(1, 1, 1, 122, 1, LINE_SOLID)
    gui.draw_rectangle(190, 8, 205, 28, 1, FILL_EMPTY)
    gui.draw_rectangle(210, 8, 230, 28, 1, FILL_FULL)
    gui.draw_circle(210, 60, 15, 1, FILL_FULL)
    gui.draw_circle(210, 60, 20, 1, FILL_EMPTY)
    gui.draw_str(10, 10, "876543210!", FONT_SIZE_24, font_24)
    gui.draw_str(10, 50, "seengreat!", FONT_SIZE_24, font_24)
    gui.draw_str(10, 90, "SEENGREAT!", FONT_SIZE_24, font_24)
    gui.epd.display_DU(gui.img) # GC full refresh display screen, it is recommended to conduct full refresh every 10 DU refresh
    time.sleep(2)
    gui.clear()
    gui.draw_str(45, 5, "SEENGREAT!", FONT_SIZE_24, font_24)
    gui.draw_str(15, 35, "2inch13 E-Paper", FONT_SIZE_28, font_28)
    gui.draw_str(15, 65, "250x122 pixels", FONT_SIZE_28, font_28)
    gui.draw_str(15, 95, "Black and White", FONT_SIZE_28, font_28)
    gui.epd.display_GC(gui.img) # DU full refresh display screen, it is recommended to conduct full refresh every 10 DU refresh
    gui.epd.sleep()
    time.sleep(4)
