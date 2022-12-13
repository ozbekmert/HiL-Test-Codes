#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdio.h>      /* Standard I/O functions */
#include <fcntl.h>
#include <syslog.h>		/* Syslog functionallity */
#include <inttypes.h>
#include <errno.h>
#include <math.h>
#include <iostream>

#include "PCA9685.h"
using namespace std;

//! Constructor takes bus and address arguments
/*!
 \param bus the bus to use in /dev/i2c-%d.
 \param address the device address on bus
 */
PCA9685::PCA9685(int bus, int address) {
	i2c = new I2C(bus,address);
	setPWMFreq(1040);
	reset();

}

PCA9685::~PCA9685() {
	delete i2c;
}
//! Sets PCA9685 mode to 00
void PCA9685::reset() {

		i2c->write_byte(MODE1, 0x00); //Normal mode


}
//! Set the frequency of PWM
/*!
 \param freq desired frequency. 40Hz to 1000Hz using internal 25MHz oscillator.
 */
void PCA9685::setPWMFreq(int freq)
{


	  freq *= 0.9;  // Correct for overshoot in the frequency setting (see issue #11).
	  float prescaleval = 25000000;
	  prescaleval /= 4096;
	  prescaleval /= freq;
	  prescaleval -= 1;
	  uint8_t prescale = floor(prescaleval + 0.5);
	  uint8_t oldmode = i2c->read_byte(MODE1);
	  uint8_t newmode = (oldmode&0x7F) | 0x10; // sleep
	  i2c->write_byte(MODE1, newmode); // go to sleep

	  i2c->write_byte(PRE_SCALE, prescale); // set the prescaler
	  usleep(1000);
	  i2c->write_byte(MODE1, oldmode);
	  usleep(1000);
	  i2c->write_byte(MODE1, oldmode | 0xa1);  //  This sets the MODE1 register to turn on auto increment.





}

//! PWM a single channel
/*!
 \param led channel (1-16) to set PWM value for
 \param value 0-4095 value for PWM
 */
void PCA9685::setPWM(uint8_t led, int value) {
	setPWM(led, 0, value);
}
//! PWM a single channel with custom on time
/*!
 \param led channel (1-16) to set PWM value for
 \param on_value 0-4095 value to turn on the pulse
 \param off_value 0-4095 value to turn off the pulse
 */
void PCA9685::setPWM(uint8_t led, int on_value, int off_value)
{
		i2c->write_byte(LED0_ON_L + LED_MULTIPLYER * (led - 1), on_value & 0xFF);
		i2c->write_byte(LED0_ON_H + LED_MULTIPLYER * (led - 1), on_value >> 8);
		i2c->write_byte(LED0_OFF_L + LED_MULTIPLYER * (led - 1), off_value & 0xFF);
		i2c->write_byte(LED0_OFF_H + LED_MULTIPLYER * (led - 1), off_value >> 8);
}

//! Get current PWM value
/*!
 \param led channel (1-16) to get PWM value from
 */
int PCA9685::getPWM(uint8_t led)
{
	int ledval = 0;

	ledval = i2c->read_byte(LED0_OFF_H + LED_MULTIPLYER * (led-1));
	ledval = ledval & 0xf;
	ledval <<= 8;
	ledval += i2c->read_byte(LED0_OFF_L + LED_MULTIPLYER * (led-1));

	return ledval;
}



void PCA9685::writeAnalog(uint8_t led,float voltage)
{

	float percentage =0;



			this->analogMax = 3.285;
			percentage = (voltage)/analogMax;

			int on_value =0;
			int off_value = (percentage*4096-1);

			//int on_value = (percentage*4096-1);
			//int off_value = 4096-on_value;

			cout << "Duty Cycle: " << percentage << " On Value: "<< on_value <<" Off Value: "<< off_value << endl;

			i2c->write_byte(LED0_ON_L + LED_MULTIPLYER * (led - 1), on_value & 0xFF);
			i2c->write_byte(LED0_ON_H + LED_MULTIPLYER * (led - 1), on_value >> 8);

			i2c->write_byte(LED0_OFF_L + LED_MULTIPLYER * (led - 1), off_value & 0xFF);
			i2c->write_byte(LED0_OFF_H + LED_MULTIPLYER * (led - 1), off_value >> 8);





}




