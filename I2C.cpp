#include <sys/ioctl.h>
#include <errno.h>
#include <stdio.h>      /* Standard I/O functions */
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <syslog.h>		/* Syslog functionality */
#include "I2C.h"
#include <iostream>


using namespace std;

I2C::I2C(int bus, int address) {
	_i2cbus = bus;
	_i2caddr = address;
	snprintf(busfile, sizeof(busfile), "/dev/i2c-%d", bus);
	openfd();
}

I2C::~I2C() {
	close(fd);
}

uint8_t I2C::read_byte(uint8_t address) {
	if (fd != -1) {
		uint8_t buff[BUFFER_SIZE];
		buff[0] = address;
		if (write(fd, buff, BUFFER_SIZE) != BUFFER_SIZE) {
			syslog(LOG_ERR,
					"I2C slave 0x%x failed to go to register 0x%x [read_byte():write %d]",
					_i2caddr, address, errno);

				cout<< "I2C slave 0x%x failed to go to register 0x%x [read_byte():write %d]"<<endl;
			return (-1);
		} else {
			if (read(fd, dataBuffer, BUFFER_SIZE) != BUFFER_SIZE) {
				syslog(LOG_ERR,
						"Could not read from I2C slave 0x%x, register 0x%x [read_byte():read %d]",
						_i2caddr, address, errno);
			cout<<"Could not read from I2C slave 0x%x, register 0x%x [read_byte():read %d]" <<endl;
				return (-1);
			} else {
				return dataBuffer[0];
			}
		}
	} else {
		syslog(LOG_ERR, "Device File not available. Aborting read");
		cout << "Device File not available. Aborting read" << endl;
		return (-1);
	}

}

uint16_t I2C::read_byte2(uint8_t address) {
	if (fd != -1) {
		uint16_t buff[BUFFER_SIZE];
		buff[0] = address;
		if (write(fd, buff, BUFFER_SIZE) != BUFFER_SIZE) {
			syslog(LOG_ERR,
					"I2C slave 0x%x failed to go to register 0x%x [read_byte():write %d]",
					_i2caddr, address, errno);

				//cout<< "I2C slave 0x%x failed to go to register 0x%x [read_byte():write %d]"<<endl;
			return (-1);
		} else {
			if (read(fd, dataBuffer, BUFFER_SIZE) != BUFFER_SIZE) {
				syslog(LOG_ERR,
						"Could not read from I2C slave 0x%x, register 0x%x [read_byte():read %d]",
						_i2caddr, address, errno);
			//	cout<<"Could not read from I2C slave 0x%x, register 0x%x [read_byte():read %d]" <<endl;
				return (-1);
			} else {
				return dataBuffer[0];
			}
		}
	} else {
		syslog(LOG_ERR, "Device File not available. Aborting read");
		//cout << "Device File not available. Aborting read" << endl;
		return (-1);
	}

}


uint8_t I2C::write_byte(uint8_t address, uint8_t data) {
	if (fd != -1) {
		uint8_t buff[2];
		buff[0] = address;
		buff[1] = data;
		if (write(fd, buff, sizeof(buff)) != 2)
		{
			//syslog(LOG_ERR,
				//	"Failed to write to I2C Slave 0x%x @ register 0x%x [write_byte():write %d]",
				//	_i2caddr, address, errno);
			cout<<"Failed to write to I2C Slave 0x%x @ register 0x%x [write_byte():write %d]" << endl;
			return (-1);
		}
		else {
			syslog(LOG_INFO, "Wrote to I2C Slave 0x%x @ register 0x%x [0x%x]",
				_i2caddr, address, data);

			return (-1);
		}
	} else {
		//syslog(LOG_INFO, "Device File not available. Aborting write");
		cout << "Device File not available. Aborting write" << endl;
		return (-1);
	}
	return 0;
}


//! Open device file for I2C Device
void I2C::openfd() {
	if ((fd = open(busfile, O_RDWR)) < 0) {
		syslog(LOG_ERR, "Couldn't open I2C Bus %d [openfd():open %d]", _i2cbus,errno);
		cout << "Couldn't open I2C Bus %d [openfd():open %d]" << endl;
	}
	if (ioctl(fd, I2C_SLAVE, _i2caddr) < 0) {
		syslog(LOG_ERR, "I2C slave %d failed [openfd():ioctl %d]", _i2caddr,errno);
		cout<<"I2C slave %d failed [openfd():ioctl %d]"<<endl;

	}
}
