#ifndef HaralickLib
#define HaralickLib
//--------------------------------------------------------------------------------------------
//#include <stdio.h>
using namespace cv;
using namespace std;
//--------------------------------------------------------------------------------------------
//Mat KernelPrepare(int offset, float angle);
Mat KernelPrepare(int offset, float angle, int interpolation = INTER_AREA);
Mat COMCardone(Mat ImInFloat, int ofset, float angle, int binCount, float maxNorm, float minNorm);
Mat COMCardone2(Mat ImInFloat, int ofset, float angle, int binCount, float maxNorm, float minNorm);
Mat COMCardone3(Mat ImInFloat, int ofset, float angle, int binCount, float maxNorm, float minNorm, int interpolation = INTER_AREA);
Mat COMCardone4(Mat ImInFloat, int ofset, float angle, int binCount, float maxNorm, float minNorm, int interpolation = INTER_AREA);

Mat COMLatice4(Mat ImInFloat, int ofset, float angle, int binCount, float maxNorm, float minNorm, int interpolation);

void COMParams(Mat COM, float *contrastOut, float *energyOut, float *homogenityOut, float *correlationOut);

//--------------------------------------------------------------------------------------------

Mat COMCardoneLatice(Mat ImInFloat, int ofset, float angle, int binCount, float maxNorm, float minNorm);
//--------------------------------------------------------------------------------------------
int FindBestAngleMin(float *Feature, int size);
int FindBestAngleMax(float *Feature, int size);

float BestAngleQualityScoreMin(float *Feature, int size, int bestAngleIndex);
float BestAngleQualityScoreMax(float *Feature, int size, int bestAngleIndex);
float BestAngleQualityScore(float *Feature, int size, int bestAngleIndex);
#endif