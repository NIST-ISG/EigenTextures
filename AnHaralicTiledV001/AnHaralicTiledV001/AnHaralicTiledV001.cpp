// AnHaralicTiledV001.cpp : Defines the entry point for the console application.
//

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\contrib\contrib.hpp"
#include "math.h"
#include <iostream>
#include <fstream>
#include <Windows.h>

#include "Functions.h"
#include "RedundantWaveletLib.h"
#include "NormalizationLib.h"
#include "HaralickLib.h"
#include "StringFcLib.h"

#define PI 3.14159265

using namespace cv;
using namespace std;

const int stepNr = 180;


int main(int argc, char* argv[])
{
	//string InFolderName, OutFolderName;
	string StrTemp;
	//string OutFileNameExtension = ".bmp";
	//string FileNameExtension = ".bmp";

	string InFileName = "D:\\Eigentextures\\BiocellBrodatzTest\\cellimage1_cropped.tif";
	string OutFileName = "D:\\Eigentextures\\BiocellBrodatzTest\\cellimage1_cropped.BMP";
	string OutFileName2 = "D:\\Eigentextures\\BiocellBrodatzTest\\cellimage1_croppedTi32Tr5wEiB.BMP";

	string InStr;

	bool displayResult = 1;
	bool displayCom = 0;
	bool saveResult = 0;
	bool commonResult = 0;
	bool outLocal = 1;
	bool outScreen = 0;
	bool goThru = 1;
	bool showTiles = 1;

	int normalisation = 1;

	int binCount = 16;

	int minOfset = 2;
	int maxOfset = 16;


	float minNorm = 0.0;
	float maxNorm = 5000;

	float minNormGlobal = 0.0;
	float maxNormGlobal = 5000;


	int maxTileX =30;
	int maxTileY =30;
	int lineHalfLength = 10;
	//	32767.0;

	float angleStep = 1;
	int interpolation = INTER_CUBIC;

	cout << "\n";
/*
	// arguments read and decode 
	if (argc < 5)
	{
		cout << "\nTo few arguments.";
		return 0;
	}

	StrTemp = argv[1];
	if (StrTemp == "Y" || StrTemp == "y")
	{
		displayResult = 1;
	}
	StrTemp = argv[2];
	if (StrTemp == "Y" || StrTemp == "y")
	{
		saveResult = 1;
	}
	StrTemp = argv[3];
	if (StrTemp == "Y" || StrTemp == "y")
	{
		commonResult = 1;
	}
	InFileName = argv[4];
	OutFileName = argv[5];

//	if (argc > 6)
//		FileNameExtension = argv[6];

	// output arguments
	if (displayResult)
		StrTemp = "Y";
	else
		StrTemp = "N";
	cout << "Display reult images - " << StrTemp << "\n";

	if (saveResult)
		StrTemp = "Y";
	else
		StrTemp = "N";
	cout << "Save reult images - " << StrTemp << "\n";

	if (commonResult)
		StrTemp = "Y";
	else
		StrTemp = "N";
	cout << "Save feature calc - " << StrTemp << "\n";
*/
	cout << "In file  - " << InFileName << "\n";
	cout << "Out file" << OutFileName << "\n";


	Mat ImIn = imread(InFileName, CV_LOAD_IMAGE_ANYDEPTH);

	int maxX, maxY, maxXY;
	maxX = ImIn.cols;
	maxY = ImIn.rows;
	maxXY = maxX * maxY;

	Mat Roi = Mat::ones(maxY, maxX, CV_16U);

	// conversion to float
	Mat ImInF, ImInFTemp;
	ImIn.convertTo(ImInF, CV_32F);


	switch (normalisation)
	{
	case 1:
		NormParamsMinMax(ImInF, &maxNormGlobal, &minNormGlobal);
		break;
	default:
		break;
	}

	Mat ImTemp, ImShow, ImShowGlobal, ImScale;


	float normCoef = maxNormGlobal / 256;
	ImInFTemp = ImInF / normCoef;

	namedWindow("Image", WINDOW_AUTOSIZE);
	namedWindow("ImageSmall", WINDOW_AUTOSIZE);

	namedWindow("COM", WINDOW_NORMAL);
	namedWindow("Kernel", WINDOW_NORMAL);

	if (displayResult)
	{
		ImInFTemp.convertTo(ImTemp, CV_8U);
		applyColorMap(ImTemp, ImShowGlobal, COLORMAP_JET);
		imwrite(OutFileName, ImShowGlobal);
		if (showTiles)
		{
			for (int x = maxTileX; x < maxX; x += maxTileX)
			{
				line(ImShowGlobal, Point(x, 0), Point(x, maxY - 1), Scalar(0.0, 0.0, 0.0, 0.0), 2);
			}
			for (int y = maxTileY; y < maxY; y += maxTileY)
			{
				line(ImShowGlobal, Point(0, y), Point(maxX - 1, y), Scalar(0.0, 0.0, 0.0, 0.0), 2);
			}

		}
		imshow("Image", ImShowGlobal);
	}
	Mat SmallIm;
	for (int y = 0; (y + maxTileY) <= maxY; y += maxTileY)
	{
		for (int x = 0; (x + maxTileX ) <= maxX; x += maxTileX)
		{
			ImInF(Rect(x,y,maxTileX,maxTileY)).copyTo(SmallIm);
			switch (normalisation)
			{
			case 1:
				NormParamsMinMax(SmallIm, &maxNorm, &minNorm);
				break;
			default:
				break;
			}
			//imwrite(OutFilename)
			normCoef = (maxNorm - minNorm)/ 256;   
			ImInFTemp = (SmallIm- minNorm) / normCoef;
			ImInFTemp.convertTo(ImTemp, CV_8U);
			applyColorMap(ImTemp, ImShow, COLORMAP_JET);
			imshow("ImageSmall", ImShow);
			waitKey(100);
			cout << "y = " << y << " x = " << x << "\n";
			float Energy[stepNr];
			float Contrast[stepNr];
			float Correlation[stepNr];
			float Homogenity[stepNr];
			string OutLocal = "";
			OutLocal += "normalisation ";
			switch (normalisation)
			{
			case 1:
				OutLocal += "Min Max ";
				break;
			default:
				OutLocal += "none ";
				break;
			}
			OutLocal += " " + to_string(normalisation) + "\n";

			OutLocal += "minNorm\t" + to_string(minNorm) + "\n";
			OutLocal += "maxNorm\t" + to_string(maxNorm) + "\n";
			OutLocal += "binCount\t" + to_string(binCount) + "\n";
			OutLocal += "interpolation\t";
			switch (interpolation)
			{
			case 0:
				OutLocal += "Nearest";
				break;
			case 1:
				OutLocal += "Linear";
				break;
			case 2:
				OutLocal += "Bicubic";
				break;
			case 3:
				OutLocal += "Area";
				break;
			case 4:
				OutLocal += "Lanczos4";
				break;
			case 5:
				OutLocal += "Bits";
				break;
			case 7:
				OutLocal += "Max";
				break;
			default:
				break;

			}
			OutLocal += " " + to_string(interpolation) + "\n";

			cout << OutLocal + "\n";
			
			// vector for best angles histogtam
			int Angles[stepNr];
			for (int i = 0; i < stepNr; i++)
			{
				Angles[i] = 0;
			}

// ofset loop
			for (int offset = minOfset; offset <= maxOfset; offset += 1)
			{
				for (int angleIndex = 0; angleIndex < stepNr; angleIndex++)
				{
					float angle = angleStep * angleIndex;
					Mat COM = COMCardone4(SmallIm, offset, angle, binCount, maxNorm, minNorm, interpolation);
					COMParams(COM, &Contrast[angleIndex], &Energy[angleIndex], &Homogenity[angleIndex], &Correlation[angleIndex]);


					if (displayCom)
					{
						int maxCOM = 0;
						int *wCOM = (int*)COM.data;
						for (int i = 0; i < binCount*binCount; i++)
						{
							if (maxCOM < *wCOM)
								maxCOM = *wCOM;
							wCOM++;
						}
						COM = COM / (maxCOM / 255);

						COM.convertTo(ImTemp, CV_8U);
						applyColorMap(ImTemp, ImShow, COLORMAP_JET);
						//namedWindow("COM", WINDOW_AUTOSIZE);
						imshow("COM", ImShow);

						Mat Kernel = KernelPrepare(offset, angle, interpolation);
						float *wKernel = (float*)Kernel.data;
						float maxKernel = 0;
						for (int i = 0; i < Kernel.cols*Kernel.rows; i++)
						{
							if (maxKernel < *wKernel)
								maxKernel = *wKernel;
							wKernel++;
						}
						Kernel = Kernel / (maxKernel / 255);
						Kernel.at<float>(Kernel.rows / 2, Kernel.cols / 2) = 255;
						Kernel.convertTo(ImTemp, CV_8U);
						applyColorMap(ImTemp, ImShow, COLORMAP_JET);
						imshow("Kernel", ImShow);
		
						if (goThru)
							waitKey(50);
						else
							waitKey(0);

					}
				}
				// best angle for ofset
				int bestAngleContrast = FindBestAngleMin(Contrast, stepNr);
				float bestAngleContrastQS = BestAngleQualityScoreMin(Contrast, stepNr, bestAngleContrast);
		
				int bestAngleEnergy = FindBestAngleMax(Energy, stepNr);
				float bestAngleEnergyQS = BestAngleQualityScoreMax(Energy, stepNr, bestAngleEnergy);

				int bestAngleHomogenity = FindBestAngleMax(Homogenity, stepNr);
				float bestAngleHomogenityQS = BestAngleQualityScoreMax(Homogenity, stepNr, bestAngleHomogenity);

				int bestAngleCorrelation = FindBestAngleMax(Correlation, stepNr);
				float bestAngleCorrelationQS = BestAngleQualityScoreMax(Correlation, stepNr, bestAngleCorrelation);

				string Temp;
				if (offset < 10)
					Temp = " ";
				else
					Temp = "";
				cout << "offset = " << Temp << offset << " b a = " 
					<< (float)bestAngleContrast * angleStep << " "
					<< (float)bestAngleEnergy * angleStep << " "
					<< (float)bestAngleHomogenity * angleStep << " "
					<< (float)bestAngleCorrelation * angleStep << " "

					<< " QC = "
					<< bestAngleContrastQS << " "
					<< bestAngleEnergyQS << " "
					<< bestAngleHomogenityQS << " "
					<< bestAngleCorrelationQS << " "
					<< "\n";
				
				Angles[bestAngleContrast]++;
				//Angles[bestAngleEnergy]++;
				Angles[bestAngleHomogenity]++;
				Angles[bestAngleCorrelation]++;
				
			}
			int bestAngle = 0;
			int maxAngle = Angles[0];
			for(int i = 1; i < stepNr; i++)
			{
				if (maxAngle < Angles[i])
				{
					maxAngle = Angles[i];
					bestAngle = i;
				}
			}
			cout << "\n" << "best Angle = " << bestAngle << " max Angle count = " << maxAngle << "\n\n";
			// show line on image
			int barCenterX = maxTileX / 2 + x;
			int barCenterY = maxTileY / 2 + y;
			int lineOffsetX = (int)round((double)(lineHalfLength)*sin((double)(bestAngle)* PI / 180.0));
			int lineOffsetY = (int)round((double)(lineHalfLength)*cos((double)(bestAngle)* PI / 180.0));

			if (maxAngle > 4)
			{
				line(ImShowGlobal, Point(barCenterX - lineOffsetX, barCenterY - lineOffsetY), Point(barCenterX + lineOffsetX, barCenterY + lineOffsetY), Scalar(0, 0.0, 0.0, 0.0), 3);
			}
			imshow("Image", ImShowGlobal);


			waitKey(100);
		}
	}


	string CommonOutTxt;
	CommonOutTxt = "FileName\t\t";
	CommonOutTxt += "RWLLS1";

	imwrite(OutFileName2, ImShowGlobal);
	if (goThru)
		waitKey(50);
	else
		waitKey(0);

	waitKey(0);
	return 0;
}

