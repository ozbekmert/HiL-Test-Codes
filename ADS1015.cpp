
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

#include "ADS1015.h"
using namespace std;

ADS1015::ADS1015(int bus, int address)
{
	i2c = new I2C(bus,address);


}

ADS1015::~ADS1015()
{
	delete i2c;
}



float ADS1015::Read( int channel )
{
	uint16_t _ret = 0;
	uint32_t ret = 0;
	uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
					  ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
					  ADS1015_REG_CONFIG_CPOL_ACTVHI | // Alert/Rdy active low   (default val)
					  ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
					  ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
					  ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)


	config |= ADS1015_REG_CONFIG_PGA_4_096V;
	config |= ( ADS1015_REG_CONFIG_MUX_SINGLE_0);	//+( channel << 12 ));
	config |= ADS1015_REG_CONFIG_OS_SINGLE;
	config = ( ( config << 8 ) & 0xFF00 ) | ( ( config >> 8 ) & 0xFF );


	i2c->write_byte( ADS1015_REG_POINTER_CONFIG, config );


	usleep( 800000 );

	_ret =i2c->read_byte2( ADS1015_REG_POINTER_CONVERT);

	ret = _ret;
	ret = ( ( ret << 8 ) & 0xFF00 ) | ( ( ret >> 8 ) & 0xFF );
	cout << "ret: " << ret << endl;
	float fret = (float)( ret * 4.096f / 32768.0f );
	cout << "fret: " << fret << endl;
	return fret;
}



