#include<iostream>
#include<fstream>
#include<sstream>
#include"TempVoltLib.h"
using namespace std;


namespace liba{


TempVoltLib::TempVoltLib() {
}

TempVoltLib::~TempVoltLib() {

}


float TempVoltLib::tempToVoltage(float **temp_vol,int rowsize, float temperature) {

    for(int i =0 ; i<rowsize; i++ )
    {
    		if(temp_vol[0][i]== temperature)
    		{
    			cout << "Temperature: " << temp_vol[0][i] << "C Corresponding Voltage Voltage:  " << temp_vol[1][i] <<"V"<<endl;
    			return temp_vol[1][i];
    	    }

    }

    return 0;

}

float** TempVoltLib::readTempValue(unsigned height, unsigned width)
{
	float** array2D = 0;
      array2D = new float*[height];
      unsigned int h =0;
      for (h = 0; h < height; h++)
      {
            array2D[h] = new float[width];
      }

      std::fstream iFile;
      iFile.open("temperature.txt");
      std::string input = "";
      int index =0;
      if( iFile.is_open() )
      {
         while( iFile.good() )
         {
			 std::getline( iFile, input, '\n' );
			 std::stringstream ss( input );
			 float myDouble = 0;
			 	 if( ss >> myDouble )
				 {

			 		array2D[0][index] = myDouble;
			 		array2D[1][index]= calculateVoltageFromResistance(myDouble);
			 		index++;
			     }
		}

         iFile.close();
		}
      else
    	 std::cout << "Error opening file...\n";




      return array2D;

}

double TempVoltLib::calculateVoltageFromResistance(double temp)
{
	 double R0 = 1000.0; // 1000 Ohm
		      double A = 3.9083*0.001; // 1000 Ohm
		      double B = -5.775*0.0000001; // 1000 Ohm
		      double C = -4.183*0.000000000001;// 1000 Ohm
		      double vt = 3.344;
			  double Rtneg =0;
		      double Rtpos =0;

		      double Vsollneg=0;
		      double Vsollpos=0;



		      if(-200< temp && temp < 0)
		      {
				  Rtneg = R0*(1 +  A*temp  + B*temp*temp  +  C*temp*temp*temp*(temp-100));
		          Vsollneg = vt-(vt*2*Rtneg/(2200+Rtneg));
		        		  return Vsollneg;
		      }
		      else if(0 <= temp && temp < 250)
		      {
				Rtpos = R0*(1+A*temp+B*temp*temp);
				Vsollpos = vt-(vt*2*Rtpos/(2200+Rtpos));
				  return Vsollpos;
		      }
		      else
		    	 return 0;


}





};

