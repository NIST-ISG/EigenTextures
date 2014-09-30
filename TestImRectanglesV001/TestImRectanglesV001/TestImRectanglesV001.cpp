// TestImRectanglesV001.cpp : Defines the entry point for the console application.
// one rectangle in the middle of image

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\contrib\contrib.hpp"

#include "StringFcLib.h"

#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	string FolderName, FileNameBase, FileNameExtension;
	FolderName = "C:\\EigenTexturesData\\ForWavelet001\\";
	FileNameBase = "Rectange001";
	FileNameExtension = ".bmp";
	bool saveResult = 1;
	bool displayResult = 0;
	
	int maxX, maxY;
	maxX = 256;
	maxY = 256;

	for (int size = 1; size <= 20; size++)
	{
		int rectSizeX = size;
		int rectSizeY = 20;

		Point start = Point(maxX / 2 - rectSizeX / 2, maxY / 2 - rectSizeY / 2);
		Point stop = Point(start.x + rectSizeX, start.y + rectSizeY);
		Mat Im = Mat::zeros(maxY, maxX, CV_8U);
		rectangle(Im, start, stop, 255, CV_FILLED);
		if (saveResult)
		{
			string FileName = FolderName + FileNameBase + "S" + ItoStrLZ(size,3) + FileNameExtension;
 			imwrite(FileName, Im);
		}
		if (displayResult)
		{
			namedWindow("Image", WINDOW_AUTOSIZE);
			imshow("Image", Im);
			waitKey(0);
		}
	}
	return 0;
}

