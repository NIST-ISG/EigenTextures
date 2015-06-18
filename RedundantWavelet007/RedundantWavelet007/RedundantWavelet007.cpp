// RedundantWavelet007.cpp : Defines the entry point for the console application.
//

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\contrib\contrib.hpp"
#include "math.h"
#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>

#include <tinyxml.h>
#include <tinystr.h>

#include "Functions.h"
#include "RedundantWaveletLib.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	// global var declarations

	string StrTemp;

	string InFolderName, OutFolderName;
	string OutFileNameExtension = ".bmp";
	string FileNameExtension = ".bmp";
	string InFilePattern;

	bool displayResult = 1;
	bool saveResult = 1;
	bool commonResult = 0;

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
	TiXmlHandle hFeatures(0);
	pElem = hDoc.FirstChildElement().Element();

	cout << pElem->Value();
	cout << "\n";

	hRoot = TiXmlHandle(pElem);
	pElem = hRoot.FirstChild("output").FirstChild("directory").Element();
	OutFolderName = pElem->GetText();

	pElem = hRoot.FirstChild("inputImages").FirstChild("directory").Element();
	InFolderName = pElem->GetText();

	pElem = hRoot.FirstChild("inputImages").FirstChild("pattern").Element();
	InFilePattern = pElem->GetText();

	pElem = hRoot.FirstChild("features").Element();
	hFeatures = TiXmlHandle(pElem);

	string FeatureID, ParameterID, ValStr, FeatureID1;
	int maxScaleLL = 10000;
	int maxScaleLH = 10000;
	int maxScaleHL = 10000;
	int maxScaleHH = 10000;

	bool WavEnLL = 0;
	bool WavEnLH = 0;
	bool WavEnHL = 0;
	bool WavEnHH = 0;
	for (int featureNr = 0; featureNr < 1000; featureNr++)
	{
		pElem = hFeatures.Child("feature", featureNr).FirstChild("id").Element();
		if (!pElem)
			break;
		FeatureID = pElem->GetText();
		if (FeatureID == "RedundantWaveletEnergyLL")
		{
			WavEnLL = 1;
			pElem = hFeatures.Child("feature", featureNr).FirstChild("parameters").FirstChild("parameter").FirstChild("id").Element();
			if (pElem)
			{
				ParameterID = pElem->GetText();
				if (ParameterID == "MaxScale")
				{
					pElem = hFeatures.Child("feature", featureNr).FirstChild("parameters").FirstChild("parameter").FirstChild("value").Element();
					ValStr = pElem->GetText();
					maxScaleLL = stoi(ValStr);
				}
			}
		}

	}
	//	cout << pElem->Value();



	cout << "Output folder name" << "\t" << OutFolderName << "\n";
	cout << "Input folder name" << "\t" << InFolderName << "\n";
	cout << "Input file pattern" << "\t" << InFilePattern << "\n";

	if (WavEnLL)
		cout << "calculate wavelet subband Energies" << "\n";
	if (!WavEnLL)
		return 0;

	cout << "MaxScaleSet" << "\t" << maxScaleLL << "\n";
	cout << "\n";
	//cout << hDoc.FirstChildElement().Element()->Value();
	//cout << "\n";
	//cout << hDoc.FirstChildElement("output").Element()->Value();
	cout << "\n";
	//cin >> StrTemp;
	//return 0;

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


	string CommonOutTxt;
	CommonOutTxt = "FileName\t\t";
	CommonOutTxt += "RWLLS1\tRWLLS2\tRWLLS3\tRWLLS4\tRWLLS5\tRWLLS6\tRWLLS7\tRWLLS8\tRWLLS9\t\t";
	CommonOutTxt += "RWHLS1\tRWHLS2\tRWHLS3\tRWHLS4\tRWHLS5\tRWHLS6\tRWHLS7\tRWHLS8\tRWHLS9\t\t";
	CommonOutTxt += "RWLHS1\tRWLHS2\tRWLHS3\tRWLHS4\tRWLHS5\tRWLHS6\tRWLHS7\tRWLHS8\tRWLHS9\t\t";
	CommonOutTxt += "RWHHS1\tRWHHS2\tRWHHS3\tRWHHS4\tRWHHS5\tRWHHS6\tRWHHS7\tRWHHS8\tRWHHS9\t";
	CommonOutTxt += "\n";

	// read first file
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFileA((InFolderName + InFilePattern).c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		cout << " file not found";
		return 0;
	}
	//handle files
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

		PrintMatFeatures(ImInF);
		//show images
		if (displayResult)
		{
			namedWindow("Image", WINDOW_AUTOSIZE);
			namedWindow("ImageLL", WINDOW_AUTOSIZE);
			namedWindow("ImageLH", WINDOW_AUTOSIZE);
			namedWindow("ImageHL", WINDOW_AUTOSIZE);
			namedWindow("ImageHH", WINDOW_AUTOSIZE);
			namedWindow("ScaleLL", WINDOW_AUTOSIZE);
			namedWindow("ScaleHL", WINDOW_AUTOSIZE);
			namedWindow("ScaleLH", WINDOW_AUTOSIZE);
			namedWindow("ScaleHH", WINDOW_AUTOSIZE);
		}
		Mat ImTemp, ImShow, ImScale;
		if (displayResult)
		{
			ImInF.convertTo(ImTemp, CV_8U);
			applyColorMap(ImTemp, ImShow, COLORMAP_JET);
			namedWindow("Image", WINDOW_AUTOSIZE);
			imshow("Image", ImShow);
			//  waitKey(0);
		}
		// output string 
		string WaveletMeanAbs = "File Name: " + InFileName + "\n";
		WaveletMeanAbs += "Scale\tLL\tHL\tLH\tHH\n";
		cout << WaveletMeanAbs;

		// Strings for common outputs
		string WavMeanAbsLL, WavMeanAbsHL, WavMeanAbsLH, WavMeanAbsHH;
		WavMeanAbsLL = "";
		WavMeanAbsHL = "";
		WavMeanAbsLH = "";
		WavMeanAbsHH = "";

		//calculate filters
		int oldFilterSize = 0;
		for (int scale = 1; scale <= 16; scale++)
		{
			Mat FilterLL, FilterLH, FilterHL, FilterHH;
			int filterSize = (int)pow(2, scale);
			int erodeKernelSize = filterSize - oldFilterSize + 1;
			oldFilterSize = filterSize;
			if ((filterSize > maxX / 2) | (filterSize > maxY / 2))
				break;

			Mat ErodeKernel = Mat::ones(erodeKernelSize, erodeKernelSize, CV_8U);

			erode(Roi, Roi, ErodeKernel, Point(-1, -1), 1, BORDER_CONSTANT, 0);

			FilterLL = RedundantWaveletLLFilter(scale);
			FilterHL = RedundantWaveletHLFilter(scale);
			FilterLH = RedundantWaveletLHFilter(scale);
			FilterHH = RedundantWaveletHHFilter(scale);

			// filtering
			Mat ImLL, ImLH, ImHL, ImHH;
			filter2D(ImIn, ImLL, CV_32F, FilterLL, Point(-1, -1), 0, BORDER_REPLICATE);
			filter2D(ImIn, ImHL, CV_32F, FilterHL, Point(-1, -1), 0, BORDER_REPLICATE);
			filter2D(ImIn, ImLH, CV_32F, FilterLH, Point(-1, -1), 0, BORDER_REPLICATE);
			filter2D(ImIn, ImHH, CV_32F, FilterHH, Point(-1, -1), 0, BORDER_REPLICATE);

			//energy calculations
			WaveletMeanAbs += to_string(scale) + "\t";
			float absVal;

			absVal = MeanAbsCalculaton(ImLL, Roi);
			WaveletMeanAbs += std::to_string(absVal) + "\t";
			WavMeanAbsLL += std::to_string(absVal) + "\t";
			cout << std::to_string(absVal) + "\t";

			absVal = MeanAbsCalculaton(ImHL, Roi);
			WaveletMeanAbs += std::to_string(absVal) + "\t";
			WavMeanAbsHL += std::to_string(absVal) + "\t";
			cout << std::to_string(absVal) + "\t";

			absVal = MeanAbsCalculaton(ImLH, Roi);
			WaveletMeanAbs += std::to_string(absVal) + "\t";
			WavMeanAbsLH += std::to_string(absVal) + "\t";
			cout << std::to_string(absVal) + "\t";

			absVal = MeanAbsCalculaton(ImHH, Roi);
			WaveletMeanAbs += std::to_string(absVal) + "\t";
			WavMeanAbsHH += std::to_string(absVal) + "\t";
			cout << std::to_string(absVal) + "\t";

			WaveletMeanAbs += "\n";
			cout << "\n";

			// display and save images
			if (displayResult || saveResult)
			{
				int maxIntLL = 255;
				int maxIntHL = 255;
				int maxIntLH = 255;
				int maxIntHH = 255;


				Mat AbsImLL, AbsImLH, AbsImHL, AbsImHH;
				AbsImLL = AbsImage(ImLL, Roi);
				AbsImHL = AbsImage(ImHL, Roi);
				AbsImLH = AbsImage(ImLH, Roi);
				AbsImHH = AbsImage(ImHH, Roi);


				double minSubband, maxSubband;

				//LL band
				ImShow = ImageInPseudocolors(AbsImLL, 2, 0, maxIntLL, &minSubband, &maxSubband);
				MaskImageInPseudocolors(ImShow, Roi, 200);
				ImScale = PrepareColorScale(minSubband, maxSubband, 100);
				if (displayResult)
				{
					imshow("ImageLL", ImShow);
					imshow("ScaleLL", ImScale);
				}
				if (saveResult)
				{
					string OutFileName = OutFolderName + FileNameBase + "Scale" + std::to_string(scale) + "Band" + "LL" + OutFileNameExtension;
					imwrite(OutFileName, ImShow);
				}
				//HL band
				ImShow = ImageInPseudocolors(AbsImHL, 2, 0, maxIntHL, &minSubband, &maxSubband);
				MaskImageInPseudocolors(ImShow, Roi, 200);
				ImScale = PrepareColorScale(minSubband, maxSubband, 100);
				if (displayResult)
				{
					imshow("ImageHL", ImShow);
					imshow("ScaleHL", ImScale);
				}
				if (saveResult)
				{
					string OutFileName = OutFolderName + FileNameBase + "Scale" + std::to_string(scale) + "Band" + "HL" + OutFileNameExtension;
					imwrite(OutFileName, ImShow);
				}
				//LH band
				ImShow = ImageInPseudocolors(AbsImLH, 2, 0, maxIntLH, &minSubband, &maxSubband);
				MaskImageInPseudocolors(ImShow, Roi, 200);
				ImScale = PrepareColorScale(minSubband, maxSubband, 100);
				if (displayResult)
				{
					imshow("ImageLH", ImShow);
					imshow("ScaleLH", ImScale);
				}
				if (saveResult)
				{
					string OutFileName = OutFolderName + FileNameBase + "Scale" + std::to_string(scale) + "Band" + "LH" + OutFileNameExtension;
					imwrite(OutFileName, ImShow);
				}
				//HH band
				ImShow = ImageInPseudocolors(AbsImHH, 2, 0, maxIntHH, &minSubband, &maxSubband);
				MaskImageInPseudocolors(ImShow, Roi, 200);
				ImScale = PrepareColorScale(minSubband, maxSubband, 100);
				if (displayResult)
				{
					imshow("ImageHH", ImShow);
					imshow("ScaleHH", ImScale);
				}
				if (saveResult)
				{
					string OutFileName = OutFolderName + FileNameBase + "Scale" + std::to_string(scale) + "Band" + "HH" + OutFileNameExtension;
					imwrite(OutFileName, ImShow);
				}

				if (displayResult)
					waitKey(0);
			}
		}
		// save energies file
		string TextFileName = OutFolderName + FileNameBase + ".txt";
		ofstream out(TextFileName);
		out << WaveletMeanAbs;
		out.close();

		CommonOutTxt += FileNameBase + "\t\t" + WavMeanAbsLL + "\t\t" + WavMeanAbsHL + "\t\t" + WavMeanAbsLH + "\t\t" + WavMeanAbsHH + "\n";

		//find the next file
		if (!FindNextFileA(hFind, &FindFileData))
			break;
	}

	// save common energies file
	std::string TextFileName = OutFolderName + "Common" + ".txt";
	std::ofstream out(TextFileName);
	out << CommonOutTxt;
	out.close();
	//cin >> StrTemp;
	return 0;
}



