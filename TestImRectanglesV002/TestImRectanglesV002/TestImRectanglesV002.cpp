// TestImRectanglesV002.cpp : Defines the entry point for the console application.
// multiple rectangles on image
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
	FileNameBase = "Rectange002";
	FileNameExtension = ".bmp";
	bool saveResult = 1;
	bool displayResult = 0;

	int spacingX = 40;
	int spacingY = 40;
	int offsetX = 40;
	int offsetY = 40;

	int maxX, maxY;
	maxX = 256;
	maxY = 256;

	for (int size = 1; size <= 20; size++)
	{
		int rectSizeX = size;
		int rectSizeY = 20;
		Mat Im = Mat::zeros(maxY, maxX, CV_8U);
		for (int y = offsetY; y < maxY - offsetY; y += spacingY)
		{
			for (int x = offsetX; x < maxX - offsetX; x += spacingY)
			{
				Point start = Point(x, y);
				Point stop = Point(start.x + rectSizeX, start.y + rectSizeY);
				rectangle(Im, start, stop, 255, CV_FILLED);
			}
		}
		if (saveResult)
		{
			string FileName = FolderName + FileNameBase + "S" + ItoStrLZ(size, 3) + FileNameExtension;
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

