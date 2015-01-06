/*
PCA9685 library collection
- for controling a 16CH PWM Controller
- for cross compile arduino & linux

1.1.2015 by marc philipp hammermann <marc@marelab.org>

PCA9685 device library code is placed under the MIT license
Copyright (c) 2015 marc philipp hammermann

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>      /* Standard I/O functions */
#include <fcntl.h>
#include <syslog.h>		/* Syslog functionallity */
#include <inttypes.h>
#include <errno.h>
#include <math.h>

#include "PCA9685.h"

//! Constructor takes bus and address arguments
/*!
 \param bus the bus to use in /dev/i2c-%d.
 \param address the device address on bus
 */
PCA9685::PCA9685(char* busname, uint8_t devadr) {
	this->devAddr = devadr;
	i2c = new I2Cdev(busname);
}

PCA9685::~PCA9685() {
	delete i2c;
}
//! Sets PCA9685 mode to 00
void PCA9685::reset() {

		i2c->writeByte(devAddr,MODE1, 0x00); //Normal mode
		i2c->writeByte(devAddr,MODE2, 0x04); //totem pole

}
//! Set the frequency of PWM
/*!
 \param freq desired frequency. 40Hz to 1000Hz using internal 25MHz oscillator.
 */
void PCA9685::setPWMFreq(int freq) {

		uint8_t prescale_val = (CLOCK_FREQ / 4096 / freq)  - 1;
		i2c->writeByte(devAddr,MODE1, 0x10); //sleep
		i2c->writeByte(devAddr,PRE_SCALE, prescale_val); // multiplyer for PWM frequency
		i2c->writeByte(devAddr,MODE1, 0x80); //restart
		i2c->writeByte(devAddr,MODE2, 0x04); //totem pole (default)
}

//! PWM a single channel
/*!
 \param led channel to set PWM value for
 \param value 0-4095 value for PWM
 */
void PCA9685::setPWM(uint8_t led, int value) {
	setPWM(led, 0, value);
}
//! PWM a single channel with custom on time
/*!
 \param led channel to set PWM value for
 \param on_value 0-4095 value to turn on the pulse
 \param off_value 0-4095 value to turn off the pulse
 */
void PCA9685::setPWM(uint8_t led, int on_value, int off_value) {
	i2c->writeByte(devAddr,LED0_ON_L + LED_MULTIPLYER * led , on_value & 0xFF);
	i2c->writeByte(devAddr,LED0_ON_H + LED_MULTIPLYER * led , on_value >> 8);
	i2c->writeByte(devAddr,LED0_OFF_L + LED_MULTIPLYER * led , off_value & 0xFF);
	i2c->writeByte(devAddr,LED0_OFF_H + LED_MULTIPLYER * led , off_value >> 8);
}

int PCA9685::getPWM(uint8_t led){
	int ledval = 0;
	uint8_t value;
	ledval = i2c->readByte(devAddr,LED0_OFF_H + LED_MULTIPLYER * (led-1),&value);
	ledval = ledval & 0xf;
	ledval <<= 8;
	ledval += i2c->readByte(devAddr,LED0_OFF_L + LED_MULTIPLYER * (led-1),&value);
	return ledval;
}
