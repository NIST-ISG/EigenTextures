#ifndef Functions
#define Functions
//--------------------------------------------------------------------------------------------
#include <stdio.h>
using namespace cv;
using namespace std;

int PrintMatFeatures(Mat Im);

int ShowImageInColorMap(Mat Im, int style = 0, double minIm = 0, double maxIm = 255, const string &WinName =  "OpenCV");

int ShowColorScale(double min, double max, int maxX);
//int SaveImageInColorMap(Mat Im, const std::string &FileName);
Mat ImageInPseudocolors(Mat Im, int style, double par1 = 0, double par2 = 255, double *minOut = 0, double *maxOut = 0);
Mat PrepareColorScale(double min, double max, int maxX);

Mat KeepSize(Mat ImIn, int maxX, int maxY);

Mat MultilevelContours(Mat RegIn);
int MarkContourOnPc(Mat ImIn, Mat RegIn);

int MarkRegionOnPc(Mat ImIn, Mat RegIn, unsigned char B, unsigned char G, unsigned char R);

int MarkCircleleRegionOnPc(Mat ImIn, Mat RegIn);

Mat ColorRegionOnPc(Mat RegIn);

Mat RegionFromColor(Mat ColIm);

float MeanEnergyCalculaton(Mat ImIn);
float MeanEnergyCalculaton(Mat ImIn, Mat Roi);

Mat MeanEnergyImage(Mat ImIn, Mat Roi);

int MaskImageInPseudocolors(Mat ImIn, Mat Roi, unsigned char grayLevel);
#endif