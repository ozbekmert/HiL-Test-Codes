//============================================================================
// Name        : main.cpp
// Author      : Mert Selim Özbek
// Version     : 1.0
// Copyright   : 
// Description : This file represent complete design codes
//============================================================================
/*
 * ====================================Additional Notice====================================
 *
 * ================== Pre-Installation required to initilize PWM output on beaglebone ==================
 *
 * system("SLOTS=/sys/devices/bone_capemgr.7/slots");
 * system("cd /lib/firmware/");
 * system("echo bone_pwm_P8_13 >  $SLOTS");
 * system("echo bone_pwm_P8_19 >  $SLOTS");
 * system("echo bone_pwm_P8_34 >  $SLOTS");
 * system("echo bone_pwm_P8_36 >  $SLOTS");
 * system("echo bone_pwm_P9_21 >  $SLOTS");
 * system("echo bone_pwm_P9_22 >  $SLOTS");
 * system("echo bone_pwm_P9_28 >  $SLOTS");
 * system("echo bone_pwm_P9_42 >  $SLOTS");
 * system("echo am33xx_pwm > $SLOTS");
 * system("cat $SLOTS");
 *
 * ================== PWM pins and contructor on BeagleBone ==================
 *  PWM pwm("pwm_test_P8_34.9");
 *  PWM pwm("pwm_test_P8_36.9");
 *  PWM pwm("pwm_test_P9_14.9");
 *  PWM pwm("pwm_test_P9_16.9");
 *  g++ -pthread -o beaglebone_test main.cpp I2C.cpp GPIO.cpp PCA9685.cpp PWM.cpp util.cpp
 *  g++ -pthread -o beaglebone_test main.cpp I2C.cpp GPIO.cpp PCA9685.cpp PWM.cpp util.cpp TempVoltLib.cpp
 */

//System Includes
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <string>
#include "errno.h"
// Local includes
#include "Server.hpp"
#include "RPCHandler.hpp"
//#include"PWM.h"
//#include"GPIO.h"
//#include "PCA9685.h"
//#include "TempVoltLib.h"
// Declare which objects from namespace STD we are using.
//using namespace exploringBB;
//using namespace liba;
using namespace std;
using namespace cs4513;
using std::cout;
using std::cerr;
using std::endl;

void printUsageAndExit()
{
	cout << "Usage: Server port" << endl;
	cout << "\tport - The port on which to listen." << endl;
	cout << "Example: Server 9382" << endl;
	exit(EXIT_SUCCESS);
}


int main(int argc, char** argv)
{

	// Setting up Abyss Server for XML Remote Procedure Call

	if (argc != 2)
			printUsageAndExit();
	// Convert/validate the given port number.
	errno = 0;
	char* endptr = NULL;
	int portNum = (int) strtod(argv[1], &endptr);

	if ((portNum == 0) || (errno != 0) || (*endptr != 0))
	{
		cerr << "Invalid port number: \"" << argv[1] << "\"" << endl;
		exit(EXIT_FAILURE);
	}
	cout << "Server: Using port " << portNum << endl;
	// Get the singleton instance of the Server class.
	Server* server = Server::instance();
	// Set the port number given via the command-line.
	server->setServerPort(portNum);
	// Initialize the XML-RPC server.
	server->initializeXMLRPCServer();
	// Start the XML-RPC server.
	server->run();
	cout << "Server: Waiting for client connections." << endl;
	while (server->isRunning() == true)
	{
			sleep(1);
	}
		// Kill the XML-RPC server.
		server->stop();
		// Clean up.
		delete server;
		cout << "Server: Halted." << endl;



	return 0;
}


/*

int main()
{


	//========= Setting up Internal PWMs of Bone =========
			   PWM pwm("pwm_test_P8_13.9");
			   PWM pwm1("pwm_test_P8_19.10");
			//========= Setting up GPIO of Bone ==================

			    GPIO inS1(49), inS2(115);
			    inS1.setDirection(INPUT);
			    inS2.setDirection(INPUT);
			//========= Setting up PCA9658 & I2C Connection  ==================
			   int bus =2;
			   int addr= 0x40;  // Default address of PCA9658 is 0x40. Please refer its Datasheet
			   PCA9685 PCA9685_MOD(bus,addr);
			//========= Main Program ============================
			//====================================================
			//====================Solar Test======================
			//====================Load Temp Values======================
			   TempVoltLib lib;
			   int height = 2;
			   int width = 450;
			   float** temps = lib.readTempValue(height,width);
			  // for(int i =0; i<451;i++)
			  // {
			  //	   cout <<"Temperature: " <<temps[0][i] <<endl;
			  //	   cout <<"Voltage: " <<temps[1][i]<<endl;
			  // }
			//=================Settings up outputs==================
			   PCA9685_MOD.setPWMFreq(1000);

			   PCA9685_MOD.writeAnalog(1,lib.tempToVoltage(temps,450,-5.0)); //S1
			   usleep(10000000);
			   PCA9685_MOD.writeAnalog(1,lib.tempToVoltage(temps,450,0.0)); //S2
			   usleep(10000000);
			   PCA9685_MOD.writeAnalog(1,lib.tempToVoltage(temps,450,11.0)); //S1
			   usleep(10000000);
			   PCA9685_MOD.writeAnalog(1,lib.tempToVoltage(temps,450,51.0)); //S2
			   usleep(10000000);
			   PCA9685_MOD.writeAnalog(1,lib.tempToVoltage(temps,450,91.0)); //S1
			   usleep(10000000);
			   PCA9685_MOD.writeAnalog(1,lib.tempToVoltage(temps,450,121.0)); //S2
			   usleep(10000000);

			   if(inS1.getValue() == 0 and inS2.getValue() == 0) // Relays closed
			   {
					   PCA9685_MOD.writeAnalog(1,lib.tempToVoltage(temps,450,99.0)); //S1

					   PCA9685_MOD.writeAnalog(1,lib.tempToVoltage(temps,450,58.0)); //S2
					   usleep(100000);
					   	   if(inS1.getValue() == 1 and inS2.getValue() == 1)//Relays opened
					   	   {
					   		   cout << "Solar system working well" << endl;
					   	   }
					   	   else
					   	   {
					   		   cout << "Phase 2 Problem, check voltage levels " << endl;
					   	   }
			   }
			   else
			   {
					   cout << "Relay initialization Problem, check voltage levels " << endl;
			   }




return 0;

}
*/
