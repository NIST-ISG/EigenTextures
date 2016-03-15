// ConsoleApplication1.cpp : Defines the entry point for the console application.
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

#include <tinyxml.h>
#include <tinystr.h>

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

	string InFileName;// = "D:\\Summona\\Sumona\\Sumona\\Mineral_Day28_Image6\\actin\\image6_actin_Z5.ome.tif";
	string InFileName2;
	string OutFileName;// = "D:\\Summona\\Sumona\\Sumona\\Mineral_Day28_Image6\\actinOut1\\image6_actin_Z5.ome.bmp";
	string OutFileName2;// = "D:\\Summona\\Sumona\\Sumona\\Mineral_Day28_Image6\\actinOut2\\image6_actin_Z5.ome.bmp";

	string InFolderName, InFolderName2, FileNameBase, FileNameBase2, InFileExtension, InFileExtension2, OutFolderName, OutFolderName2;

	string InStr;

	bool displayResult = 1;
	//bool displayCom = 0;
	//bool saveResult = 0;
	//bool commonResult = 0;
	//bool outLocal = 1;
	//bool outScreen = 0;
	bool goThru = 1;
	bool showTiles = 1;

	bool useSecondImage = 1;

	bool textOut = 1;

	int normalisation = 1;

	int binCount = 16;

	int minOfset = 2;
	int maxOfset = 16;
	bool useContrast;
	bool useEnergy;
	bool useHomogeneity;
	bool useCorrelation;
	int minHit = 4;

	bool useMinMean = 0;
	float minMean = 400.0;

	float minNorm = 0.0;
	float maxNorm = 4000.0;

	float minNormGlobal = 0.0;
	float maxNormGlobal = 20000.0;

	float displayMax = 20000.0;
	float displayMin = -4000.0;

	float displayMax2 = 20000.0;
	float displayMin2 = -4000.0;

	int maxTileX = 64;
	int maxTileY = 64;
	int tileLineThickness = 2;
	int lineHalfLength = 20;
	//	32767.0;

	float angleStep = 1;
	int interpolation = INTER_CUBIC;

	cout << "\n";

	// arguments read and decode 
	if (argc < 2)
	{
		cout << "\nTo few arguments.";
		return 0;
	}

	string ConfigFileName = argv[1];
	//string ConfigFileName = "D:\\FeatureExtractionProject\\WaveletInput.xml";
	TiXmlDocument doc(ConfigFileName.c_str());
	doc.LoadFile();

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);
	TiXmlHandle hParameters(0);
	pElem = hDoc.FirstChildElement().Element();

	cout << pElem->Value();
	cout << "\n";

	hRoot = TiXmlHandle(pElem);
	// file to open
	pElem = hRoot.FirstChild("input").FirstChild("directory").Element();
	if (!pElem)
	{
		cout << "invalid directory";
		return 0;
	}
	InFolderName = pElem->GetText();

	pElem = hRoot.FirstChild("input").FirstChild("filebase").Element();
	if (!pElem)
	{
		cout << "invalid filename";
		return 0;
	}
	FileNameBase = pElem->GetText();

	pElem = hRoot.FirstChild("input").FirstChild("extension").Element();
	if (!pElem)
	{
		cout << "invalid extension";
		return 0;
	}
	InFileExtension = pElem->GetText();
	// second file input
	pElem = hRoot.FirstChild("input2").FirstChild("directory").Element();
	if (!pElem)
	{
		cout << "invalid input 2 directory";
		return 0;
	}
	InFolderName2 = pElem->GetText();

	pElem = hRoot.FirstChild("input2").FirstChild("filebase").Element();
	if (!pElem)
	{
		cout << "invalid input 2 filename";
		return 0;
	}
	FileNameBase2 = pElem->GetText();

	pElem = hRoot.FirstChild("input2").FirstChild("extension").Element();
	if (!pElem)
	{
		cout << "invalid input 2 extension";
		return 0;
	}
	InFileExtension2 = pElem->GetText();

	// output folder
	pElem = hRoot.FirstChild("output").FirstChild("directory").Element();
	if (!pElem)
	{
		cout << "invalid out directory";
		return 0;
	}
	OutFolderName = pElem->GetText();
	// output folder 2
	pElem = hRoot.FirstChild("output2").FirstChild("directory").Element();
	if (!pElem)
	{
		cout << "invalid out directory2";
		return 0;
	}
	OutFolderName2 = pElem->GetText();



	pElem = hRoot.FirstChild("parameters").Element();
	if (!pElem)
	{
		cout << "no parameters section";
		return 0;
	}
	hParameters = TiXmlHandle(pElem);


	string ValStr;

	pElem = hParameters.FirstChild("displayResult").Element();
	if (!pElem)
	{
		cout << "invalid display result";
		return 0;
	}
	ValStr = pElem->GetText();
	if (ValStr == "Y")
	{
		displayResult = 1;
	}
	else
		displayResult = 0;

	pElem = hParameters.FirstChild("showTiles").Element();
	if (!pElem)
	{
		cout << "invalid showTiles";
		return 0;
	}
	ValStr = pElem->GetText();
	if (ValStr == "Y")
	{
		showTiles = 1;
	}
	else
		showTiles = 0;

	pElem = hParameters.FirstChild("goThru").Element();
	if (!pElem)
	{
		cout << "invalid goThrou";
		return 0;
	}
	ValStr = pElem->GetText();
	if (ValStr == "Y")
	{
		goThru = 1;
	}
	else
		goThru = 0;

	pElem = hParameters.FirstChild("useSecondImage").Element();
	if (!pElem)
	{
		cout << "invalid use second image";
		return 0;
	}
	ValStr = pElem->GetText();
	if (ValStr == "Y")
	{
		useSecondImage = 1;
	}
	else
		useSecondImage = 0;

	pElem = hParameters.FirstChild("textOut").Element();
	if (!pElem)
	{
		cout << "invalid textOut";
		return 0;
	}
	ValStr = pElem->GetText();
	if (ValStr == "Y")
	{
		textOut = 1;
	}
	else
		textOut = 0;

	pElem = hParameters.FirstChild("normalisation").Element();
	if (!pElem)
	{
		cout << "invalid normalisation";
		return 0;
	}
	ValStr = pElem->GetText();
	normalisation = stoi(ValStr);

	pElem = hParameters.FirstChild("binCount").Element();
	if (!pElem)
	{
		cout << "invalid binCount";
		return 0;
	}
	ValStr = pElem->GetText();
	binCount = stoi(ValStr);

	pElem = hParameters.FirstChild("minOfset").Element();
	if (!pElem)
	{
		cout << "invalid minOfset";
		return 0;
	}
	ValStr = pElem->GetText();
	minOfset = stoi(ValStr);

	pElem = hParameters.FirstChild("maxOfset").Element();
	if (!pElem)
	{
		cout << "invalid maxOfset";
		return 0;
	}
	ValStr = pElem->GetText();
	maxOfset = stoi(ValStr);

	pElem = hParameters.FirstChild("useContrast").Element();
	if (!pElem)
	{
		cout << "invalid useContrast";
		return 0;
	}
	ValStr = pElem->GetText();
	if (ValStr == "Y")
	{
		useContrast = 1;
	}
	else
		useContrast = 0;

	pElem = hParameters.FirstChild("useEnergy").Element();
	if (!pElem)
	{
		cout << "invalid useEnergy";
		return 0;
	}
	ValStr = pElem->GetText();
	if (ValStr == "Y")
	{
		useEnergy = 1;
	}
	else
		useEnergy = 0;

	pElem = hParameters.FirstChild("useHomogeneity").Element();
	if (!pElem)
	{
		cout << "invalid useHomogeneity";
		return 0;
	}
	ValStr = pElem->GetText();
	if (ValStr == "Y")
	{
		useHomogeneity = 1;
	}
	else
		useHomogeneity = 0;

	pElem = hParameters.FirstChild("useCorrelation").Element();
	if (!pElem)
	{
		cout << "invalid useCorrelation";
		return 0;
	}
	ValStr = pElem->GetText();
	if (ValStr == "Y")
	{
		useCorrelation = 1;
	}
	else
		useCorrelation = 0;

	pElem = hParameters.FirstChild("minHit").Element();
	if (!pElem)
	{
		cout << "invalid minHit";
		return 0;
	}
	ValStr = pElem->GetText();
	minHit = stoi(ValStr);

	pElem = hParameters.FirstChild("useMinMean").Element();
	if (!pElem)
	{
		cout << "invalid useMinMean";
		return 0;
	}
	ValStr = pElem->GetText();
	if (ValStr == "Y")
	{
		useMinMean = 1;
	}
	else
		useMinMean = 0;

	pElem = hParameters.FirstChild("minMean").Element();
	if (!pElem)
	{
		cout << "invalid minMean";
		return 0;
	}
	ValStr = pElem->GetText();
	minMean = stoi(ValStr);

	pElem = hParameters.FirstChild("minNorm").Element();
	if (!pElem)
	{
		cout << "invalid minNorm";
		return 0;
	}
	ValStr = pElem->GetText();
	minNorm = stof(ValStr);

	pElem = hParameters.FirstChild("maxNorm").Element();
	if (!pElem)
	{
		cout << "invalid maxNorm";
		return 0;
	}
	ValStr = pElem->GetText();
	maxNorm = stof(ValStr);

	pElem = hParameters.FirstChild("minNormGlobal").Element();
	if (!pElem)
	{
		cout << "invalid minNormGlobal";
		return 0;
	}
	ValStr = pElem->GetText();
	minNormGlobal = stof(ValStr);

	pElem = hParameters.FirstChild("maxNormGlobal").Element();
	if (!pElem)
	{
		cout << "invalid maxNormGlobal";
		return 0;
	}
	ValStr = pElem->GetText();
	maxNormGlobal = stof(ValStr);


	pElem = hParameters.FirstChild("displayMax").Element();
	if (!pElem)
	{
		cout << "invalid displayMax";
		return 0;
	}
	ValStr = pElem->GetText();
	displayMax = stof(ValStr);

	pElem = hParameters.FirstChild("displayMin").Element();
	if (!pElem)
	{
		cout << "invalid displayMin";
		return 0;
	}
	ValStr = pElem->GetText();
	displayMin = stof(ValStr);

	pElem = hParameters.FirstChild("displayMax2").Element();
	if (!pElem)
	{
		cout << "invalid displayMax2";
		return 0;
	}
	ValStr = pElem->GetText();
	displayMax2 = stof(ValStr);

	pElem = hParameters.FirstChild("displayMin2").Element();
	if (!pElem)
	{
		cout << "invalid displayMin2";
		return 0;
	}
	ValStr = pElem->GetText();
	displayMin2 = stof(ValStr);

	pElem = hParameters.FirstChild("maxTileX").Element();
	if (!pElem)
	{
		cout << "invalid maxTileX";
		return 0;
	}
	ValStr = pElem->GetText();
	maxTileX = stoi(ValStr);

	pElem = hParameters.FirstChild("maxTileY").Element();
	if (!pElem)
	{
		cout << "invalid MaxTileY";
		return 0;
	}
	ValStr = pElem->GetText();
	maxTileY = stoi(ValStr);

	pElem = hParameters.FirstChild("tileLineThickness").Element();
	if (!pElem)
	{
		cout << "invalid tileLineThickness";
		return 0;
	}
	ValStr = pElem->GetText();
	tileLineThickness = stoi(ValStr);

	pElem = hParameters.FirstChild("lineHalfLength").Element();
	if (!pElem)
	{
		cout << "invalid lineHalfLength";
		return 0;
	}
	ValStr = pElem->GetText();
	lineHalfLength = stoi(ValStr);

	pElem = hParameters.FirstChild("angleStep").Element();
	if (!pElem)
	{
		cout << "invalid angleStep";
		return 0;
	}
	ValStr = pElem->GetText();
	angleStep = stof(ValStr);

	cout << "Input folder name" << "\t" << InFolderName << "\n";
	cout << "Input 2 folder name" << "\t" << InFolderName2 << "\n";

	cout << "Output folder name" << "\t" << OutFolderName << "\n";
	cout << "Output 2 folder name" << "\t" << OutFolderName2 << "\n";

	cout << "Input file name" << "\t" << FileNameBase << InFileExtension << "\n";
	cout << "Input 2 file name" << "\t" << FileNameBase2 << InFileExtension2 << "\n";

	InFileName = InFolderName + FileNameBase + InFileExtension;
	InFileName2 = InFolderName2 + FileNameBase2 + InFileExtension2;

	OutFileName = OutFolderName + FileNameBase + ".bmp";
	OutFileName2 = OutFolderName2 + FileNameBase2 + ".bmp";


	cout << "In file  - " << InFileName << "\n";
	cout << "Out file" << OutFileName << "\n";

	float featCount = 0;
	if (useContrast)
		featCount++;
	if (useEnergy)
		featCount++;
	if (useHomogeneity)
		featCount++;
	if (useCorrelation)
		featCount++;

	// read first file
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFileA((InFolderName + "*" + InFileExtension).c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		cout << " file not found";
		cin >> InStr;
		return 0;
	}
	//handle files

	while (hFind != INVALID_HANDLE_VALUE)
	{
		string FoundFileName = FindFileData.cFileName;
		// file name base
		int lastIndex = FoundFileName.find_last_of(".");
		FileNameBase = FoundFileName.substr(0, lastIndex);
		InFileName = InFolderName + FoundFileName;

		Mat ImIn = imread(InFileName, CV_LOAD_IMAGE_ANYDEPTH);
		if (!ImIn.size)
		{
			cout << "no such file";
			return 0;
		}

		int maxX, maxY, maxXY;
		maxX = ImIn.cols;
		maxY = ImIn.rows;
		maxXY = maxX * maxY;
		Mat ImTemp, ImShow, ImShowGlobal, ImScale;
	
		// conversion to float
		Mat ImInF, ImInFTemp;
		ImIn.convertTo(ImInF, CV_32F);
		//	float displayCoefRatio, displayCoefOffset;

		if (displayResult)
		{
			namedWindow("Image", WINDOW_AUTOSIZE);
			namedWindow("ImageSmall", WINDOW_AUTOSIZE);
			if (useSecondImage)
				namedWindow("Image2", WINDOW_AUTOSIZE);
		}


		float gainCoef = 255.0 / (displayMax - displayMin);
		float offsetCoef = gainCoef * displayMin;
		ImInFTemp = ImInF * gainCoef - offsetCoef;
		ImInFTemp.convertTo(ImTemp, CV_8U);
		applyColorMap(ImTemp, ImShowGlobal, COLORMAP_JET);

		if (showTiles)
		{
			for (int x = maxTileX; x < maxX; x += maxTileX)
			{
				line(ImShowGlobal, Point(x, 0), Point(x, maxY - 1), Scalar(0.0, 0.0, 0.0, 0.0), tileLineThickness);
			}
			for (int y = maxTileY; y < maxY; y += maxTileY)
			{
				line(ImShowGlobal, Point(0, y), Point(maxX - 1, y), Scalar(0.0, 0.0, 0.0, 0.0), tileLineThickness);
			}
		}


		if (displayResult)
		{
			imshow("Image", ImShowGlobal);
		}




		Mat SmallIm, SmallIm2;
		string OutString = "yRoiNr\txRoiNr\t\direction\tDDC\tmean Tile\tstd Tile\tmean tile 2\tstd Tile 2\n";
		int xTileNr = 0;
		int yTileNr = 0;
		for (int y = 0; (y + maxTileY) <= maxY; y += maxTileY)
		{
			for (int x = 0; (x + maxTileX) <= maxX; x += maxTileX)
			{
				ImInF(Rect(x, y, maxTileX, maxTileY)).copyTo(SmallIm);
	
				float meanSmallIm = NAN;
				float stdSmallIm = NAN;
				float meanSmallIm2 = NAN;
				float stdSmallIm2 = NAN;
				if (textOut)
				{
					MatFMeanAndStd(SmallIm, &meanSmallIm, &stdSmallIm);
				}
				switch (normalisation)
				{
				case 1:
					NormParamsMinMax(SmallIm, &maxNorm, &minNorm);
					break;
				case 2:
					maxNorm = maxNormGlobal;
					minNorm = minNormGlobal;
					break;
				case 3:
					NormParamsMeanP3Std(SmallIm, &maxNorm, &minNorm);
					break;
				case 4:
					maxNorm = maxNormGlobal;
					minNorm = minNormGlobal;
					break;
				default:
					break;
				}
				//imwrite(OutFilename)
				float normCoef;
				normCoef = (maxNorm - minNorm) / 256;
				ImInFTemp = (SmallIm - minNorm) / normCoef;
				ImInFTemp.convertTo(ImTemp, CV_8U);
				applyColorMap(ImTemp, ImShow, COLORMAP_JET);
				imshow("ImageSmall", ImShow);
				//waitKey(100);
				cout << "y = " << yTileNr << " x = " << xTileNr << "  ";
				float Energy[stepNr];
				float Contrast[stepNr];
				float Correlation[stepNr];
				float Homogenity[stepNr];
				string OutLocal = "";
				OutLocal += "normalisation ";
				switch (normalisation)
				{
				case 1:
					OutLocal += "Min Max Local";
					break;
				case 2:
					OutLocal += "Min Max Global";
					break;
				case 3:
					OutLocal += "Mean +/- 3std Local";
					break;
				case 4:
					OutLocal += "Mean +/- 3std Global";
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
					OutLocal += "BiLinear";
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

				//cout << OutLocal + "\n";

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
					}

					// best angle for ofset
					int bestAngleContrast, bestAngleEnergy, bestAngleHomogenity, bestAngleCorrelation;
					if (useContrast)
						bestAngleContrast = FindBestAngleMin(Contrast, stepNr);
					if (useEnergy)
						bestAngleEnergy = FindBestAngleMax(Energy, stepNr);
					if (useHomogeneity)
						bestAngleHomogenity = FindBestAngleMax(Homogenity, stepNr);
					if (useCorrelation)
						bestAngleCorrelation = FindBestAngleMax(Correlation, stepNr);

					string Temp;
					if (offset < 10)
						Temp = " ";
					else
						Temp = "";
					//				cout << "offset = " << Temp << offset << " b a = "
					//					<< (float)bestAngleCorrelation * angleStep << " "
					//					<< "\n";

					if (useContrast)
						Angles[bestAngleContrast]++;
					if (useEnergy)
						Angles[bestAngleEnergy]++;
					if (useHomogeneity)
						Angles[bestAngleHomogenity]++;
					if (useCorrelation)
						Angles[bestAngleCorrelation]++;

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
				cout << "  " << "A = " << bestAngle << "  c = " << maxAngle << "  ";
				// show line on image
				int barCenterX = maxTileX / 2 + x;
				int barCenterY = maxTileY / 2 + y;
				double lineLength = (double)(lineHalfLength) / (maxOfset - minOfset + 1) / featCount;
				int lineOffsetX = (int)round(lineLength * maxAngle * sin((double)(bestAngle)* PI / 180.0));
				int lineOffsetY = (int)round(lineLength * maxAngle * cos((double)(bestAngle)* PI / 180.0));

				//
				bool meanCondition = 0;
				if (useMinMean)
				{
					if (meanSmallIm >= minMean)
						meanCondition = true;
					else
						meanCondition = false;
				}
				else
					meanCondition = true;
				if ((maxAngle >= minHit) && meanCondition)
				{
					line(ImShowGlobal, Point(barCenterX - lineOffsetX, barCenterY - lineOffsetY), Point(barCenterX + lineOffsetX, barCenterY + lineOffsetY), Scalar(0, 0.0, 0.0, 0.0), 3);
					if (useSecondImage)
					{
						line(Im2ShowGlobal, Point(barCenterX - lineOffsetX, barCenterY - lineOffsetY), Point(barCenterX + lineOffsetX, barCenterY + lineOffsetY), Scalar(0, 0.0, 0.0, 0.0), 3);
					}
				}
				imshow("Image", ImShowGlobal);
				if (useSecondImage)
				{
					imshow("Image2", Im2ShowGlobal);
				}
				waitKey(100);
				//xTileNr++;
				if (textOut)
				{
					OutString += to_string(yTileNr) + "\t";
					OutString += to_string(xTileNr) + "\t";
					if (maxAngle >= minHit)
					{
						OutString += to_string((float)bestAngle * angleStep) + "\t";
						OutString += to_string((float)(maxAngle) / (float)(maxOfset - minOfset)) + "\t";
					}
					else
					{
						OutString += "NAN\t";
						OutString += "NAN\t";
					}
					OutString += to_string(meanSmallIm) + "\t";
					OutString += to_string(stdSmallIm) + "\t";
					if (useSecondImage)
					{
						OutString += to_string(meanSmallIm2) + "\t";
						OutString += to_string(stdSmallIm2) + "\t";
					}
					OutString += "\n";
					cout << to_string(meanSmallIm) << "  " << to_string(stdSmallIm) << "  ";
					cout << to_string(meanSmallIm2) << "  " << to_string(stdSmallIm2);
					cout << "\n";
				}
				xTileNr++;
			}
			yTileNr++;
			xTileNr = 0;
		}
		OutFileName = OutFolderName + FileNameBase + ".bmp";
		imwrite(OutFileName, ImShowGlobal);
		if (useSecondImage)
			imwrite(OutFileName2, Im2ShowGlobal);
		if (textOut)
		{
			string CommTextFileName = OutFolderName + FileNameBase + ".txt";
			ofstream out(CommTextFileName);
			out << OutString;
			out.close();
		}
		if (useSecondImage)
		{
			imwrite(OutFileName2, Im2ShowGlobal);
		}
		if (goThru)
			waitKey(50);
		else
			waitKey(0);

		if (!FindNextFileA(hFind, &FindFileData))
			break;


	}
	return 0;
}

