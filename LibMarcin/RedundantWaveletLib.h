#ifndef RedundantWaveletLib
#define RedundantWaveletLib
//--------------------------------------------------------------------------------------------
using namespace cv;
using namespace std;

//-----------------------------------------------------------------------------------------------------------------
Mat RedundantWaveletLLFilter(int scale);
Mat RedundantWaveletHLFilter(int scale);
Mat RedundantWaveletLHFilter(int scale);
Mat RedundantWaveletHHFilter(int scale);
#endif
