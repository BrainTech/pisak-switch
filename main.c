#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "usbdrv.h"
#include "usbdrv/usbdrv.h"

#define F_CPU 16500000L
#include <util/delay.h>

PROGMEM const char usbHidReportDescriptor[34] = { /* USB report descriptor, size must match usbconfig.h */
        0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
        0x09, 0x00,                    // USAGE (Undefined)
        0xa1, 0x01,                    // COLLECTION (Application)
        0x09, 0x00,                    //   USAGE (Undefined)
        0xA1, 0x00,                    //   COLLECTION (Physical)
        0x05, 0x09,                    //     USAGE_PAGE (Button)
        0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
        0x29, 0x01,                    //     USAGE_MAXIMUM (Button 1)
        0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
        0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
        0x95, 0x01,                    //     REPORT_COUNT (1)
        0x75, 0x01,                    //     REPORT_SIZE (1)
        0x81, 0x02,                    //     INPUT (Data,Var,Abs)
        0x95, 0x01,                    //     REPORT_COUNT (1)
        0x75, 0x07,                    //     REPORT_SIZE (7)
        0x81, 0x03,                    //     INPUT (Const,Var,Abs)
        0xC0,                          //   END_COLLECTION
        0xC0,                          // END COLLECTION
};


usbMsgLen_t usbFunctionSetup(uchar data[8]) {
    /* Function which is never used but is required by V-USB. */
    return USB_NO_MSG;
    }

void getInterruptData(uchar *data) {
    /* Reads data from PINB0. Puts 0 into data                *
     * if the button is not pressed and 1 otherwise.          */
    wdt_reset();
    if (PINB & (1<<PB0)) {
        *data = 0;
    }
    else {
        *data = 1;
    }
}

int main() {
	uchar i;
    DDRB |= _BV(PB0);       // set PB0 as input
    PORTB |= _BV(PB0);      // set pull-up for PB0
	
    wdt_enable(WDTO_1S);
    usbInit();
    usbDeviceDisconnect();
    for(i = 0; i<250; i++) {
        wdt_reset();
        _delay_ms(2);
    }
    usbDeviceConnect();
    sei();

    while(1) {
        wdt_reset();
        usbPoll();

        if(usbInterruptIsReady()) {
            uchar *interrupt_data;
            interrupt_data = 0;
            getInterruptData(interrupt_data);
            usbSetInterrupt(interrupt_data, 1);
        }
    }
	
    return 0;
}
