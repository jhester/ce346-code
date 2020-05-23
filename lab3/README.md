Project Lab 3: Display Management
========

In this lab you will integrate the OLED display into your project, building a heads up display with text, and moving sprites / objects on the screen. This will demonstrate low level understanding of communication protocols and timing, and represent the first interaction with a sophisticated peripheral which requires a fairly complex software API.

### Setup

For this lab the supporting code is a bit more sophisticated, as we are using bitmap fonts and images along with a somewhat bare bones OLED / SSD1306 library made for the MSP430FR5994.

To get going, copy these files into a new project, make sure you have setup the display correctly, and then configure the pins inside the `oled.cpp` file, inside the `init_spi` and the `SSD1306::init` functions. You need to make sure the SPI pins, the DC, RST, and CS pins are all setup.