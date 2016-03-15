// ImageParamsCheckV001.cpp : Defines the entry point for the console application.
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
//#include "RedundantWaveletLib.h"
#include "NormalizationLib.h"
//#include "HaralickLib.h"
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


	float featCount = 0;
	if (useContrast)
		featCount++;
	if (useEnergy)
		featCount++;
	if (useHomogeneity)
		featCount++;
	if (useCorrelation)
		featCount++;
	// out common string
	string OutStringCommon = "";
	OutStringCommon += "Config Nile Name:\t" + ConfigFileName + "\n";
	OutStringCommon += "Input Directory:\t" + InFolderName + "\n";
	OutStringCommon += "Input Directory 2:\t" + InFolderName2 + "\n";
	OutStringCommon += "Output Directory:\t" + OutFolderName + "\n";
	OutStringCommon += "Output Directory 2:\t" + OutFolderName2 + "\n";
	
	OutStringCommon += "Display result:\t";
	if (displayResult)
		OutStringCommon += "T";
	else
		OutStringCommon += "N";
	OutStringCommon += "\n";

	OutStringCommon += "Show tiles:\t";
	if (showTiles)
		OutStringCommon += "T";
	else
		OutStringCommon += "N";
	OutStringCommon += "\n";

	OutStringCommon += "Go thru:\t";
	if (goThru)
		OutStringCommon += "T";
	else
		OutStringCommon += "N";
	OutStringCommon += "\n";

	OutStringCommon += "Use second image:\t";
	if (useSecondImage)
		OutStringCommon += "T";
	else
		OutStringCommon += "N";
	OutStringCommon += "\n";

	OutStringCommon += "Text out:\t";
	if (textOut)
		OutStringCommon += "T";
	else
		OutStringCommon += "N";
	OutStringCommon += "\n";

	OutStringCommon += "Normalisation:\t";
	OutStringCommon += to_string(normalisation);
	OutStringCommon += "\n";

	OutStringCommon += "Bin count:\t";
	OutStringCommon += to_string(binCount);
	OutStringCommon += "\n";

	OutStringCommon += "Min Ofset:\t";
	OutStringCommon += to_string(minOfset);
	OutStringCommon += "\n";

	OutStringCommon += "Max Ofset:\t";
	OutStringCommon += to_string(maxOfset);
	OutStringCommon += "\n";


	OutStringCommon += "Use Contrast:\t";
	if (useContrast)
		OutStringCommon += "T";
	else
		OutStringCommon += "N";
	OutStringCommon += "\n";

	OutStringCommon += "Use Energy:\t";
	if (useEnergy)
		OutStringCommon += "T";
	else
		OutStringCommon += "N";
	OutStringCommon += "\n";

	OutStringCommon += "Use Homogeneity:\t";
	if (useHomogeneity)
		OutStringCommon += "T";
	else
		OutStringCommon += "N";
	OutStringCommon += "\n";

	OutStringCommon += "Use Correlation:\t";
	if (useCorrelation)
		OutStringCommon += "T";
	else
		OutStringCommon += "N";
	OutStringCommon += "\n";

	OutStringCommon += "Min Hit:\t";
	OutStringCommon += to_string(minHit);
	OutStringCommon += "\n";

	OutStringCommon += "UseMinMean:\t";
	if (useMinMean)
		OutStringCommon += "T";
	else
		OutStringCommon += "N";
	OutStringCommon += "\n";

	OutStringCommon += "Min Mean:\t";
	OutStringCommon += to_string(minMean);
	OutStringCommon += "\n";

	OutStringCommon += "Min Norm:\t";
	OutStringCommon += to_string(minNorm);
	OutStringCommon += "\n";

	OutStringCommon += "Max Norm:\t";
	OutStringCommon += to_string(maxNorm);
	OutStringCommon += "\n";

	OutStringCommon += "Min Norm Global:\t";
	OutStringCommon += to_string(minNormGlobal);
	OutStringCommon += "\n";

	OutStringCommon += "Max Norm Global:\t";
	OutStringCommon += to_string(maxNormGlobal);
	OutStringCommon += "\n";

	OutStringCommon += "Display Max:\t";
	OutStringCommon += to_string(displayMax);
	OutStringCommon += "\n";

	OutStringCommon += "Display Min:\t";
	OutStringCommon += to_string(displayMin);
	OutStringCommon += "\n";

	OutStringCommon += "Display Max2:\t";
	OutStringCommon += to_string(displayMax2);
	OutStringCommon += "\n";

	OutStringCommon += "Display Min2:\t";
	OutStringCommon += to_string(displayMin2);
	OutStringCommon += "\n";

	OutStringCommon += "Max Tile X:\t";
	OutStringCommon += to_string(maxTileX);
	OutStringCommon += "\n";

	OutStringCommon += "Max TileY:\t";
	OutStringCommon += to_string(maxTileY);
	OutStringCommon += "\n";

	OutStringCommon += "Tile Line Thickness>:\t";
	OutStringCommon += to_string(tileLineThickness);
	OutStringCommon += "\n";

	OutStringCommon += "Line Half Length:\t";
	OutStringCommon += to_string(lineHalfLength);
	OutStringCommon += "\n";

	OutStringCommon += "Angle Step:\t";
	OutStringCommon += to_string(angleStep);
	OutStringCommon += "\n";

	OutStringCommon += "Interpolation:\t";
	OutStringCommon += to_string(interpolation);
	OutStringCommon += "\n";

	OutStringCommon += "\n";

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
	string OutString = OutStringCommon;
	OutString += "\n";
	OutString += "Input file name:\t";
	//OutString += "Input file name 2:\t";
	//OutString += "Output file name:\t";
	//OutString += "Output file name 2:\t";
	OutString += "Min Intensity:\t";
	OutString += "Max Intensity:\t";
	OutString += "Mean Intensity:\t";
	OutString += "Std Intensity:\t";
	OutString += "Perc1 \t";
	OutString += "Perc99 \t";
	OutString += "Perm1 \t";
	OutString += "Perm999 \t";
	OutString += "\t";
	for (int i = 0; i < 1048; i++)
		OutString += "Hist" + to_string(i*64);

	OutString += "\n";
	while (hFind != INVALID_HANDLE_VALUE)
	{
		string FoundFileName = FindFileData.cFileName;
		// file name base
		int lastIndex = FoundFileName.find_last_of(".");
		FileNameBase = FoundFileName.substr(0, lastIndex);
		InFileName = InFolderName + FoundFileName;
		OutFileName = OutFolderName + FileNameBase + ".bmp";

		cout << "In file  - " << InFileName << "\n";
//		cout << "Out file - " << OutFileName << "\n";

		OutString += FoundFileName + "\t";
	
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

		//Mat Roi = Mat::ones(maxY, maxX, CV_16U);

		// conversion to float
		Mat ImInF, ImInFTemp;
		ImIn.convertTo(ImInF, CV_32F);

		float meanIm, stdIm;
		int minIm, maxIm, perc1, perc99, perm1, perm999;
		int *Histogram = 0;
		Histogram = MatFMeanAndStd(ImInF, &meanIm, &stdIm, &minIm, &maxIm);
		int *HistogramBined = 0;
		HistogramBined = FistogramFeatures(Histogram, 1, &perc1, &perc99, &perm1, &perm999);

		OutString += to_string(minIm) + "\t";
		OutString += to_string(maxIm) + "\t";
		OutString += to_string(meanIm) + "\t";
		OutString += to_string(stdIm) + "\t";

		OutString += to_string(perc1) + "\t";
		OutString += to_string(perc99) + "\t";
		OutString += to_string(perm1) + "\t";
		OutString += to_string(perm999) + "\t";

		OutString += "\t";

		for (int i = 0; i < 1048; i++)
			OutString += to_string(HistogramBined[i]) + "\t";

		OutString += "\n";

		delete Histogram;
		Histogram = 0;
		delete HistogramBined;
		HistogramBined = 0;

		Mat ImTemp, ImShow, ImShowGlobal, ImScale;

		if (displayResult)
		{
			namedWindow("Image", WINDOW_AUTOSIZE);
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

		imwrite(OutFileName, ImShowGlobal);
		if (goThru)
			waitKey(50);
		else
			waitKey(0);

		if (!FindNextFileA(hFind, &FindFileData))
			break;

	}
	string CommTextFileName = OutFolderName + "Common" + ".txt";
	ofstream out(CommTextFileName);
	out << OutString;
	out.close();

	return 0;
}


