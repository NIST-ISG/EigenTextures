#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\contrib\contrib.hpp"
#include "math.h"
#include <iostream>
#include "RedundantWaveletLib.h"

//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
Mat RedundantWaveletLLFilter(int scale)
{
	int filterSize = (int)pow(2, scale);
	int halfFilterSize = filterSize / 2;
	float filterMaxXY = (float)(filterSize*filterSize);
	float filterCoef = (float)(1.0) / filterMaxXY;

	Mat FilterLL = Mat::zeros(filterSize, filterSize, CV_32F);
	float *wFilterLL = (float*)FilterLL.data;

	for (int y = 0; y <filterSize; y++)
	{
		for (int x = 0; x <filterSize; x++)
		{
			*wFilterLL = filterCoef;
			wFilterLL++;
		}
	}
	return FilterLL;
}
//-----------------------------------------------------------------------------------------------------------------
Mat RedundantWaveletHLFilter(int scale)
{
	int filterSize = (int)pow(2, scale);
	int halfFilterSize = filterSize / 2;
	float filterMaxXY = (float)(filterSize*filterSize);
	float filterCoef = (float)(1.0) / filterMaxXY;
	float filterCoefN = filterCoef * (float)(-1.0);
	Mat FilterHL = Mat::zeros(filterSize, filterSize, CV_32F);
	float *wFilterHL = (float*)FilterHL.data;

	for (int y = 0; y <filterSize; y++)
	{
		for (int x = 0; x <filterSize; x++)
		{
			if (x < halfFilterSize)
				*wFilterHL = filterCoef;
			else
				*wFilterHL = filterCoefN;
			wFilterHL++;
		}
	}
	return FilterHL;
}
//-----------------------------------------------------------------------------------------------------------------
Mat RedundantWaveletLHFilter(int scale)
{
	int filterSize = (int)pow(2, scale);
	int halfFilterSize = filterSize / 2;
	float filterMaxXY = (float)(filterSize*filterSize);
	float filterCoef = (float)(1.0) / filterMaxXY;
	float filterCoefN = filterCoef * (float)(-1.0);

	Mat FilterLH = Mat::zeros(filterSize, filterSize, CV_32F);
	float *wFilterLH = (float*)FilterLH.data;

	for (int y = 0; y <filterSize; y++)
	{
		for (int x = 0; x <filterSize; x++)
		{
			if (y < halfFilterSize)
				*wFilterLH = filterCoef;
			else
				*wFilterLH = filterCoefN;
			wFilterLH++;
		}
	}
	return FilterLH;
}
//-----------------------------------------------------------------------------------------------------------------
Mat RedundantWaveletHHFilter(int scale)
{
	int filterSize = (int)pow(2, scale);
	int halfFilterSize = filterSize / 2;
	float filterMaxXY = (float)(filterSize*filterSize);
	float filterCoef = (float)(1.0)/ filterMaxXY;
	float filterCoefN = filterCoef * (float)(-1.0);

	Mat FilterHH = Mat::zeros(filterSize, filterSize, CV_32F);
	float *wFilterHH = (float*)FilterHH.data;

	for (int y = 0; y < filterSize; y++)
	{
		for (int x = 0; x < filterSize; x++)
		{
			if ((y < halfFilterSize && x < halfFilterSize) || (y >= halfFilterSize && x >= halfFilterSize))
				*wFilterHH = filterCoef;
			else
				*wFilterHH = filterCoefN;
			wFilterHH++;
		}
	}
	return FilterHH;
}