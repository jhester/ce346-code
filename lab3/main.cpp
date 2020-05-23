#include <msp430.h> 
#include "oled.h"
#include <stdint.h>

// This is how we can include bitmap images
#include "images/heart.h"

// Configure MSP430 into fast mode
void init_clocks() {
    // Startup clock system with max DCO setting ~8MHz
    CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    CSCTL1 = DCOFSEL_3 | DCORSEL;           // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // Set all dividers
    CSCTL0_H = 0;                           // Lock CS registers
}

/**
 * main.c
 */
SSD1306 display;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                              // to activate previously configured port settings
    P1DIR |= 0x01;                          // Set P1.0 to output direction
    init_clocks();

    display.init();
    display.clear_off();

    display.drawFastVLineInternal(10, 10, 45, SSD1306_WHITE);
    display.drawFastHLineInternal(10, 10, 20, SSD1306_WHITE);
    display.draw_rect(50, 30, 10, 10, SSD1306_WHITE);
    display.fill_rect(20, 30, 9, 9, SSD1306_WHITE);

    //draw_fast_h_line(0, 30, 120, SSD1306_WHITE);
    display.set_xy(0, 0, SSD1306_WHITE);
    display.set_xy(0, 63, SSD1306_WHITE);
    display.set_xy(127, 0, SSD1306_WHITE);
    display.set_xy(126, 63, SSD1306_WHITE);
    display.setFont(FONT_SMALL);
    char *txt = "Hello 346!";
    display.drawText(42, 3, txt, SSD1306_WHITE);
    display.drawImage(heart_image, 99, 20);
    display.refresh();

    while(1) {
        // Main loop here?
        // Do something fun?
        //display_refresh();
        __delay_cycles(10000);
    }
}
