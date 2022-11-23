2.13inch E-Paper Display from seengreat:www.seengreat.com
 =======================================
# Instructions
## Product Overview
 This product is a 2.13 inch E-Ink display expansion module. It is designed based on Raspberry Pi 40PIN extension header and is applicable to Raspberry Pi series motherboards. We provide the C and python demo codes of Raspberry Pi, reserve SPI control interfaces to facilitate connect to Arduino, STM32 and other control boards. We also provide Arduino and STM32 demo codes, which can draw dots, lines, rectangles and circles, and display English number characters.<br>
## Product parameters
|-----------|-------------------------------|
|Size	|65mm(Length)*30mm(width)|
|Pixels|	122*250|
|Display Color|	monochrome|
|Voltage Translator|	TXS0108EPWR|
|Signal interface|	SPI|
|Supply voltage|	5V|
|LCD display area|	23.7046mm (W)* 48.55mm (H)|
|Partial Refresh|	Minimum 560ms  (4-wire SPI status)|
|Global Refresh|	Minimum 950ms (4-wire SPI status)|
## Product dimensions
65mm(Length)*30mm(width)<br>
# Usage
## Raspberry Pi demo codes usage
### Hardware interface configuration description
The sample program in the Raspberry Pi motherboard uses the wiringPi pin definitions. The definition of the connection with the Raspberry Pi motherboard is shown in the following table：<br>
|----------------------|----------------------|-----------|----------|
|E-Ink display 	|Pin function	|BCM	|WiringPi|
|VCC|	5V|	3.3V|	3.3V|
|GND|	GND|	GND|	GND|
|BUSY|	P5|	24|	5|
|RSTN|	P0|	17|	0|
|D/C |	P6|	25|	6|
|SDA|	P_MOSI|	10|	12|
|SCL|	P_SCK|	11|	14|
|CSB|	P_CE0|	8|	10|
### Demo Codes Usage	
#### Wiringpi library installation
sudo apt-get install wiringpi<br>
   wget https://project-downloads.drogon.net/wiringpi-latest.deb  # Version 4B upgrade of Raspberry Pi<br>
   sudo dpkg -i wiringpi-latest.deb<br>
   gpio -v # If version 2.52 appears, the installation is successful<br>
#For the Bullseye branch system, use the following command:<br>
git clone https://github.com/WiringPi/WiringPi<br>
cd WiringPi<br>
./build<br>
gpio -v<br>
#Running gpio - v will result in version 2.70. If it does not appear, it indicates an installation error<br>
If the error prompt "ImportError: No module named 'wiringpi'" appears when running the python version of the sample program, run the following command<br>
#For Python 2. x version<br>
pip install wiringpi<br>
 
#For Python version 3. X<br>
pip3 install wiringpi<br>
Note: If the installation fails, you can try the following compilation and installation:<br>
git clone --recursive https://github.com/WiringPi/WiringPi-Python.git<br>
Note: The -- recursive option can automatically pull the submodule, otherwise you need to download it manually.<br>
Enter the WiringPi Python folder you just downloaded, enter the following command, compile and install:<br>
#For Python 2. x version<br>
sudo python setup.py install <br>
#For Python version 3. X<br>
sudo python3 setup.py install<br>
If the following error occurs:<br>
"Error:Building this module requires either that swig is installed<br>
        (e.g.,'sudo apt install swig') or that wiringpi_wrap.c from the<br>
        source distribution (on pypi) is available."<br>
At this time, enter the command sudo apt install swig to install swig. After that, compile and install sudo python3 setup.py install. If a message similar to the following appears, the installation is successful.<br>
"ges<br>
Adding wiringpi 2.60.0 to easy-install.pth file<br>
Installed /usr/local/lib/python3.7/dist-packages/wiringpi-2.60.0-py3.7-linux-armv7<br>
Processing dependencies for wiringpi==2.60.0<br>
Finished processing dependencies for wiringpi==2.60.0"<br>
#### Open SPI interface
sudo raspi-config<br>
Enable SPI interface:<br>
Interfacing Options > SPI > Yes<br>
To view enabled SPI devices:<br>
ls /dev/spi * # The following will be printed: "/dev/spidev0.0" and "/dev/spidev0.1"<br>
#### Installation of python library
The demo codes uses the python 3 environment. To run the python demo codes, you need to install the pil, numpy, and spiderv libraries. Enter the following commands in order to install:<br>
sudo apt-get install python3-pil<br>
sudo apt-get install python3-numpy<br>
sudo apt-get install python3-pip<br>
sudo pip3 install spidev<br>
#### C version demo codes
Enter2.13inch E-Paper Display\demo codes\raspberry_pi\c directory<br>
sudo make clean<br>
sudo make<br>
sudo ./main<br>
After entering the above command, you can observe the E-Ink display.<br>
#### python Version demo codes
Enter 2.13inch E-Paper Display\demo codes\raspberry_pi\python directory<br>
python3 gui_demo.py<br>
After entering the above command, you can observe the E-Ink display.<br>
## Arduino Demo Codes Usage
### Hardware interface configuration description
|----------------------|--------|
|E-Ink display	|Arduino|
|VCC|	3.3V|
|GND|	GND|
|CS|	D53|
|CLK|	D52|
|MOSI|	D51|
|DC|	D8|
|RST|	D9|
|BUSY|	D7|
### Demo Codes Usage
Open 2.13inch E-Paper Display\demo codes\arduino-mega\2inch13_epaper with Arduino IED Click Verify to verify the project file, and then transfer it to the module to observe the E-Ink display.<br>
## STM32 Demo Codes Usage
### Hardware interface configuration description
|----------------------|-------|
|E-Ink display|	STM32|
|VCC|	3.3V|
|GND|	GND|
|CS|	PB12|
|CLK|	PB13|
|MOSI|	PB15|
|DC|	PA8|
|RST|	PA11|
|BUSY|	PA12|
### Demo Codes Usage
Open the routine in directory 2.13inch E-Paper Display\demo codes\STM32 with Keil uVision5 software, compile it correctly, download it to the module, and observe the E-Ink display.<br>
