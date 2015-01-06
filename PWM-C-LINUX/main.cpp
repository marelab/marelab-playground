/*
PCA9685 command line tool
- setting the PWM Channels 0-15
- setting of frequenz
- additional loop function for vary the pwm value
- linux x86 and raspberry pi usage

usage:
     pcapwm16ch -b <i2c bus name ex. '/dev/i2c-1'>
                -a <i2c adress>
                -c <channel 0..15> set channel
                -p <pwm 0..4095> set pwm
                -g <channel 0..15>get pwm of channel
                -f <frequenz 0..10000>set pwm frequenzy
                -l <loop 0..4095>sets the max value of a endless loop
                -d <delay in ms> delay between each loop
                -s <width> step width of a loop

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
#include "stdint.h"
#include "PCA9685.h"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>
using namespace std;

#define RASPBERRY_PI_I2C    "/dev/i2c-1"
#define BANANA_PI_I2C       "/dev/i2c-2"
#define LINUX       		"/dev/i2c-10"
#define ARDUINO				" "

#define I2C_DEV              RASPBERRY_PI_I2C


int main( int argc, char **argv )
{


	cout << "marelab pwm test" << endl;
	cout << "................................................" << endl;


    int pwmchannel = 0;
	int pwmvalue = 0;
	int errparm = 0;
	int pwmaddress = 0x41;
	int pwmfrequenz = 100;
	int pwmloopmax = -1;
	int pwmdelay = -1;
	int pwmstep = 1;
	char *i2cbusname;
	int minparameter = 0;
	int c;
	while ((c = getopt(argc, argv, "b:a:c:p:g:f:l:d:s:")) != -1) {
		switch (c) {
		case 'b': // set i2c bus
			 if (optarg)
				 i2cbusname = optarg;
			     minparameter = minparameter +1;
			 break;
		case 'a': // set adress
			if (optarg)
				minparameter = minparameter +1;
				pwmaddress = std::atoi(optarg);
			break;
		case 'c': // set channel pwm
			if (optarg)
				minparameter = minparameter +1;
				pwmchannel = std::atoi(optarg);
			break;
		case 'p': // set pwm of channel
			if (optarg)
				pwmvalue = std::atoi(optarg);
			break;
		case 'g': // get pwm of channel
			if (optarg)
				pwmchannel = std::atoi(optarg);
			break;
		case 'f': // set pwm frequenz
			if (optarg){
				pwmfrequenz = std::atoi(optarg);
			}
			break;
		case 'l': // loop max value
			if (optarg) {
				pwmloopmax = std::atoi(optarg);
			}
			break;
		case 'd': // delay between each loop
			if (optarg) {
				pwmdelay = std::atoi(optarg);
			}
			break;
		case 's': // step width of a loop
			if (optarg) {
				pwmstep = std::atoi(optarg);
			}
			break;
		default:
			errparm = 1;
			break;
		}
	}


	if ((errparm == 1)||(minparameter != 3)) {
		std::cout << "parameters missing ...." << endl;
		std::cout << "call pcapwm16ch -b <i2c bus name ex. '/dev/i2c-1'>"<< '\n';
		std::cout << "                -a <i2c adress>"<< '\n';
		std::cout << "                -c <channel 0..15> set channel" << '\n';
		std::cout << "                -p <pwm 0..4095> set pwm" << '\n';
		std::cout << "                -g <channel 0..15>get pwm of channel" << '\n';
		std::cout << "                -f <frequenz 0..10000>set pwm frequenzy" << '\n';
		std::cout << "                -l <loop 0..4095>sets the max value of a endless loop" << '\n';
		std::cout << "                -d <delay in ms> delay between each loop" << '\n';
		std::cout << "                -s <width> step width of a loop" << '\n';
	} else {
		PCA9685 pwm(i2cbusname,65);
		pwm.setPWMFreq(pwmfrequenz);
		std::cout << "freq    : " << pwmfrequenz << '\n';
		std::cout << "adress  : " << pwmaddress << '\n';
		std::cout << "channel : " << pwmchannel << '\n';
		std::cout << "pwmvalue: " << pwmvalue << '\n';
		std::cout << "pwmstep : " << pwmstep << '\n';

		pwm.setPWM(pwmchannel,pwmvalue);
		if (pwmloopmax != -1) // we do a loop
		{
			std::cout << "endless loop mode... " <<'\n';
			while(1==1)
			{
				for (int v = 0; v <= pwmloopmax;v=v+pwmstep)
				{
					pwm.setPWM(pwmchannel,v);
					std::cout << "pwmvalue: " << v << '\n';
					usleep(pwmdelay);
				}
			}
		}

	}
    return 0;
}
