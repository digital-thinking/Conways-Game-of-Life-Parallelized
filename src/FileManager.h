/* Exercise: [MGS] EPR: Game Of Life; with focus on performance
 * Author: David Pertiller
 * Date: 21.10.2012
 * Version: 1, first design and implementation
*/

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Shared.h"


using namespace std;

BoardData ReadboardData(const char *input)
{
	std::ifstream file(input); //open input file

	if (file.is_open())
	{
		string line;	//parse line by line
		std::getline(file, line); //first line contains board size in the format: width,height
		unsigned int width, height;
		//parse width and height
		{
			std::stringstream linestream(line);
			std::vector<std::string> boardSize;
			std::string size;
			while (std::getline(linestream, size, ',')) //split the string
			{
				boardSize.push_back(size);
			}
			//convert string to int to get the final sizes
			width = atoi(boardSize[0].c_str()); //the width is the first value of the splitted string
			height = atoi(boardSize[1].c_str()); //the height is the second value of the splitted string
		}

		//read the data (each cell equals one character) from the file and save it in a proper 2D vector
		vector<unsigned char> boardData(height*width);

		//read Input-lines and fill board with data
		unsigned int row = 0;
		while(std::getline(file, line))
		{
			const char *data = line.c_str(); //this creates a char-array; every element equals 1 boardData
			for (unsigned int column = 0; column < width; ++column)
			{
				int alive = data[column] == 'x' ? 255 : 0;
				boardData[row * width + column] = alive; //fill vector with contents of the string
			}

			++row;
		}

		file.close();

		BoardData dataStruct;
		dataStruct.data = boardData;
		dataStruct.width = width;
		dataStruct.height = height;

		return dataStruct;
	}

	return BoardData();
}

bool SaveBoardData(vector<vector<int>> &boardData, const char *output)
{
	unsigned int height = boardData.size();
	unsigned int width = boardData[0].size();

	//convert 1 and 0 to 'x' and '.'
	vector<vector<char>> charData(height, vector<char>(width));


	ofstream outputFile(output); //write to output file
	if (outputFile.is_open())
	{
		outputFile << width << "," << height << '\n'; //the first line shows the width and height

		//write the computed data for every row in the vector
		for (unsigned int i = 0; i < height; ++i)
		{			
			//convert int data back to char
			for (unsigned int row = 0; row < width; ++row)
				charData[i][row] = boardData[i][row] == 1 ? 'x' : '.';

			string line(charData[i].begin(), charData[i].end());
			outputFile << line << '\n';
		}

		outputFile.close();
	}
	return true;
}
