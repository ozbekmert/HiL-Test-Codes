// System includes
#include <iostream>
#include <unistd.h>
#include <string>
#include <stdlib.h>

#include <sstream>
// XMLR-RPC Includes
#include <xmlrpc-c/girerr.hpp>

// Local includes
#include "RPCHandler.hpp"
#include "Server.hpp"
#include "errno.h"
#include"PWM.h"
#include"GPIO.h"
#include "PCA9685.h"
#include "ADS1015.h"
#include "TempVoltLib.h"


// Declare which objects from std we're using.
using std::cout;
using std::cerr;
using std::endl;

// The RPC handlers may need to access the Server.
using cs4513::Server;
using namespace exploringBB;
using namespace liba;
// Define the RPC handler classes to be in the cs4513 namespace
using namespace cs4513;



void setSensorTemp::execute(xmlrpc_c::paramList const& params, xmlrpc_c::value* const retvalP)
{

	 bool check = true;
	 int bus =2;
	 int addr= 0x40;  // Default address of PCA9658 is 0x40. Please refer its Datasheet
	 PCA9685 PCA9685_MOD(bus,addr);
	 TempVoltLib lib;
	 int height = 2;
	 int width = 450;
	 float** temps = lib.readTempValue(height,width);



	 string sensorName ="";
	 string sensorTemp ="";

	 std::stringstream convert;
	 int val;

	 for(int i = 0; i<8; i++)
	 {
		  sensorName = (string)params.getString(0);

		  sensorTemp = (string)params.getString(1);


		float lol = atof(sensorTemp.c_str());

		if(sensorName == "S1" && check)
		{
			 convert << sensorTemp;
			 convert >> val;
     	     convert.str(""); // clear the stringstream
	         convert.clear(); // clear the state flags for another conversion

			PCA9685_MOD.writeAnalog(1,lib.tempToVoltage(temps,450,lol)); //S1
			cout<<"Temperature is: "<<lol<<endl;
			usleep(500000);
			check = false;

		}
		else if( sensorName == "S2" && check)
		{
			 convert << sensorTemp;
			 convert >> val;
    	     convert.str(""); // clear the stringstream
	         convert.clear(); // clear the state flags for another conversion

			PCA9685_MOD.writeAnalog(2,lib.tempToVoltage(temps,450,lol)); //S1
			cout<<"Temperature is: "<<lol<<endl;
			usleep(500000);
			check = false;

		}
		else if( sensorName == "S3" && check)
		{
			 convert << sensorTemp;
			 convert >> val;
    	     convert.str(""); // clear the stringstream
	         convert.clear(); // clear the state flags for another conversion

			PCA9685_MOD.writeAnalog(3,lib.tempToVoltage(temps,450,lol)); //S1
			cout<<"Temperature is: "<<lol<<endl;
			usleep(500000);
			check = false;


		}
		else if( sensorName == "S4" && check)
		{
			 convert << sensorTemp;
			 convert >> val;
    	     convert.str(""); // clear the stringstream
	         convert.clear(); // clear the state flags for another conversion

			PCA9685_MOD.writeAnalog(4,lib.tempToVoltage(temps,450,lol)); //S1
			cout<<"Temperature is: "<<lol<<endl;
			usleep(500000);
			check = false;


		}
		else if( sensorName == "S5" && check)
		{
			 convert << sensorTemp;
			 convert >> val;
    	     convert.str(""); // clear the stringstream
	         convert.clear(); // clear the state flags for another conversion

			PCA9685_MOD.writeAnalog(5,lib.tempToVoltage(temps,450,lol)); //S1
			cout<<"Temperature is: "<<lol<<endl;
			usleep(500000);
			check = false;

		}
		else if( sensorName  == "S6" && check)
		{
			 convert << sensorTemp;
			 convert >> val;
    	     convert.str(""); // clear the stringstream
	         convert.clear(); // clear the state flags for another conversion

			PCA9685_MOD.writeAnalog(6,lib.tempToVoltage(temps,450,lol)); //S1
			cout<<"Temperature is: "<<lol<<endl;
			usleep(500000);
			check = false;

		}
		else if( sensorName  == "S7" && check)
		{
			 convert << sensorTemp;
			 convert >> val;
    	     convert.str(""); // clear the stringstream
	         convert.clear(); // clear the state flags for another conversion

			PCA9685_MOD.writeAnalog(7,lib.tempToVoltage(temps,450,lol)); //S1
			cout<<"Temperature is: "<<lol<<endl;
			usleep(500000);
			check = false;

		}
		else if( sensorName == "S8" && check )
		{
			 convert << sensorTemp;
			 convert >> val;
    	     convert.str(""); // clear the stringstream
	         convert.clear(); // clear the state flags for another conversion

			PCA9685_MOD.writeAnalog(8,lib.tempToVoltage(temps,450,lol)); //S1
			cout<<"Temperature is: "<<lol<<endl;
			usleep(500000);
			check = false;

		}
		else if( check )
		{
			 *retvalP = xmlrpc_c::value_boolean(false);
		   // executes when the none of the above condition is true.
		}

	}
	 *retvalP = xmlrpc_c::value_boolean(true);

}

void setSensorHum::execute(xmlrpc_c::paramList const& params, xmlrpc_c::value* const retvalP)
{


}

void getRelay::execute(xmlrpc_c::paramList const& params, xmlrpc_c::value* const retvalP)
{

}
void getPWM::execute(xmlrpc_c::paramList const& params, xmlrpc_c::value* const retvalP)
{
	// Specs:
	//
	//  **2 PWM outputs are located on XHCC therefore only 2 reading operations are needed
	//  **First PWM of XHCC is assigned on Adafruit I2C to PWM to 9th Channel because first 8 Channels are occupied for temperature setting operation
	//	**Second PWM of XHCC is assigned on Adafruit I2C to PWM to 10th Channel
	//	Design Notes:
	//	Since Adafruit I2C to PWM can accept input signal which lies between 0-3.3V,therefore, 0-10V signal is going to be translated to 0-3,3V signal
    //  Attenuation(Dampfung) is hard-coded and should not be changed as long as designated Platine is in use.

	     int bus =2;
		 int addr= 0x48;  // Default address of PCA9658 is 0x40. Please refer its Datasheet
		 ADS1015 ADS1015_MOD(bus,addr);
		 string channel_Name ="";
		 int channel_value =0;
		 channel_Name = (string)params.getString(0);
		 int value=-1;

		 if(channel_Name =="PWM0")
		 {
			 cout<<"getting PWM 1 value.... "<<endl;
			 channel_value = ADS1015_MOD.Read(0);
			 value = channel_value;
			 cout<<" PWM value is "<<  value<<endl;
			 *retvalP = xmlrpc_c::value_int(value);
		 }
		 else if(channel_Name == "PWM1")
		 {
			 cout<<"getting PWM 2 value.... "<<endl;
			 channel_value = ADS1015_MOD.Read(1);
			 value = (int)channel_value;
			 cout<<" PWM 2 value :"<< value <<endl;
			 *retvalP = xmlrpc_c::value_int(value);

		 }
		 else
		 {

			 *retvalP = xmlrpc_c::value_int(value);
		 }

}
void get10V::execute(xmlrpc_c::paramList const& params, xmlrpc_c::value* const retvalP)
{
	// Specs:
	//
	//  **1 10V pot Frei Relay is located on XHCC therefore only 1 reading operations are needed
	//  **10V pot Frei Relay of XHCC is assigned on Adafruit I2C to PWM to 11th Channel because first 10 Channels are occupied for temperature setting & PWM read operation.
    //
	/* int bus =2;
	 int addr= 0x40;  // Default address of PCA9658 is 0x40. Please refer its Datasheet
	 PCA9685 PCA9685_MOD(bus,addr);
	 string requested_OP = (string)params.getString(0);
	 int channel_value = PCA9685_MOD.getPWM(11);

	 if(channel_value < 0.1)
	 {
		 *retvalP = xmlrpc_c::value("OFF");
	 }
	 else if(channel_value > 3.2 && channel_value < 3.4 )
	 {

		 *retvalP = xmlrpc_c::value("ON");
	 }
	 else
	 {
		 *retvalP = xmlrpc_c::value_boolean(false);
	 }
*/

}
void MixerIsClosing::execute(xmlrpc_c::paramList const& params, xmlrpc_c::value* const retvalP)
{
	// Specs:
		//
		//  **Mixer is located on XHCC therefore only 1 reading operations are needed
		//  **Mixer of XHCC is assigned on Adafruit I2C to PWM to 12th Channel because first 12 Channels are occupied for temperature setting,PWM read operation & 10POTFREI.
	    //
	/*	 int bus =2;
		 int addr= 0x40;  // Default address of PCA9658 is 0x40. Please refer its Datasheet
		 PCA9685 PCA9685_MOD(bus,addr);
		 string requested_OP = (string)params.getString(0);
		 int channel_value = PCA9685_MOD.getPWM(12);

		 if(channel_value < 0.1)
		 {
			 *retvalP = xmlrpc_c::value("OFF");
		 }
		 else if(channel_value > 3.2 && channel_value < 3.4 )
		 {

			 *retvalP = xmlrpc_c::value("ON");
		 }
		 else
		 {
			 *retvalP = xmlrpc_c::value_boolean(false);
		 }
*/
}
void MixerIsOpening::execute(xmlrpc_c::paramList const& params, xmlrpc_c::value* const retvalP)
{
	// Specs:
		//
		//  **Mixer is located on XHCC therefore only 1 reading operations are needed
		//  **Mixer of XHCC is assigned on Adafruit I2C to PWM to 12th Channel because first 12 Channels are occupied for temperature setting,PWM read operation & 10POTFREI.
	    //
		/* int bus =2;
		 int addr= 0x40;  // Default address of PCA9658 is 0x40. Please refer its Datasheet
		 PCA9685 PCA9685_MOD(bus,addr);
		 string requested_OP = (string)params.getString(0);
		 int channel_value = PCA9685_MOD.getPWM(12);

		 if(channel_value < 0.1)
		 {
			 *retvalP = xmlrpc_c::value("OFF");
		 }
		 else if(channel_value > 3.2 && channel_value < 3.4 )
		 {

			 *retvalP = xmlrpc_c::value("ON");
		 }
		 else
		 {
			 *retvalP = xmlrpc_c::value_boolean(false);
		 }
*/
}
void setRcMode::execute(xmlrpc_c::paramList const& params, xmlrpc_c::value* const retvalP)
{

}
void setRcTemp::execute(xmlrpc_c::paramList const& params, xmlrpc_c::value* const retvalP)
{

}
void TestHandler::execute(xmlrpc_c::paramList const& params, xmlrpc_c::value* const retvalP)
{

}


void ShutdownHandler::execute(xmlrpc_c::paramList const& params, xmlrpc_c::value* const retvalP) {
	cout << "Server: Handling \"" << cs4513::Server::SHUTDOWN_RPC_NAME << "\" RPC." << endl;
	Server::instance()->shutdown();
	*retvalP = xmlrpc_c::value_boolean(true);
}

