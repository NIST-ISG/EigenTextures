// HaralickDirectionsV003.cpp : Defines the entry point for the console application.
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

using namespace cv;
using namespace std;

const int stepNr = 180;

#define PI 3.14159265

int main(int argc, char* argv[])
{
	// global var declarations
	string InFolderName, OutFolderName;
	string StrTemp;
	string OutFileNameExtension = ".bmp";
	string FileNameExtension = ".bmp";

	string InStr;

	bool displayResult = 1;//
	bool saveResult = 1;
	bool saveCOM = 1;//
	bool commonResult = 1;
	bool outLocal = 1;
	bool outScreen = 0;
	bool displayCOM = 1;//
	bool goThru = 1;

	int normalisation = 0;

	int binCount = 16;

	int minOfset = 2;
	int maxOfset = 16;


	float minNorm = 0.0;
	float maxNorm = 65535.0;// 32767.0;

	float angleStep = 1;
	int interpolation = INTER_LINEAR;

	

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

	if (argc > 6)
		FileNameExtension = argv[6];

	// output arguments
	if (displayResult)
		StrTemp = "Y";
	else
		StrTemp = "N";
	cout << "Display result images - " << StrTemp << "\n";

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



	string CommonOut = "";
	string CommonOutFName;
	string CommonOutBA, CommonOutBACont, CommonOutBAEner, CommonOutBAHomo, CommonOutBACorr;
	string CommonOutBANr, CommonOutBANrCont, CommonOutBANrEner, CommonOutBANrHomo, CommonOutBANrCorr;
	CommonOutFName += "FileName\t";
	CommonOutBA += "BestAngle\t";
	CommonOutBACont += "Best Angle Contrast\t";
	CommonOutBAEner += "Best Angle Enargy\t";
	CommonOutBAHomo += "Best Angle Homogenity\t";
	CommonOutBACorr += "Best Angle Correlation\t";
	CommonOutBANr += "Best Angle #\t";
	CommonOutBANrCont += "Best Angle # Contrast\t";
	CommonOutBANrEner += "Best Angle # Energy\t";
	CommonOutBANrHomo += "Best Angle # Homogenity\t";
	CommonOutBANrCorr += "Best Angle # Correlation\t";
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
	}
	if (displayCOM)
	{
		namedWindow("COM", WINDOW_NORMAL);
		namedWindow("Kernel", WINDOW_NORMAL);
	}

	float angle = 0;
	while (hFind != INVALID_HANDLE_VALUE)
	{
		string InFileName = FindFileData.cFileName;
		//cout << "file - " << InFileName << "\n\r";

		// file name base
		int lastIndex = InFileName.find_last_of(".");
		string FileNameBase = InFileName.substr(0, lastIndex);

		string InFileNameFull = InFolderName + InFileName;
		string OutFileNameFull;
		// read image
		Mat ImIn = imread(InFileNameFull, CV_LOAD_IMAGE_ANYDEPTH);

		int maxX, maxY, maxXY;
		maxX = ImIn.cols;
		maxY = ImIn.rows;
		maxXY = maxX * maxY;

		Mat Roi = Mat::ones(maxY, maxX, CV_16U);

		// conversion to float
		Mat ImInF, ImInFTemp;
		ImIn.convertTo(ImInF, CV_32F);

		Mat ImTemp, ImShow, ImScale;


		float normCoef = maxNorm / 256;
		ImInFTemp = ImInF / normCoef;
		if (displayResult)
		{
			ImInFTemp.convertTo(ImTemp, CV_8U);
			applyColorMap(ImTemp, ImShow, COLORMAP_JET);
			imshow("Image", ImShow);
			waitKey(100);
		}


		switch (normalisation)
		{
		case 1:
			NormParamsMinMax(ImInF, &maxNorm, &minNorm);
			break;
		default:
			break;
		}

		float Energy[stepNr];
		float Contrast[stepNr];
		float Correlation[stepNr];
		float Homogenity[stepNr];
		string OutLocal = "File name\t" + InFileName + "\n";
		string OutLocal2 = "Best Angle\tContrast\tEnergy\tHomogenity\tCorrelation\t\tQCoeff\tContrast\tEnergy\tHomogenity\tCorrelation\n";
		OutLocal += "maxX\t" + to_string(maxX) + "\n";
		OutLocal += "maxY\t" + to_string(maxY) + "\n";
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

		cout << "\n" << OutLocal + "\n";



		OutLocal += "Contrast ";
		for (int angleIndex = 0; angleIndex <= stepNr; angleIndex++)
		{
			OutLocal += "\t";
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

		// vector for best angles histogtam
		int AnglesC[stepNr];
		int AnglesE[stepNr];
		int AnglesH[stepNr];
		int AnglesR[stepNr];
		int Angles[stepNr];

		for (int i = 0; i < stepNr; i++)
		{
			AnglesC[i] = 0;
			AnglesE[i] = 0;
			AnglesH[i] = 0;
			AnglesR[i] = 0;
			Angles[i] = 0;
		}

		// ofset loop
		for (int offset = minOfset; offset <= maxOfset; offset += 1)
		{
			for (int angleIndex = 0; angleIndex < stepNr; angleIndex++)
			{
				angle = angleStep * angleIndex;
				Mat COM = COMCardone4(ImInF, offset, angle, binCount, maxNorm, minNorm, interpolation);
				COMParams(COM, &Contrast[angleIndex], &Energy[angleIndex], &Homogenity[angleIndex], &Correlation[angleIndex]);
				if (outScreen)
				{
					cout << Contrast[angleIndex] << "\t" << Energy[angleIndex] << "\t" << Homogenity[angleIndex] << "\t" << Correlation[angleIndex] << "\n";
				}


				if (displayCOM)
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
					if (saveCOM)
					{
						OutFileNameFull = OutFolderName + FileNameBase + "COM" + "O" + ItoStrLZ(offset, 2) + "A" + ItoStrLZ(angle, 3) + ".bmp";
						imwrite(OutFileNameFull, ImShow);
					}

					Mat Kernel = Mat::zeros(offset * 2 + 3, offset * 2 + 3, CV_32F);
					
					float spX, spY;
					float spX0, spX1, spY0, spY1;
					float coef00, coef01, coef10, coef11;

					
					float *wSP00;
					float *wSP01;
					float *wSP10;
					float *wSP11;

					//Second point coordinates
					spX = (float)(offset)*sin((angle)* PI / 180.0);
					spY = (float)(offset)*cos((angle)* PI / 180.0);

					spX0 = floor(spX);
					spX1 = ceil(spX);
					if (spX1 == spX0)
						spX1 = spX0 + 1;

					spY0 = floor(spY);
					spY1 = ceil(spY);
					if (spY1 == spY0)
						spY1 = spY0 + 1;

					//Second Point coeficients
					coef00 = (spY1 - spY)*(spX1 - spX) / ((spX1 - spX0)*(spY1 - spY0));
					coef01 = (spY1 - spY)*(spX - spX0) / ((spX1 - spX0)*(spY1 - spY0));
					coef10 = (spY - spY0)*(spX1 - spX) / ((spX1 - spX0)*(spY1 - spY0));
					coef11 = (spY - spY0)*(spX - spX0) / ((spX1 - spX0)*(spY1 - spY0));


					
					float *wKernel = (float*)Kernel.data;
					
					*(wKernel + int(offset + 1)*Kernel.cols + int(offset + 1)) = 1;
					*(wKernel + int(spY0 + offset + 1)*Kernel.cols + int(spX0 + offset + 1)) = coef00;
					*(wKernel + int(spY0 + offset + 1)*Kernel.cols + int(spX1 + offset + 1)) = coef01;
					*(wKernel + int(spY1 + offset + 1)*Kernel.cols + int(spX0 + offset + 1)) = coef10;
					*(wKernel + int(spY1 + offset + 1)*Kernel.cols + int(spX1 + offset + 1)) = coef11;
					
					/*
					float maxKernel = 0;
					for (int i = 0; i < Kernel.cols*Kernel.rows; i++)
					{
						if (maxKernel < *wKernel)
							maxKernel = *wKernel;
						wKernel++;
					}
					*/
					Kernel = Kernel * 255; //(maxKernel / 255);
					//Kernel.at<float>(Kernel.rows / 2, Kernel.cols / 2) = 255;
					Kernel.convertTo(ImTemp, CV_8U);
					applyColorMap(ImTemp, ImShow, COLORMAP_JET);
					imshow("Kernel", ImShow);
					if (saveCOM)
					{
						OutFileNameFull = OutFolderName + FileNameBase + "Kernel" + "O" + ItoStrLZ(offset, 2) + "A" + ItoStrLZ(angle, 3) + ".bmp";
						imwrite(OutFileNameFull, ImShow);
					}

					if (goThru)
						waitKey(50);
					else
						waitKey(0);
				}

			}
			// find best angle
			// best angle for ofset
			int bestAngleContrast = FindBestAngleMin(Contrast, stepNr);
			float bestAngleContrastQS = BestAngleQualityScore(Contrast, stepNr, bestAngleContrast);
			
			int bestAngleEnergy = FindBestAngleMax(Energy, stepNr);
			float bestAngleEnergyQS = BestAngleQualityScore(Energy, stepNr, bestAngleEnergy);

			int bestAngleHomogenity = FindBestAngleMax(Homogenity, stepNr);
			float bestAngleHomogenityQS = BestAngleQualityScore(Homogenity, stepNr, bestAngleHomogenity);

			int bestAngleCorrelation = FindBestAngleMax(Correlation, stepNr);
			float bestAngleCorrelationQS = BestAngleQualityScore(Correlation, stepNr, bestAngleCorrelation);

//			string Temp;
//			if (offset < 10)
//				Temp = " ";
//			else
//				Temp = "";
//			cout << "offset = " << Temp << offset << " BA = "
			cout << "offset = " << ItoStrLZ(offset,2) << " BA = "
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
			Angles[bestAngleEnergy]++;
			Angles[bestAngleHomogenity]++;
			Angles[bestAngleCorrelation]++;


			AnglesC[bestAngleContrast]++;
			AnglesE[bestAngleEnergy]++;
			AnglesH[bestAngleHomogenity]++;
			AnglesR[bestAngleCorrelation]++;

			// show line on image

			//
			if (outLocal)
			{
				OutLocal += "Offset ";
				OutLocal += ItoStrLZ(offset,2);
				for (int angleIndex = 0; angleIndex < stepNr; angleIndex++)
				{
					OutLocal += "\t" + to_string(Contrast[angleIndex]);
				}
				OutLocal += "\t";
				OutLocal += "\t";
				OutLocal += "Offset ";
				OutLocal += ItoStrLZ(offset, 2);
				for (int angleIndex = 0; angleIndex < stepNr; angleIndex++)
				{
					OutLocal += "\t" + to_string(Energy[angleIndex]);
				}
				OutLocal += "\t";
				OutLocal += "\t";
				OutLocal += "Offset ";
				OutLocal += ItoStrLZ(offset,2);
				for (int angleIndex = 0; angleIndex < stepNr; angleIndex++)
				{
					OutLocal += "\t" + to_string(Homogenity[angleIndex]);
				}
				OutLocal += "\t";
				OutLocal += "\t";
				OutLocal += "Offset ";
				OutLocal += ItoStrLZ(offset,2);
				for (int angleIndex = 0; angleIndex < stepNr; angleIndex++)
				{
					OutLocal += "\t" + to_string(Correlation[angleIndex]);
				}

				OutLocal += "\n";

				OutLocal2 += "offset " + ItoStrLZ(offset, 2) + "\t";
				OutLocal2 += to_string((float)bestAngleContrast * angleStep) + "\t";
				OutLocal2 += to_string((float)bestAngleEnergy * angleStep) + "\t";
				OutLocal2 += to_string((float)bestAngleHomogenity * angleStep) + "\t";
				OutLocal2 += to_string((float)bestAngleCorrelation * angleStep) + "\t";
				OutLocal2 += "\t";
				
				OutLocal2 += "offset " + ItoStrLZ(offset, 2) + "\t";
				OutLocal2 += to_string(bestAngleContrastQS) + "\t";
				OutLocal2 += to_string(bestAngleEnergyQS) + "\t";
				OutLocal2 += to_string(bestAngleHomogenityQS) + "\t";
				OutLocal2 += to_string(bestAngleCorrelationQS) + "\t";
				OutLocal2 += "\n";

			}
		}
		int bestAngle = 0;
		int maxAngle = Angles[0];
		for (int i = 1; i < stepNr; i++)
		{
			if (maxAngle < Angles[i])
			{
				maxAngle = Angles[i];
				bestAngle = i;
			}
		}

		int bestAngleC = 0;
		int maxAngleC = AnglesC[0];
		for (int i = 1; i < stepNr; i++)
		{
			if (maxAngleC < AnglesC[i])
			{
				maxAngleC = AnglesC[i];
				bestAngleC = i;
			}
		}

		int bestAngleE = 0;
		int maxAngleE = AnglesE[0];
		for (int i = 1; i < stepNr; i++)
		{
			if (maxAngleE < AnglesE[i])
			{
				maxAngleE = AnglesE[i];
				bestAngleE = i;
			}
		}

		int bestAngleH = 0;
		int maxAngleH = AnglesH[0];
		for (int i = 1; i < stepNr; i++)
		{
			if (maxAngleH < AnglesH[i])
			{
				maxAngleH = AnglesH[i];
				bestAngleH = i;
			}
		}

		int bestAngleR = 0;
		int maxAngleR = AnglesR[0];
		for (int i = 1; i < stepNr; i++)
		{
			if (maxAngleR < AnglesR[i])
			{
				maxAngleR = AnglesR[i];
				bestAngleR = i;
			}
		}






		cout << "\n\n" << "Best Angle = " << bestAngle << " Best Angle Occurrence = " << maxAngle << "\n\n";

		if (outLocal)
		{
			OutLocal += "\n\n";
			OutLocal += ("Best Ang = \t" + to_string(bestAngle * angleStep) + "\t occurence = \t" + to_string(maxAngle));
			OutLocal += "\n";
			OutLocal += ("Best Ang Cont = \t" + to_string(bestAngleC * angleStep) + "\t occurence = \t" + to_string(maxAngleC));
			OutLocal += "\n";
			OutLocal += ("Best Ang Ener = \t" + to_string(bestAngleE * angleStep) + "\t occurence = \t" + to_string(maxAngleE));
			OutLocal += "\n";
			OutLocal += ("Best Ang Homo = \t" + to_string(bestAngleH * angleStep) + "\t occurence = \t" + to_string(maxAngleH));
			OutLocal += "\n";
			OutLocal += ("Best Ang Corr = \t" + to_string(bestAngleR * angleStep) + "\t occurence = \t" + to_string(maxAngleR));
			OutLocal += "\n\n";

			OutLocal += "Angle\t";
			for (int i = 0; i < stepNr; i++)
			{
				OutLocal += to_string(i * angleStep);
				OutLocal += "\t";
			}
			OutLocal += "\n";
			OutLocal += "Occurence\t";
			for (int i = 0; i < stepNr; i++)
			{
				OutLocal += to_string(Angles[i]);
				OutLocal += "\t";
			}
			OutLocal += "\n";
			OutLocal += "Occ Cont\t";
			for (int i = 0; i < stepNr; i++)
			{
				OutLocal += to_string(AnglesC[i]);
				OutLocal += "\t";
			}
			OutLocal += "\n";
			OutLocal += "Occ Ener\t";
			for (int i = 0; i < stepNr; i++)
			{
				OutLocal += to_string(AnglesE[i]);
				OutLocal += "\t";
			}
			OutLocal += "\n";
			OutLocal += "Occ Hono\t";
			for (int i = 0; i < stepNr; i++)
			{
				OutLocal += to_string(AnglesH[i]);
				OutLocal += "\t";
			}
			OutLocal += "\n";
			OutLocal += "Occ Corr\t";
			for (int i = 0; i < stepNr; i++)
			{
				OutLocal += to_string(AnglesR[i]);
				OutLocal += "\t";
			}
			
			OutLocal += "\n\n" + OutLocal2;
			
			string TextFileName = OutFolderName + FileNameBase + ".txt";
			ofstream out(TextFileName);
			out << OutLocal;
			out.close();

			//to_string(bestAngle * angleStep) + "\t occurence = \t" + to_string(maxAngle))

			CommonOutFName += InFileName +"\t";
			CommonOutBA += to_string(bestAngle * angleStep) + "\t";
			CommonOutBACont += to_string(bestAngleC * angleStep) + "\t";
			CommonOutBAEner += to_string(bestAngleE * angleStep) + "\t";
			CommonOutBAHomo += to_string(bestAngleH * angleStep) + "\t";
			CommonOutBACorr += to_string(bestAngleR * angleStep) + "\t";
			CommonOutBANr += to_string(maxAngle)+ "\t";
			CommonOutBANrCont += to_string(maxAngleC) + "\t";
			CommonOutBANrEner += to_string(maxAngleE) + "\t";
			CommonOutBANrHomo += to_string(maxAngleH) + "\t";
			CommonOutBANrCorr += to_string(maxAngleR) + "\t";

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
	CommonOut = "Common Out File\n";
	CommonOut += "Folder Name\t" + OutFolderName + "\n\n";
	CommonOut += CommonOutFName + "\n\n";
	CommonOut += CommonOutBA + "\n";
	CommonOut += CommonOutBACont + "\n";
	CommonOut += CommonOutBAEner + "\n";
	CommonOut += CommonOutBAHomo + "\n";
	CommonOut += CommonOutBACorr + "\n\n";

	CommonOut += CommonOutBANr + "\n";
	CommonOut += CommonOutBANrCont + "\n";
	CommonOut += CommonOutBANrEner + "\n";
	CommonOut += CommonOutBANrHomo + "\n";
	CommonOut += CommonOutBANrCorr + "\n";

	string CommTextFileName = OutFolderName + "CommOut" + ".txt";
	ofstream out(CommTextFileName);
	out << CommonOut;
	out.close();
	
	cin >> StrTemp;
	return 0;
}


