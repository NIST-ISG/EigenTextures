#ifndef HaralickLib
#define HaralickLib
//--------------------------------------------------------------------------------------------
//#include <stdio.h>
using namespace cv;
using namespace std;
//--------------------------------------------------------------------------------------------
Mat KernelPrepare(int offset, float angle);
Mat COMCardone(Mat ImInFloat, int ofset, float angle, int binCount, float maxNorm, float minNorm);
Mat COMCardone2(Mat ImInFloat, int ofset, float angle, int binCount, float maxNorm, float minNorm);

void COMParams(Mat COM, float *contrastOut, float *energyOut, float *homogenityOut, float *correlationOut);
//--------------------------------------------------------------------------------------------
#endif