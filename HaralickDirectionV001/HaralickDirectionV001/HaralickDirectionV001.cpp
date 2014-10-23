// HaralickDirectionV001.cpp : Defines the entry point for the console application.
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


using namespace cv;
using namespace std;

const int stepNr = 180;

int main(int argc, char* argv[])
{
	// global var declarations
	string InFolderName, OutFolderName;
	string StrTemp;
	string OutFileNameExtension = ".bmp";
	string FileNameExtension = ".tif";

	string InStr;

	bool displayResult = 0;
	bool saveResult = 0;
	bool commonResult = 0;
	bool outLocal = 0;
	bool outScreen = 0;
	bool goThru = 1;

	int binCount = 16;
	int ofset = 10;
	float angleStep = 1;
	
	cout << "\n";

	// arguments read and decode 
	if (argc < 5)
	{
		cout << "\nTo few arguments.";
		return 0;
	}

	StrTemp = argv[1];
	if (StrTemp == "Y")
	{
		displayResult = 1;
	}
	StrTemp = argv[2];
	if (StrTemp == "Y")
	{
		saveResult = 1;
	}
	StrTemp = argv[3];
	if (StrTemp == "Y")
	{
		commonResult = 1;
	}
	InFolderName = argv[4];
	OutFolderName = argv[5];

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

	cout << "In folder  - " << InFolderName << "\n";
	cout << "Out folder - " << OutFolderName << "\n";




	string CommonOutTxt;
	CommonOutTxt = "FileName\t\t";
	CommonOutTxt += "RWLLS1";

	// read first file
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFileA((InFolderName + "*" + FileNameExtension).c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		cout << " file not found";
		cin >> InStr;
		return 0;
	}
	//handle files
	if (displayResult)
	{
		namedWindow("Image", WINDOW_AUTOSIZE);
		namedWindow("COM", WINDOW_NORMAL);
		namedWindow("Kernel", WINDOW_NORMAL);
	}

	float angle = 0;
	while (hFind != INVALID_HANDLE_VALUE)
	{
		string InFileName = FindFileData.cFileName;
		cout << "file - " << InFileName << "\n\r";

		// file name base
		int lastIndex = InFileName.find_last_of(".");
		string FileNameBase = InFileName.substr(0, lastIndex);

		string InFileNameFull = InFolderName + InFileName;

		// read image
		Mat ImIn = imread(InFileNameFull, CV_LOAD_IMAGE_GRAYSCALE);

		int maxX, maxY, maxXY;
		maxX = ImIn.cols;
		maxY = ImIn.rows;
		maxXY = maxX * maxY;

		Mat Roi = Mat::ones(maxY, maxX, CV_16U);

		// conversion to float
		Mat ImInF;
		ImIn.convertTo(ImInF, CV_32F);

		Mat ImTemp, ImShow, ImScale;

		if (displayResult)
		{
			ImInF.convertTo(ImTemp, CV_8U);
			applyColorMap(ImTemp, ImShow, COLORMAP_JET);
			imshow("Image", ImShow);
		}


		float minNorm, maxNorm;
		NormParamsMinMax(ImInF, &maxNorm, &minNorm);
		cout << maxNorm << "\t" << minNorm << "\n";

		float Energy[stepNr];
		float Contrast[stepNr];
		float Correlation[stepNr];
		float Homogenity[stepNr];
		string OutLocal = "File name \t" + InFileName + "\n";

		OutLocal += "Contrast ";
		for (int angleIndex = 0; angleIndex <= stepNr; angleIndex++)
		{
			OutLocal += "\t" ;
		}
		OutLocal += "\t";

		OutLocal += "Energy ";
		for (int angleIndex = 0; angleIndex <= stepNr; angleIndex++)
		{
			OutLocal += "\t";
		}
		OutLocal += "\t";

		OutLocal += "Homogenity ";
		for (int angleIndex = 0; angleIndex <= stepNr; angleIndex++)
		{
			OutLocal += "\t";
		}
		OutLocal += "\t";

		OutLocal += "Corelation ";
		for (int angleIndex = 0; angleIndex <= stepNr; angleIndex++)
		{
			OutLocal += "\t";
		}
		OutLocal += "\n";


		for (int k = 0; k < 4; k++)
		{
			OutLocal += "Angle ";
			for (int angleIndex = 0; angleIndex < stepNr; angleIndex++)
			{
				OutLocal += "\t" + to_string(angleStep * angleIndex);
			}
			OutLocal += "\t";
			OutLocal += "\t";
		}
		OutLocal += "\n";

// ofset loop
		for (int offset = 1; offset <= 32; offset += 1) 
		{
			for (int angleIndex = 0; angleIndex < stepNr; angleIndex++)
			{
				angle = angleStep * angleIndex;
				Mat COM = COMCardone2(ImInF, offset, angle, binCount, maxNorm, minNorm);
				COMParams(COM, &Contrast[angleIndex], &Energy[angleIndex], &Homogenity[angleIndex], &Correlation[angleIndex]);
				if (outScreen)
				{
					cout << Contrast[angleIndex] << "\t" << Energy[angleIndex] << "\t" << Homogenity[angleIndex] << "\t" << Correlation[angleIndex] << "\n";
				}

		
				if (displayResult)
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


					Mat Kernel = KernelPrepare(offset, angle);
					float *wKernel = (float*)Kernel.data;
					float maxKernel = 0;
					for (int i = 0; i < Kernel.cols*Kernel.rows; i++)
					{
						if (maxKernel < *wKernel)
							maxKernel = *wKernel;
						wKernel++;
					}
					Kernel = Kernel / maxKernel;
					Kernel.convertTo(ImTemp, CV_8U);
					applyColorMap(ImTemp, ImShow, COLORMAP_JET);
					imshow("Kernel", Kernel);


					if(goThru)
						waitKey(100);
					else
						waitKey(0);
				}

			}
			// find best angle
			float bestAngleIndex;
			float bestAngleQuality;
			float minContrast = (float)10000000;
			for (int angleIndex = 0; angleIndex < stepNr; angleIndex++)
			{
				if (minContrast > Contrast[angleIndex])
				{
					minContrast = Contrast[angleIndex];
					bestAngleIndex = angleIndex;
				}
			}
			float sum = 0;
			for (int angleIndex = 0; angleIndex < stepNr; angleIndex++)
			{
				if (angleIndex != bestAngleIndex)
				{
					sum += Contrast[angleIndex];
				}
			}
			float mean = sum / (stepNr - (float)1);
			float dev = 0;
			for (int angleIndex = 0; angleIndex < stepNr; angleIndex++)
			{
				if (angleIndex != bestAngleIndex)
				{
					dev += (mean - Contrast[angleIndex]) * (mean - Contrast[angleIndex]);
				}
			}
			float std = sqrt(dev / (stepNr - (float)1));
			float coef = (mean - std) - minContrast;

			cout << "offset = " << offset << " best angle = " << bestAngleIndex * angleStep << " qality coeff = " << coef << "\n";

			//
			if (outLocal)
			{
				OutLocal += "offset" + to_string(offset);
				for (int angleIndex = 0; angleIndex < stepNr; angleIndex++)
				{
					OutLocal += "\t" + to_string(Contrast[angleIndex]);
				}
				OutLocal += "\t";
				OutLocal += "\t";
				OutLocal += "offset" + to_string(offset);
				for (int angleIndex = 0; angleIndex < stepNr; angleIndex++)
				{
					OutLocal += "\t" + to_string(Energy[angleIndex]);
				}
				OutLocal += "\t";
				OutLocal += "\t";
				OutLocal += "offset" + to_string(offset);
				for (int angleIndex = 0; angleIndex < stepNr; angleIndex++)
				{
					OutLocal += "\t" + to_string(Homogenity[angleIndex]);
				}
				OutLocal += "\t";
				OutLocal += "\t";
				OutLocal += "offset" + to_string(offset);
				for (int angleIndex = 0; angleIndex < stepNr; angleIndex++)
				{
					OutLocal += "\t" + to_string(Correlation[angleIndex]);
				}

				OutLocal += "\n";
			}
		}
		if (outLocal)
		{
			string TextFileName = OutFolderName + FileNameBase + ".txt";
			ofstream out(TextFileName);
			out << OutLocal;
			out.close();
		}
		//show images

//		Mat ImTemp, ImShow, ImScale;

		//NormParamsMeanP3Std(ImInF, &maxNorm, &minNorm);
		//cout << maxNorm << "\t" << minNorm << "\t";

		//waitKey(10);
		//find the next file
		if (!FindNextFileA(hFind, &FindFileData))
			break;
	}

	cin >> StrTemp;
	return 0;
}

