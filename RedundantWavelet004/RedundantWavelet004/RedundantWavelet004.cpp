// RedundantWavelet004.cpp : Defines the entry point for the console application.
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

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	// global var declarations
	string InFolderName, OutFolderName;
	string StrTemp;
	string OutFileNameExtension = ".bmp";
	string FileNameExtension = ".tif";

	bool displayResult = 0;
	bool saveResult = 0;
	bool commonResult = 0;

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
	CommonOutTxt += "RWLLS1\tRWLLS2\tRWLLS3\tRWLLS4\tRWLLS5\tRWLLS6\tRWLLS7\tRWLLS8\tRWLLS9\t\t";
	CommonOutTxt += "RWHLS1\tRWHLS2\tRWHLS3\tRWHLS4\tRWHLS5\tRWHLS6\tRWHLS7\tRWHLS8\tRWHLS9\t\t";
	CommonOutTxt += "RWLHS1\tRWLHS2\tRWLHS3\tRWLHS4\tRWLHS5\tRWLHS6\tRWLHS7\tRWLHS8\tRWLHS9\t\t";
	CommonOutTxt += "RWHHS1\tRWHHS2\tRWHHS3\tRWHHS4\tRWHHS5\tRWHHS6\tRWHHS7\tRWHHS8\tRWHHS9\t";
	CommonOutTxt += "\n";

	// read first file
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFileA((InFolderName + "*" + FileNameExtension).c_str(), &FindFileData);
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
		string WaveletEnergies = "File Name: " + InFileName + "\n";
		WaveletEnergies += "Scale\tLL\tHL\tLH\tHH\n";
		cout << WaveletEnergies;

		// Strings for common outputs
		string WavEnLL, WavEnHL, WavEnLH, WavEnHH;
		WavEnLL = "";
		WavEnHL = "";
		WavEnLH = "";
		WavEnHH = "";

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
			WaveletEnergies += to_string(scale) + "\t";
			float energy;

			energy = MeanEnergyCalculaton(ImLL, Roi);
			WaveletEnergies += std::to_string(energy) + "\t";
			WavEnLL += std::to_string(energy) + "\t";
			cout << std::to_string(energy) + "\t";

			energy = MeanEnergyCalculaton(ImHL, Roi);
			WaveletEnergies += std::to_string(energy) + "\t";
			WavEnHL += std::to_string(energy) + "\t";
			cout << std::to_string(energy) + "\t";

			energy = MeanEnergyCalculaton(ImLH, Roi);
			WaveletEnergies += std::to_string(energy) + "\t";
			WavEnLH += std::to_string(energy) + "\t";
			cout << std::to_string(energy) + "\t";

			energy = MeanEnergyCalculaton(ImHH, Roi);
			WaveletEnergies += std::to_string(energy) + "\t";
			WavEnHH += std::to_string(energy) + "\t";
			cout << std::to_string(energy) + "\t";

			WaveletEnergies += "\n";
			cout << "\n";

			// display and save images
			if (displayResult || saveResult)
			{
				int maxIntLL = 60000;
				int maxIntHL = 2000;
				int maxIntLH = 2000;
				int maxIntHH = 2000;


				Mat EnImLL, EnImLH, EnImHL, EnImHH;
				EnImLL = MeanEnergyImage(ImLL, Roi);
				EnImHL = MeanEnergyImage(ImHL, Roi);
				EnImLH = MeanEnergyImage(ImLH, Roi);
				EnImHH = MeanEnergyImage(ImHH, Roi);


				double minSubband, maxSubband;

				//LL band
				ImShow = ImageInPseudocolors(EnImLL, 2, 0, maxIntLL, &minSubband, &maxSubband);
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
				ImShow = ImageInPseudocolors(EnImHL, 2, 0, maxIntHL, &minSubband, &maxSubband);
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
				ImShow = ImageInPseudocolors(EnImLH, 2, 0, maxIntLH, &minSubband, &maxSubband);
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
				ImShow = ImageInPseudocolors(EnImHH, 2, 0, maxIntHH, &minSubband, &maxSubband);
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
					waitKey(100);
			}
		}
		// save energies file
		string TextFileName = OutFolderName + FileNameBase + ".txt";
		ofstream out(TextFileName);
		out << WaveletEnergies;
		out.close();

		CommonOutTxt += FileNameBase + "\t\t" + WavEnLL + "\t\t" + WavEnHL + "\t\t" + WavEnLH + "\t\t" + WavEnHH + "\n";

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
