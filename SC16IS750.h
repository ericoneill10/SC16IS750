
/*
  SC16IS750.h - Library for interfacing with SC16IS750 i2c to serial and gpio port expander with Arduino
  Created by Eric O'Neill, July 30, 2015
  
*/

#ifndef SC16IS750_H
#define SC16IS750_H

#include <Wire.h>
#include "Arduino.h"

#define THR        0x00 << 3
#define RHR        0x00 << 3
#define IER        0x01 << 3
#define FCR        0x02 << 3
#define IIR        0x02 << 3
#define LCR        0x03 << 3
#define MCR        0x04 << 3
#define LSR        0x05 << 3
#define MSR        0x06 << 3
#define SPR        0x07 << 3
#define TXLVL      0x08 << 3
#define RXLVL      0x09 << 3
#define DLAB       0x80 << 3
#define IODIR      0x0A << 3
#define IOSTATE    0x0B << 3
#define IOINTMSK   0x0C << 3
#define IOCTRL     0x0E << 3
#define EFCR       0x0F << 3

#define DLL        0x00 << 3
#define DLM        0x01 << 3
#define EFR        0x02 << 3
#define XON1       0x04 << 3  
#define XON2       0x05 << 3
#define XOFF1      0x06 << 3
#define XOFF2      0x07 << 3


// See Chapter 11 of datasheet
#define SPI_READ_MODE_FLAG 0x80

class SC16IS750 {
public:

	//i2c to uart functions
	SC16IS750(int address);
	//void begin();
    byte available();
    byte txBufferSize();
    int read();
    void write(byte value);
    void write(const char *str);

    //gpio expander functions
    void configurePins(byte pinConfig);
    int writePin(int pin, bool val);
    int readPin(int pin);
    //int readPin(int pin);
    void configureUart();  
    bool uartConnected();

private:
	byte _deviceAddress;
	byte _outputRegVal;
	byte _inputRegVal;
	byte _pinConfig;

	void writeRegister(byte registerAddress, byte data);
    byte readRegister(byte registerAddress);
    void initUart();
    
};

#endif