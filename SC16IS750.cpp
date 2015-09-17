/*
  SC16IS750.cpp - Library for interfacing with SC16IS750 i2c to serial and gpio port expander with Arduino
  Created by Eric O'Neill, July 30, 2015
  
*/

#include "SC16IS750.h"
#include "Arduino.h"
#include "Wire.h"

SC16IS750::SC16IS750(int address){
	
	_outputRegVal = 0x00;
	_inputRegVal = 0x00;
	_deviceAddress = address;
}

//-------------------- private functions ---------------------------

byte SC16IS750::readRegister(byte regAddress){
	Wire.beginTransmission(_deviceAddress);
	Wire.write(regAddress);
	Wire.endTransmission();
	Wire.requestFrom(_deviceAddress, 1);
	if(Wire.available()){
		return Wire.read(); // read values in the input register
	}
	else{
		Serial.print("failed to read");
	}

}

void SC16IS750::writeRegister(byte regAddress, byte data){
	Wire.beginTransmission(_deviceAddress);
  	Wire.write(regAddress);
  	Wire.write(data);
 	Wire.endTransmission();
}

void SC16IS750::configureUart(){
	writeRegister(LCR, 0x80); // 0x80 to program baudrate
  	writeRegister(DLL, 0x5F); //0x73 = 9600 with Xtal = 12.288MHz
  	writeRegister(DLM, 0x00); 

  	writeRegister(LCR, 0xBF); // access EFR register
  	//writeRegister(EFR, 0xD0); // enable enhanced registers
  	writeRegister(EFR, 0x10);
  	writeRegister(LCR, 0x03); // 8 data bit, 1 stop bit, no parity
  	//writeRegister(FCR, 0x06); // reset TXFIFO, reset RXFIFO, non FIFO mode
  	//writeRegister(FCR, 0x01); // enable FIFO mode
  	writeRegister(IER, 0x00);
  	writeRegister(FCR, 0x07);  
  	//writeRegister(EFCR, 0x00); 
}

bool SC16IS750::uartConnected(){
	const char TEST_CHARACTER = 'H';
  
 	writeRegister(SPR, TEST_CHARACTER);

 	return (readRegister(SPR) == TEST_CHARACTER);
}

//-------------------- public functions ---------------------------

// must configure pins to be inputs or outputs before using them. Output: 1, Input: 0
void SC16IS750::configurePins(byte pinConfig){
	_pinConfig = pinConfig;
	writeRegister(IODIR, pinConfig);
	writeRegister(IOCTRL, 0x00);
	writeRegister(IOINTMSK, 0xFF);
}

int SC16IS750::writePin(int pin, bool val){
	if((_pinConfig >> pin & 0x01) == 0){
		return -1; //pin config is set to input
	}
	Wire.beginTransmission(_deviceAddress);
	Wire.write(IOSTATE);
	//set bit at pin
	if(val){
		_outputRegVal |= 1 << pin;
	}
	//clear bit at pin
	else{
		_outputRegVal &= ~(1 << pin);
	}
	Wire.write(_outputRegVal);
	if(Wire.endTransmission() == 0){
		return 1;
	}
	else{
		return -1;
	}
}

int SC16IS750::readPin(int pin){
	if(_pinConfig >> pin == 0){
		return -1; // pin configuration is set to output
	}
	Wire.beginTransmission(_deviceAddress);
	Wire.write(IOSTATE);
	Wire.endTransmission();

	Wire.requestFrom(_deviceAddress, 1);
	if(Wire.available()){
		_inputRegVal = Wire.read(); // read values in the input register
	}
	return (_inputRegVal >> pin) & 0x01; // return value from input register corresponding to the pin specified in parameter


}

byte SC16IS750::available() {
  /*
  
    Get the number of bytes (characters) available for reading.

    This is data that's already arrived and stored in the receive
    buffer (which holds 64 bytes).
  
   */
  // This alternative just checks if there's data but doesn't
  // return how many characters are in the buffer:
  //    readRegister(LSR) & 0x01
  return readRegister(RXLVL);
}

byte SC16IS750::txBufferSize(){
	return readRegister(TXLVL);
}

int SC16IS750::read(){
  return readRegister(RHR);
}

void SC16IS750::write(byte value) {
  /*
  
    Write byte to UART.
 
   */
  while (readRegister(TXLVL) == 0) {
    // Wait for space in TX buffer
  };
  writeRegister(THR, value); 
}


