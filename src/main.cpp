/* Exercise: [MGS] EPR: Game Of Life; with focus on performance
 * Author: David Pertiller
 * Date: 21.12.2012
 * Version: 2, first design and implementation
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include "Timer.h"
#include <string>
#include <vector>
#include <sstream>
#include "FileManager.h"
#include "GolOpenMP.hpp"
#include "Change.hpp"
#include "GolSeq.hpp"
#include "GolOpenCL.hpp"
#include "Shared.h"

#include <GL/glut.h>



using namespace std;


int main( int argc, char* argv[] )
{

	glutInit(&argc, argv);


	//proper execution: gol --load board5x5 test1 in.gol --save board5x5 test1 out.gol --generations 1 --measure
	char *input = NULL, *output = NULL; //path to the input and output file
	char *mode = NULL;
	static int generations = 1; //how many generations should be computed
	bool measure = false;
	int threads = -1;
	
	//OpenCL specific additional command line parameters
	unsigned __int64 deviceType = 0xFFFFFFFF;
	int platformID = -1;
	int deviceID = -1;

	Timer timer;
	timer.start(); //measure init time (load+creation of board in memory)

	//parse the command line arguments:
	for (int i = 1; i < argc; i++)
	{
		char *option = argv[i];

		if (strcmp(option, "--load") == 0)
		{
			input = argv[++i];
		}
		else if (strcmp(option, "--save") == 0)
		{
			output = argv[++i];
		}
		else if (strcmp(option, "--generations") == 0)
		{
			generations = atoi(argv[++i]);
		}
		else if (strcmp(option, "--measure") == 0)
		{
			measure = true;
		}
		else if (strcmp(option, "--mode") == 0)
		{
			mode = argv[++i];
		}
		else if (strcmp(option, "--device") == 0)
		{
			char *deviceFlag = argv[++i];
			
			if (strcmp(deviceFlag, "cpu") == 0)
				deviceType = (1 << 1); //CL_DEVICE_TYPE_CPU
			else if (strcmp(deviceFlag, "gpu") == 0)
				deviceType = (1 << 2); //CL_DEVICE_TYPE_GPU
			//everything else is set to 0xFFFFFFFF (CL_DEVICE_TYPE_ALL)
		}
		else if (strcmp(option, "--platformId") == 0)
		{
			platformID = atoi(argv[++i]);
		}
		else if (strcmp(option, "--deviceId") == 0)
		{
			deviceID = atoi(argv[++i]);
		}
		else if (strcmp(option, "--threads") == 0)
		{
			threads = atoi(argv[++i]);
		}
	}

	if (input == false) //input and output must be specified
		return EXIT_FAILURE;

	
	//FileManager fileManager; //This FileManager is dedicated to parsing GOL Data
	cout << "Reading file: " << input << "\n";
	BoardData boardData  = ReadboardData(input); //read the data from the input file

	if (boardData.size() == 0) //if an error occurred, the board size is 0
	{
		return EXIT_FAILURE;
	}


	//Board board(width, height); //the computation will take place in the board class
		
	timer.stop(); //Initialization time till board has been created
	if (measure)
		std::cout << "Init: " << timer.getElapsedTimeString() << "\n";

	if (strcmp(mode, "ocl") == 0)
	{
		timer.start(); //measure how long the computation of the generations takes
		ComputeCL(boardData, generations, deviceType, platformID, deviceID); //the computation method will iterate over the data for the given number of generations
		timer.stop(); //Kernel run time
	}
	

	if (measure)
		std::cout << " Compute:" << timer.getElapsedTimeString() << "\n";

	system("pause");
	return 0;
}