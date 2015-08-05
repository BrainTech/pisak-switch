/**
 * Project: AVR ATtiny USB Tutorial at http://codeandlife.com/
 * Author: Joonas Pihlajamaa, joonas.pihlajamaa@iki.fi
 * Inspired by V-USB example code by Christian Starkjohann
 * Copyright: (C) 2012 by Joonas Pihlajamaa
 * License: GNU GPL v3 (see License.txt)
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "usbdrv.h"
#include "usbdrv/usbdrv.h"

#define F_CPU 16500000L
#include <util/delay.h>

#define VENDOR_RQ_READ_BUFFER 0x01

PROGMEM const char usbHidReportDescriptor[52] = { /* USB report descriptor, size must match usbconfig.h */
        0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
        0x09, 0x00,                    // USAGE (Undefined)
        0xa1, 0x01,                    // COLLECTION (Application)
        0x09, 0x01,                    //   USAGE (Pointer)
        0xA1, 0x00,                    //   COLLECTION (Physical)
        0x05, 0x09,                    //     USAGE_PAGE (Button)
        0x19, 0x01,                    //     USAGE_MINIMUM
        0x29, 0x03,                    //     USAGE_MAXIMUM
        0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
        0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
        0x95, 0x03,                    //     REPORT_COUNT (3)
        0x75, 0x01,                    //     REPORT_SIZE (1)
        0x81, 0x02,                    //     INPUT (Data,Var,Abs)
        0x95, 0x01,                    //     REPORT_COUNT (1)
        0x75, 0x05,                    //     REPORT_SIZE (5)
        0x81, 0x03,                    //     INPUT (Const,Var,Abs)
        0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
        0x09, 0x30,                    //     USAGE (X)
        0x09, 0x31,                    //     USAGE (Y)
        0x09, 0x38,                    //     USAGE (Wheel)
        0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
        0x25, 0x7F,                    //     LOGICAL_MAXIMUM (127)
        0x75, 0x08,                    //     REPORT_SIZE (8)
        0x95, 0x03,                    //     REPORT_COUNT (3)
        0x81, 0x06,                    //     INPUT (Data,Var,Rel)
        0xC0,                          //   END_COLLECTION
        0xC0,                          // END COLLECTION
};

static uchar replyBuf[16] = "Hello, USB!";
static uchar dataReceived = 0, dataLength = 0; // for USB_DATA_IN

static uchar reportBuffer = 0;
static uchar currentPosition, bytesRemaining, idleRate;

// this gets called when custom control message is received
usbMsgLen_t usbFunctionSetup(uchar data[8]) {
    usbRequest_t *rq = (void *)data;

    switch(rq->bRequest){
        case VENDOR_RQ_READ_BUFFER:
            currentPosition = 0;                // initialize position index
            bytesRemaining = rq->wLength.word;  // store the amount of data requested
            return USB_NO_MSG;                          // tell driver to use usbFunctionRead()
    }

    return 0; // by default don't return any data
}

// This gets called when data is sent from PC to the device
USB_PUBLIC uchar usbFunctionWrite(uchar *data, uchar len) {
	uchar i;
			
	for(i = 0; dataReceived < dataLength && i < len; i++, dataReceived++)
		replyBuf[dataReceived] = data[i];
		
    return (dataReceived == dataLength); // 1 if we received it all, 0 if not
}

uchar getData(uchar position){
    return 0;
}

uchar usbFunctionRead(uchar *data, uchar len)
{
    uchar i;
    if(len > bytesRemaining)                // len is max chunk size
        len = bytesRemaining;               // send an incomplete chunk
    bytesRemaining -= len;
    for(i = 0; i < len; i++)
        data[i] = getData(currentPosition); // copy the data to the buffer
    return len;                             // return real chunk size
}

void pinInit() {
    DDRB |= _BV(PB0); // PB0 as input
    PORTB |= _BV(PB0);
    DDRB &= ~_BV(PB1); //PB1 as output
    PORTB &= ~_BV(PB1);
}

uchar getInterruptData(uchar *pos) {
    wdt_reset();
    if (PINB ^ _BV(PB0)) {
        PORTB &= ~_BV(PB1);
        *pos = 25;
        return 1;
    }
    else {
        return 0;
    }
}

int main() {
	uchar i;

	pinInit();
	
    wdt_enable(WDTO_1S); // enable 1s watchdog timer

    usbInit();
	
    usbDeviceDisconnect(); // enforce re-enumeration
    for(i = 0; i<250; i++) { // wait 500 ms
        wdt_reset(); // keep the watchdog happy
        _delay_ms(2);
    }
    usbDeviceConnect();
	
    sei(); // Enable interrupts after re-enumeration

    while(1) {
        wdt_reset(); // keep the watchdog happy
        usbPoll();

        if(usbInterruptIsReady()) {
            uchar *p;
            p = 0;
            uchar len = getInterruptData(p); // obtain chunk of max 8 bytes
            PORTB |= _BV(PB1);
            if(len > 0) {                         // only send if we have data
                usbSetInterrupt(p, len);
            }
        }


    }
	
    return 0;
}
