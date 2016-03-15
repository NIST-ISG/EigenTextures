// ImageParamsCheckV002.cpp : Defines the entry point for the console application.
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

//using namespace cv;
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

	bool useFixtDispNorm = 0;

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

	pElem = hParameters.FirstChild("useFixtDispNorm").Element();
	if (!pElem)
	{
		cout << "invalid useFixtDispNorm";
		return 0;
	}
	ValStr = pElem->GetText();
	if (ValStr == "Y")
	{
		useFixtDispNorm = 1;
	}
	else
		useFixtDispNorm = 0;

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
		OutFileName = OutFolderName + FileNameBase + ".bmp";

		cout << "In file  - " << InFileName << "\n";
		cout << "Out file - " << OutFileName << "\n";

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

		switch (normalisation)
		{
		case 1:
			NormParamsMinMax(ImInF, &maxNormGlobal, &minNormGlobal);
			break;
		case 2:
			NormParamsMinMax(ImInF, &maxNormGlobal, &minNormGlobal);
			break;
		case 3:
			NormParamsMeanP3Std(ImInF, &maxNormGlobal, &minNormGlobal);
			break;
		case 4:
			NormParamsMeanP3Std(ImInF, &maxNormGlobal, &minNormGlobal);
			break;
		case 5:
			NormParams1to99perc(ImInF, &maxNormGlobal, &minNormGlobal);
			break;
		case 6:
			NormParams1to99perc(ImInF, &maxNormGlobal, &minNormGlobal);
			break;

		default:
			break;
		}

		Mat ImTemp, ImShow, ImShowGlobal, ImScale;

		//	float displayCoefRatio, displayCoefOffset;

		if (displayResult)
		{
			namedWindow("Image", WINDOW_AUTOSIZE);

			if (!useFixtDispNorm)
			{
				displayMax = maxNormGlobal;
				displayMin = minNormGlobal;
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
		
			imshow("Image", ImShowGlobal);
			waitKey(100);
		}

		Mat SmallIm;

		string OutString;

		OutString += "Config Nile Name:\t" + ConfigFileName + "\n";
		OutString += "Input Directory:\t" + InFolderName + "\n";
		OutString += "Input file name:\t" + FoundFileName + "\n";
		OutString += "Input Directory 2:\t" + InFolderName2 + "\n";
		OutString += "Input file name 2:\t" + FileNameBase2 + InFileExtension2 + "\n";
		OutString += "Output Directory:\t" + OutFolderName + "\n";
		OutString += "Output file name:\t" + FileNameBase + ".bmp" + "\n";
		OutString += "Output Directory 2:\t" + OutFolderName2 + "\n";
		OutString += "Output file name 2:\t" + FileNameBase2 + ".bmp" + "\n";

		OutString += "Display result:\t";
		if (displayResult)
			OutString += "Y";
		else
			OutString += "N";
		OutString += "\n";

		OutString += "Show tiles:\t";
		if (showTiles)
			OutString += "Y";
		else
			OutString += "N";
		OutString += "\n";

		OutString += "Go thru:\t";
		if (goThru)
			OutString += "Y";
		else
			OutString += "N";
		OutString += "\n";

		OutString += "Use second image:\t";
		if (useSecondImage)
			OutString += "Y";
		else
			OutString += "N";
		OutString += "\n";

		OutString += "Text out:\t";
		if (textOut)
			OutString += "Y";
		else
			OutString += "N";
		OutString += "\n";

		OutString += "Normalisation:\t";
		OutString += to_string(normalisation);
		OutString += "\n";

		OutString += "Bin count:\t";
		OutString += to_string(binCount);
		OutString += "\n";

		OutString += "Min Ofset:\t";
		OutString += to_string(minOfset);
		OutString += "\n";

		OutString += "Max Ofset:\t";
		OutString += to_string(maxOfset);
		OutString += "\n";

		OutString += "Use Contrast:\t";
		if (useContrast)
			OutString += "Y";
		else
			OutString += "N";
		OutString += "\n";

		OutString += "Use Energy:\t";
		if (useEnergy)
			OutString += "Y";
		else
			OutString += "N";
		OutString += "\n";

		OutString += "Use Homogeneity:\t";
		if (useHomogeneity)
			OutString += "Y";
		else
			OutString += "N";
		OutString += "\n";

		OutString += "Use Correlation:\t";
		if (useCorrelation)
			OutString += "Y";
		else
			OutString += "N";
		OutString += "\n";

		OutString += "Min Hit:\t";
		OutString += to_string(minHit);
		OutString += "\n";

		OutString += "Use Min Mean:\t";
		if (useMinMean)
			OutString += "Y";
		else
			OutString += "N";
		OutString += "\n";

		OutString += "Min Mean:\t";
		OutString += to_string(minMean);
		OutString += "\n";

		OutString += "Min Norm:\t";
		OutString += to_string(minNorm);
		OutString += "\n";

		OutString += "Max Norm:\t";
		OutString += to_string(maxNorm);
		OutString += "\n";

		OutString += "Min Norm Global:\t";
		OutString += to_string(minNormGlobal);
		OutString += "\n";

		OutString += "Max Norm Global:\t";
		OutString += to_string(maxNormGlobal);
		OutString += "\n";

		OutString += "Use fixt display normalisation:\t";
		if (useFixtDispNorm)
			OutString += "Y";
		else
			OutString += "N";
		OutString += "\n";

		OutString += "Display Max:\t";
		OutString += to_string(displayMax);
		OutString += "\n";

		OutString += "Display Min:\t";
		OutString += to_string(displayMin);
		OutString += "\n";

		OutString += "Display Max2:\t";
		OutString += to_string(displayMax2);
		OutString += "\n";

		OutString += "Display Min2:\t";
		OutString += to_string(displayMin2);
		OutString += "\n";

		OutString += "Max Tile X:\t";
		OutString += to_string(maxTileX);
		OutString += "\n";

		OutString += "Max TileY:\t";
		OutString += to_string(maxTileY);
		OutString += "\n";

		OutString += "Tile Line Thickness>:\t";
		OutString += to_string(tileLineThickness);
		OutString += "\n";

		OutString += "Line Half Length:\t";
		OutString += to_string(lineHalfLength);
		OutString += "\n";

		OutString += "Angle Step:\t";
		OutString += to_string(angleStep);
		OutString += "\n";

		OutString += "Interpolation:\t";
		OutString += to_string(interpolation);
		OutString += "\n";

		float imMean, imStd;
		int* Hist;
		int imMin, imMax;
		Hist = MatFMeanAndStd(ImInF, &imMean, &imStd, &imMin, &imMax);

		OutString += "image min:\t";
		OutString += to_string(imMin);
		OutString += "\n";

		OutString += "image ax:\t";
		OutString += to_string(imMax);
		OutString += "\n";

		OutString += "image mean:\t";
		OutString += to_string(imMean);
		OutString += "\n";

		OutString += "image std:\t";
		OutString += to_string(imStd);
		OutString += "\n";

		int* HistBined;
		
		int perc1, perc99, perm1, perm999;
		HistBined = FistogramFeatures(Hist, 1, &perc1, &perc99, &perm1, &perm999);
		delete[] Hist;
		delete[] HistBined;

		OutString += "Percenty l:\t";
		OutString += to_string(perc1);
		OutString += "\n";

		OutString += "Perc 99:\t";
		OutString += to_string(perc99);
		OutString += "\n";

		OutString += "Percentyl 0.1:\t";
		OutString += to_string(perm1);
		OutString += "\n";

		OutString += "Percentyl 99.9:\t";
		OutString += to_string(perm999);
		OutString += "\n";

		OutString += "\n";

		OutString += "yRoiNr\txRoiNr\t\min\tmax\tmean\tstd\tperc 1\tperc99\tperc 0.1\tperc99.9\n";
		int xTileNr = 0;
		int yTileNr = 0;
		for (int y = 0; (y + maxTileY) <= maxY; y += maxTileY)
		{
			for (int x = 0; (x + maxTileX) <= maxX; x += maxTileX)
			{
				ImInF(Rect(x, y, maxTileX, maxTileY)).copyTo(SmallIm);

				OutString += to_string(yTileNr) + "\t";
				OutString += to_string(xTileNr) + "\t";

				Hist = MatFMeanAndStd(SmallIm, &imMean, &imStd, &imMin, &imMax);

				OutString += to_string(imMin);
				OutString += "\t";

				OutString += to_string(imMax);
				OutString += "\t";

				OutString += to_string(imMean);
				OutString += "\t";

				OutString += to_string(imStd);
				OutString += "\t";

				HistBined = FistogramFeatures(Hist, 1, &perc1, &perc99, &perm1, &perm999);
				delete[] Hist;
				delete[] HistBined;

				OutString += to_string(perc1);
				OutString += "\t";

				OutString += to_string(perc99);
				OutString += "\t";

				OutString += to_string(perm1);
				OutString += "\t";

				OutString += to_string(perm999);
				OutString += "\n";

//				cout << "y = " << ItoStrLS(yTileNr, 2) << " x = " << ItoStrLS(xTileNr, 2) << "  ";
//				cout << "\n";
				
				xTileNr++;
			}
			yTileNr++;
			xTileNr = 0;
		}

		string CommTextFileName = OutFolderName + FileNameBase + ".xls";
		ofstream out(CommTextFileName);
		out << OutString;
		out.close();

		if (!FindNextFileA(hFind, &FindFileData))
			break;
	}
	return 0;
}

