#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <math.h>
#include "HaralickLib.h"

#define PI 3.14159265
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
/*
Mat KernelPrepare(int offset, float angle)
{
	int center = offset + 2;
	int size = (offset + 2) * 2 + 1;
	Mat Kernel = Mat::zeros(size, size,CV_32F);
	Kernel.at<float>(center + offset, center) = 1;
	Mat rot_mat = getRotationMatrix2D(Point(center,center), (double)angle, 1);
	warpAffine(Kernel, Kernel, rot_mat, Kernel.size(),INTER_AREA );
	return Kernel;

}
*/
//-------------------------------------------------------------------------------------------------------------
Mat KernelPrepare(int offset, float angle, int interpolation)
{
	int center = offset + 2;
	int size = (offset + 2) * 2 + 1;
	Mat Kernel = Mat::zeros(size, size, CV_32F);
	Kernel.at<float>(center + offset, center) = 1;
	Mat rot_mat = getRotationMatrix2D(Point(center, center), (double)angle, 1);
	warpAffine(Kernel, Kernel, rot_mat, Kernel.size(), interpolation);
	return Kernel;

}
//-------------------------------------------------------------------------------------------------------------
Mat COMCardone(Mat ImInFloat, int ofset, float angle, int binCount, float maxNorm, float minNorm)
{
	int maxX = ImInFloat.cols;
	int maxY = ImInFloat.rows;
	int maxXY = maxX*maxY;
	
	float *wImInF = (float*)ImInFloat.data;
	float *wOriginImInF = (float*)ImInFloat.data;
	float *wEndImInF = (float*)ImInFloat.data + maxXY;
	// prepare pointers and waights for CO Matrix
	Mat Kernel = KernelPrepare(ofset, angle);
	float *wKernel = (float*)Kernel.data;
	int maxKernelX = Kernel.cols;
	int maxKernelY = Kernel.rows;
	int maxKernelXY = maxKernelX * maxKernelY;
	int centerKernelX = maxKernelX / 2;
	int centerKernelY = maxKernelY / 2;

	float wtSP[16];
	float *wSP[16];
	int countSP = 0;
	for (int y = 0; y < maxKernelY; y++)
	{
		for (int x = 0; x < maxKernelX; x++)
		{
			if (*wKernel)
			{
				wtSP[countSP] = *wKernel;
				wSP[countSP] = wImInF + (y - centerKernelY) * maxX + (x - centerKernelX);
				countSP++;
			}
			wKernel++;
		}
		
	}
	float gainCoef = (binCount - 1) / (maxNorm - minNorm);
	float offsetCoef = gainCoef * minNorm;

	Mat COM = Mat::zeros(binCount, binCount, CV_32S);
	for (int i = 0; i < maxXY; i++)
	{
		bool pointersInRange = 1;
		for (int k = 0; k < countSP; k++)
		{
			if (wSP[k] < wOriginImInF)
				pointersInRange = 0;
			if (wSP[k] > wEndImInF)
				pointersInRange = 0;
		}
		if (pointersInRange)
		{

			float valSP = 0;
			for (int k = 0; k < countSP; k++)
			{
				valSP += *wSP[k] * wtSP[k];
			}

			float valFP = *wImInF;
			int comX = (int)(round(valFP * gainCoef - offsetCoef));
			if (comX >= binCount)
				comX = binCount - 1;
			if (comX < 0)
				comX = 0;
			int comY = (int)(round(valSP * gainCoef - offsetCoef));
			if (comY >= binCount)
				comY = binCount - 1;
			if (comY < 0)
				comY = 0;

			COM.at<int>(comY, comX)++;
			COM.at<int>(comX, comY)++;
		}
		for (int k = 0; k < countSP; k++)
			wSP[k]++;
		wImInF++;
	}
	return COM;
}
//-------------------------------------------------------------------------------------------------------------
Mat COMCardone2(Mat ImInFloat, int ofset, float angle, int binCount, float maxNorm, float minNorm)
{
	int maxX = ImInFloat.cols;
	int maxY = ImInFloat.rows;
	int maxXY = maxX*maxY;

	float *wImInF;
	float *wOriginImInF;
	float *wEndImInF;
	int maxKernelX, maxKernelY, maxKernelXY, centerKernelX, centerKernelY;
	
	float wtSP[16];
	float *wSP[16];
	int countSP;

	Mat Kernel;
	float *wKernel;
	float gainCoef = (binCount - 1) / (maxNorm - minNorm);
	float offsetCoef = gainCoef * minNorm;

	Mat COM = Mat::zeros(binCount, binCount, CV_32S);

	
	// prepare pointers and waights for CO Matrix
	
	Kernel = KernelPrepare(ofset, angle);
	wKernel = (float*)Kernel.data;
	
	maxKernelX = Kernel.cols;
	maxKernelY = Kernel.rows;
	maxKernelXY = maxKernelX * maxKernelY;
	centerKernelX = maxKernelX / 2;
	centerKernelY = maxKernelY / 2;
	countSP = 0;

	wImInF = (float*)ImInFloat.data;
	wOriginImInF = (float*)ImInFloat.data;
	wEndImInF = (float*)ImInFloat.data + maxXY;

	for (int y = 0; y < maxKernelY; y++)
		for (int x = 0; x < maxKernelX; x++)
		{
			if (*wKernel)
			{
				wtSP[countSP] = *wKernel;
				wSP[countSP] = wImInF + (y-centerKernelY) * maxX + (x-centerKernelX);
				countSP++;
			}

			wKernel++;
		}




	for (int i = 0; i < maxXY; i++)
	{
		bool pointersInRange = 1;
		for (int k = 0; k < countSP; k++)
		{
			if (wSP[k] < wOriginImInF)
				pointersInRange = 0;
			if (wSP[k] > wEndImInF)
				pointersInRange = 0;
		}
		if (pointersInRange)
		{

			float valSP = 0;
			for (int k = 0; k < countSP; k++)
			{
				valSP += *wSP[k] * wtSP[k];
			}

			float valFP = *wImInF;
			int comX = (int)(round(valFP * gainCoef - offsetCoef));
			if (comX >= binCount)
				comX = binCount - 1;
			if (comX < 0)
				comX = 0;
			int comY = (int)(round(valSP * gainCoef - offsetCoef));
			if (comY >= binCount)
				comY = binCount - 1;
			if (comY < 0)
				comY = 0;

			COM.at<int>(comY, comX)++;
			COM.at<int>(comX, comY)++;
		}
		for (int k = 0; k < countSP; k++)
			wSP[k]++;
		wImInF++;
	}

	// oposit direction

	Kernel = KernelPrepare(ofset, angle + 180);
	wKernel = (float*)Kernel.data;

	maxKernelX = Kernel.cols;
	maxKernelY = Kernel.rows;
	maxKernelXY = maxKernelX * maxKernelY;
	countSP = 0;

	wImInF = (float*)ImInFloat.data;
	wOriginImInF = (float*)ImInFloat.data;
	wEndImInF = (float*)ImInFloat.data + maxXY;

	for (int y = 0; y < maxKernelY; y++)
		for (int x = 0; x < maxKernelX; x++)
		{
		if (*wKernel)
		{
			wtSP[countSP] = *wKernel;
			wSP[countSP] = wImInF + (y - centerKernelY) * maxX + (x - centerKernelX);
			countSP++;
		}

		wKernel++;
		}


	for (int i = 0; i < maxXY; i++)
	{
		bool pointersInRange = 1;
		for (int k = 0; k < countSP; k++)
		{
			if (wSP[k] < wOriginImInF)
				pointersInRange = 0;
			if (wSP[k] > wEndImInF)
				pointersInRange = 0;
		}
		if (pointersInRange)
		{

			float valSP = 0;
			for (int k = 0; k < countSP; k++)
			{
				valSP += *wSP[k] * wtSP[k];
			}

			float valFP = *wImInF;
			int comX = (int)(round(valFP * gainCoef - offsetCoef));
			if (comX >= binCount)
				comX = binCount - 1;
			if (comX < 0)
				comX = 0;
			int comY = (int)(round(valSP * gainCoef - offsetCoef));
			if (comY >= binCount)
				comY = binCount - 1;
			if (comY < 0)
				comY = 0;

			COM.at<int>(comY, comX)++;
			COM.at<int>(comX, comY)++;
		}
		for (int k = 0; k < countSP; k++)
			wSP[k]++;
		wImInF++;
	}




	return COM;
}
//-------------------------------------------------------------------------------------------------------------
Mat COMCardone3(Mat ImInFloat, int ofset, float angle, int binCount, float maxNorm, float minNorm, int interpolation)
{
	int maxX = ImInFloat.cols;
	int maxY = ImInFloat.rows;
	int maxXY = maxX*maxY;

	float *wImInF;
	float *wOriginImInF;
	float *wEndImInF;
	int maxKernelX, maxKernelY, maxKernelXY, centerKernelX, centerKernelY;

	//int laticeX = (int)round((double)(ofset)*sin((double)(angle)* PI / 180.0)) + 1;
	//int laticeY = (int)round((double)(ofset)*cos((double)(angle)* PI / 180.0)) + 1;
	int maxOffsetX, minOffsetX, maxOffsetY, minOffsetY;
	float wtSP[20];
	float *wSP[20];
	int countSP;

	Mat Kernel;
	float *wKernel;
	float gainCoef = (binCount - 1) / (maxNorm - minNorm);
	float offsetCoef = gainCoef * minNorm;

	Mat COM = Mat::zeros(binCount, binCount, CV_32S);


	// prepare pointers and waights for CO Matrix

	Kernel = KernelPrepare(ofset, angle, interpolation);
	wKernel = (float*)Kernel.data;

	maxKernelX = Kernel.cols;
	maxKernelY = Kernel.rows;
	maxKernelXY = maxKernelX * maxKernelY;
	centerKernelX = maxKernelX / 2;
	centerKernelY = maxKernelY / 2;
	countSP = 0;

	wImInF = (float*)ImInFloat.data;
	wOriginImInF = (float*)ImInFloat.data;
	wEndImInF = (float*)ImInFloat.data + maxXY;
	maxOffsetX = maxKernelX * -1;
	minOffsetX = maxKernelX;
	maxOffsetY = maxKernelY * -1;
	minOffsetY = maxKernelY;
	for (int y = 0; y < maxKernelY; y++)
		for (int x = 0; x < maxKernelX; x++)
		{
			if (*wKernel)
			{
				wtSP[countSP] = *wKernel;
				wSP[countSP] = wImInF + (y - centerKernelY) * maxX + (x - centerKernelX);
				countSP++;
				if (maxOffsetX < (x - centerKernelX))
					maxOffsetX = x - centerKernelX;
				if (minOffsetX > (x - centerKernelX))
					minOffsetX = x - centerKernelX;
				if (maxOffsetY < (y - centerKernelY))
					maxOffsetY = y - centerKernelX;
				if (minOffsetY > (y - centerKernelX))
					minOffsetY = y - centerKernelX;
			}

			wKernel++;
		}




	for (int i = 0; i < maxXY; i++)
	{
		int x = i % maxX;
		int y = i / maxX;

		bool pointersInRange = 1;
		for (int k = 0; k < countSP; k++)
		{
			if (wSP[k] < wOriginImInF)
				pointersInRange = 0;
			if (wSP[k] > wEndImInF)
				pointersInRange = 0;
		}
		if ((x + minOffsetX) < 0)
			pointersInRange = 0;
		if ((x + maxOffsetX) >= maxX)
			pointersInRange = 0;
		if ((y + minOffsetY) < 0)
			pointersInRange = 0;
		if ((y + maxOffsetY) >= maxY)
			pointersInRange = 0;

		if (pointersInRange)
		{

			float valSP = 0;
			for (int k = 0; k < countSP; k++)
			{
				valSP += *wSP[k] * wtSP[k];
			}

			float valFP = *wImInF;
			int comX = (int)(round(valFP * gainCoef - offsetCoef));
			if (comX >= binCount)
				comX = binCount - 1;
			if (comX < 0)
				comX = 0;
			int comY = (int)(round(valSP * gainCoef - offsetCoef));
			if (comY >= binCount)
				comY = binCount - 1;
			if (comY < 0)
				comY = 0;

			COM.at<int>(comY, comX)++;
			COM.at<int>(comX, comY)++;
		}
		for (int k = 0; k < countSP; k++)
			wSP[k]++;
		wImInF++;
	}

	// oposit direction

	Kernel = KernelPrepare(ofset, angle + 180, interpolation);
	wKernel = (float*)Kernel.data;

	maxKernelX = Kernel.cols;
	maxKernelY = Kernel.rows;
	maxKernelXY = maxKernelX * maxKernelY;
	countSP = 0;

	wImInF = (float*)ImInFloat.data;
	wOriginImInF = (float*)ImInFloat.data;
	wEndImInF = (float*)ImInFloat.data + maxXY;

	maxOffsetX = maxKernelX * -1;
	minOffsetX = maxKernelX;
	maxOffsetY = maxKernelY * -1;
	minOffsetY = maxKernelY;
	for (int y = 0; y < maxKernelY; y++)
		for (int x = 0; x < maxKernelX; x++)
		{
			if (*wKernel)
			{
				wtSP[countSP] = *wKernel;
				wSP[countSP] = wImInF + (y - centerKernelY) * maxX + (x - centerKernelX);
				countSP++;
				if (maxOffsetX < (x - centerKernelX))
					maxOffsetX = x - centerKernelX;
				if (minOffsetX >(x - centerKernelX))
					minOffsetX = x - centerKernelX;
				if (maxOffsetY < (y - centerKernelY))
					maxOffsetY = y - centerKernelX;
				if (minOffsetY >(y - centerKernelX))
					minOffsetY = y - centerKernelX;
			}

		wKernel++;
	}


	for (int i = 0; i < maxXY; i++)
	{
		int x = i % maxX;
		int y = i / maxX;
		bool pointersInRange = 1;
		for (int k = 0; k < countSP; k++)
		{
			if (wSP[k] < wOriginImInF)
				pointersInRange = 0;
			if (wSP[k] > wEndImInF)
				pointersInRange = 0;
		}
		if ((x + minOffsetX) < 0)
			pointersInRange = 0;
		if ((x + maxOffsetX) >= maxX)
			pointersInRange = 0;
		if ((y + minOffsetY) < 0)
			pointersInRange = 0;
		if ((y + maxOffsetY) >= maxY)
			pointersInRange = 0;

		if (pointersInRange)
		{

			float valSP = 0;
			for (int k = 0; k < countSP; k++)
			{
				valSP += *wSP[k] * wtSP[k];
			}

			float valFP = *wImInF;
			int comX = (int)(round(valFP * gainCoef - offsetCoef));
			if (comX >= binCount)
				comX = binCount - 1;
			if (comX < 0)
				comX = 0;
			int comY = (int)(round(valSP * gainCoef - offsetCoef));
			if (comY >= binCount)
				comY = binCount - 1;
			if (comY < 0)
				comY = 0;

			COM.at<int>(comY, comX)++;
			COM.at<int>(comX, comY)++;
		}
		for (int k = 0; k < countSP; k++)
			wSP[k]++;
		wImInF++;
	}




	return COM;
}
//-------------------------------------------------------------------------------------------------------------
Mat COMCardone4(Mat ImInFloat, int ofset, float angle, int binCount, float maxNorm, float minNorm, int interpolation)
{
	int maxX = ImInFloat.cols;
	int maxY = ImInFloat.rows;
	int maxXY = maxX*maxY;

	float *wImInF;
	float *wOriginImInF;
	float *wEndImInF;
	//int maxKernelX, maxKernelY, maxKernelXY, centerKernelX, centerKernelY;

	float gainCoef = (binCount - 1) / (maxNorm - minNorm);
	float offsetCoef = gainCoef * minNorm;

	
	float spX, spY;
	float spX0, spX1, spY0, spY1;
	float coef00, coef01, coef10, coef11;

	int maxOffsetX, minOffsetX, maxOffsetY, minOffsetY;
	float *wSP00;
	float *wSP01;
	float *wSP10;
	float *wSP11;

	Mat COM = Mat::zeros(binCount, binCount, CV_32S);
	
	
	
	wImInF = (float*)ImInFloat.data;
	wOriginImInF = (float*)ImInFloat.data;
	wEndImInF = (float*)ImInFloat.data + maxXY;
	//Second point coordinates
	spX = (float)(ofset)*sin((angle)* PI / 180.0);
	spY = (float)(ofset)*cos((angle)* PI / 180.0);
	
	spX0 = floor(spX);
	spX1 = ceil(spX);
	if (spX1 == spX0)
		spX1 = spX0 + 1;

	spY0 = floor(spY);
	spY1 = ceil(spY);
	if (spY1 == spY0)
		spY1 = spY0 + 1;

	//Second Point coeficients
	coef00 = (spY1 - spY)*(spX1 - spX) / ((spX1 - spX0)*(spY1 - spY0));
	coef01 = (spY1 - spY)*(spX - spX0) / ((spX1 - spX0)*(spY1 - spY0));
	coef10 = (spY - spY0)*(spX1 - spX) / ((spX1 - spX0)*(spY1 - spY0));
	coef11 = (spY - spY0)*(spX - spX0) / ((spX1 - spX0)*(spY1 - spY0));

	

	// second point pointer
	//float wtSP[20];
	//float *wSP[20];
	//int countSP;
	

	// second point pointers
	wSP00 = wImInF + (int)spY0 * maxX + (int)spX0;
	wSP01 = wImInF + (int)spY0 * maxX + (int)spX1;
	wSP10 = wImInF + (int)spY1 * maxX + (int)spX0;
	wSP11 = wImInF + (int)spY1 * maxX + (int)spX1;


	minOffsetX = 0;
	if (minOffsetX > spX0)
		minOffsetX = spX0;
	if (minOffsetX > spX1)
		minOffsetX = spX1;

	minOffsetY = 0;
	if (minOffsetY > spY0)
		minOffsetY = spY0;
	if (minOffsetY > spY1)
		minOffsetY = spY1;


	maxOffsetX = 0;
	if (maxOffsetX < spX0)
		maxOffsetX = spX0;
	if (maxOffsetX < spX1)
		maxOffsetX = spX1;

	maxOffsetY = 0;
	if (maxOffsetY < spY0)
		maxOffsetY = spY0;
	if (maxOffsetY < spY1)
		maxOffsetY = spY1;


	for (int i = 0; i < maxXY; i++)
	{
		int x = i % maxX;
		int y = i / maxX;

		bool pointersInRange = 1;

		if (wSP00 < wOriginImInF)
			pointersInRange = 0;
		if (wSP01 < wOriginImInF)
			pointersInRange = 0;
		if (wSP10 < wOriginImInF)
			pointersInRange = 0;
		if (wSP11 < wOriginImInF)
			pointersInRange = 0;

		if (wSP00 > wEndImInF)
			pointersInRange = 0;
		if (wSP01 > wEndImInF)
			pointersInRange = 0;
		if (wSP10 > wEndImInF)
			pointersInRange = 0;
		if (wSP11 > wEndImInF)
			pointersInRange = 0;

		if ((x + minOffsetX) < 0)
			pointersInRange = 0;
		if ((x + maxOffsetX) >= maxX)
			pointersInRange = 0;
		if ((y + minOffsetY) < 0)
			pointersInRange = 0;
		if ((y + maxOffsetY) >= maxY)
			pointersInRange = 0;

		if (pointersInRange)
		{
			float valSP = *wSP00 * coef00 + *wSP01 * coef01 + *wSP10 * coef10 + *wSP11 * coef11;
			float valFP = *wImInF;

			int comX = (int)(round(valFP * gainCoef - offsetCoef));
			if (comX >= binCount)
				comX = binCount - 1;
			if (comX < 0)
				comX = 0;
			int comY = (int)(round(valSP * gainCoef - offsetCoef));
			if (comY >= binCount)
				comY = binCount - 1;
			if (comY < 0)
				comY = 0;

			COM.at<int>(comY, comX)++;
			COM.at<int>(comX, comY)++;
		}
		wSP00++;
		wSP01++;
		wSP10++;
		wSP11++;
		wImInF++;
	}

	// oposit direction
	wImInF = (float*)ImInFloat.data;

	//Second point coordinates
	spX = (float)(ofset)*sin((angle + 180)* PI / 180.0);
	spY = (float)(ofset)*cos((angle + 180)* PI / 180.0);

	spX0 = floor(spX);
	spX1 = ceil(spX);
	if (spX1 == spX0)
		spX1 = spX0 + 1;

	spY0 = floor(spY);
	spY1 = ceil(spY);
	if (spY1 == spY0)
		spY1 = spY0 + 1;

	//Second Point coeficients
	coef00 = (spY1 - spY)*(spX1 - spX) / ((spX1 - spX0)*(spY1 - spY0));
	coef01 = (spY1 - spY)*(spX - spX0) / ((spX1 - spX0)*(spY1 - spY0));
	coef10 = (spY - spY0)*(spX1 - spX) / ((spX1 - spX0)*(spY1 - spY0));
	coef11 = (spY - spY0)*(spX - spX0) / ((spX1 - spX0)*(spY1 - spY0));



	// second point pointer
	//float wtSP[20];
	//float *wSP[20];
	//int countSP;


	// second point pointers
	wSP00 = wImInF + (int)spY0 * maxX + (int)spX0;
	wSP01 = wImInF + (int)spY0 * maxX + (int)spX1;
	wSP10 = wImInF + (int)spY1 * maxX + (int)spX0;
	wSP11 = wImInF + (int)spY1 * maxX + (int)spX1;


	minOffsetX = 0;
	if (minOffsetX > spX0)
		minOffsetX = spX0;
	if (minOffsetX > spX1)
		minOffsetX = spX1;

	minOffsetY = 0;
	if (minOffsetY > spY0)
		minOffsetY = spY0;
	if (minOffsetY > spY1)
		minOffsetY = spY1;


	maxOffsetX = 0;
	if (maxOffsetX < spX0)
		maxOffsetX = spX0;
	if (maxOffsetX < spX1)
		maxOffsetX = spX1;

	maxOffsetY = 0;
	if (maxOffsetY < spY0)
		maxOffsetY = spY0;
	if (maxOffsetY < spY1)
		maxOffsetY = spY1;


	for (int i = 0; i < maxXY; i++)
	{
		int x = i % maxX;
		int y = i / maxX;

		bool pointersInRange = 1;

		if (wSP00 < wOriginImInF)
			pointersInRange = 0;
		if (wSP01 < wOriginImInF)
			pointersInRange = 0;
		if (wSP10 < wOriginImInF)
			pointersInRange = 0;
		if (wSP11 < wOriginImInF)
			pointersInRange = 0;

		if (wSP00 > wEndImInF)
			pointersInRange = 0;
		if (wSP01 > wEndImInF)
			pointersInRange = 0;
		if (wSP10 > wEndImInF)
			pointersInRange = 0;
		if (wSP11 > wEndImInF)
			pointersInRange = 0;

		if ((x + minOffsetX) < 0)
			pointersInRange = 0;
		if ((x + maxOffsetX) >= maxX)
			pointersInRange = 0;
		if ((y + minOffsetY) < 0)
			pointersInRange = 0;
		if ((y + maxOffsetY) >= maxY)
			pointersInRange = 0;

		if (pointersInRange)
		{
			float valSP = *wSP00 * coef00 + *wSP01 * coef01 + *wSP10 * coef10 + *wSP11 * coef11;
			float valFP = *wImInF;

			int comX = (int)(round(valFP * gainCoef - offsetCoef));
			if (comX >= binCount)
				comX = binCount - 1;
			if (comX < 0)
				comX = 0;
			int comY = (int)(round(valSP * gainCoef - offsetCoef));
			if (comY >= binCount)
				comY = binCount - 1;
			if (comY < 0)
				comY = 0;

			COM.at<int>(comY, comX)++;
			COM.at<int>(comX, comY)++;
		}
		wSP00++;
		wSP01++;
		wSP10++;
		wSP11++;
		wImInF++;
	}





	return COM;
}
//-------------------------------------------------------------------------------------------------------------
Mat COMLatice4(Mat ImInFloat, int ofset, float angle, int binCount, float maxNorm, float minNorm, int interpolation)
{
	int maxX = ImInFloat.cols;
	int maxY = ImInFloat.rows;
	int maxXY = maxX*maxY;

	float *wImInF;
	float *wSPL;
	float *wOriginImInF;
	float *wEndImInF;
	//int maxKernelX, maxKernelY, maxKernelXY, centerKernelX, centerKernelY;

	float gainCoef = (binCount - 1) / (maxNorm - minNorm);
	float offsetCoef = gainCoef * minNorm;


	float spX, spY;
	float spXL, spYL;
	//float coef00, coef01, coef10, coef11;

	int maxOffsetX, minOffsetX, maxOffsetY, minOffsetY;


	Mat COM = Mat::zeros(binCount, binCount, CV_32S);



	wImInF = (float*)ImInFloat.data;
	wOriginImInF = (float*)ImInFloat.data;
	wEndImInF = (float*)ImInFloat.data + maxXY;
	//Second point coordinates
	spX = (float)(ofset)*sin((angle)* PI / 180.0);
	spY = (float)(ofset)*cos((angle)* PI / 180.0);

	spXL = round(spX);
	spYL = round(spY);
	

	// second point pointer
	//float wtSP[20];
	//float *wSP[20];
	//int countSP;


	// second point pointer
	wSPL = wImInF + (int)spYL * maxX + (int)spXL;

	minOffsetX = 0;
	if (minOffsetX > spXL)
		minOffsetX = spXL;

	minOffsetY = 0;
	if (minOffsetY > spYL)
		minOffsetY = spYL;

	maxOffsetX = 0;
	if (maxOffsetX < spXL)
		maxOffsetX = spXL;

	maxOffsetY = 0;
	if (maxOffsetY < spYL)
		maxOffsetY = spYL;

	for (int i = 0; i < maxXY; i++)
	{
		int x = i % maxX;
		int y = i / maxX;

		bool pointersInRange = 1;

		if (wSPL < wOriginImInF)
			pointersInRange = 0;
	
		if (wSPL > wEndImInF)
			pointersInRange = 0;

		if ((x + minOffsetX) < 0)
			pointersInRange = 0;
		if ((x + maxOffsetX) >= maxX)
			pointersInRange = 0;
		if ((y + minOffsetY) < 0)
			pointersInRange = 0;
		if ((y + maxOffsetY) >= maxY)
			pointersInRange = 0;

		if (pointersInRange)
		{
			float valSP = *wSPL;
			float valFP = *wImInF;

			int comX = (int)(round(valFP * gainCoef - offsetCoef));
			if (comX >= binCount)
				comX = binCount - 1;
			if (comX < 0)
				comX = 0;
			int comY = (int)(round(valSP * gainCoef - offsetCoef));
			if (comY >= binCount)
				comY = binCount - 1;
			if (comY < 0)
				comY = 0;

			COM.at<int>(comY, comX)++;
			COM.at<int>(comY, comX)++;
			COM.at<int>(comX, comY)++;
			COM.at<int>(comX, comY)++;
		}
		wSPL++;
		wImInF++;
	}
	//oposit direction




	return COM;
}
//-------------------------------------------------------------------------------------------------------------
void COMParams(Mat COM, float *contrastOut, float *energyOut, float *homogenityOut, float *correlationOut)
{
	int maxX = COM.cols;
	int maxY = COM.rows;
	int maxXY = maxX * maxY;

	int *wCOM;

	int sum = 0;
	wCOM = (int*)COM.data;
	for (int i = 0; i < maxXY; i++)
	{
		sum += *wCOM;
		wCOM++;
	}
	float sumF = (float)sum;
	
	float energy = 0;
	float contrast = 0;
	float homogenity = 0;
	float muX = 0;
	float muY = 0;
	wCOM = (int*)COM.data;
	for (int y = 0; y < maxY; y++)
	{
		for (int x = 0; x < maxX; x++)
		{
			float normValCOM = (float)(*wCOM) / sumF;
			contrast += (float)((x - y)*(x - y)) * normValCOM;
			energy += normValCOM*normValCOM;
			homogenity += normValCOM / ((float)(1 + abs(y - x)));
			muX += (float)(x + 1) * normValCOM;
			muY += (float)(y + 1) * normValCOM;
			wCOM++;
		}
	}

	float sigmaX = 0;
	float sigmaY = 0;
	float corelation = 0;
	wCOM = (int*)COM.data;
	for (int y = 0; y < maxY; y++)
	{
		for (int x = 0; x < maxX; x++)
		{
			float normValCOM = (float)(*wCOM) / sumF;
			sigmaX += ((float)(x + 1) - muX) * ((float)(x + 1) - muX) * normValCOM;
			sigmaY += ((float)(y + 1) - muY) * ((float)(y + 1) - muY) * normValCOM;
	
			corelation += ((float)(x + 1) - muX) * ((float)(y + 1) - muY) * normValCOM;
			wCOM++;
		}
	}
	
	sigmaX = sqrt(sigmaX);
	sigmaY = sqrt(sigmaY);

	corelation = corelation / (sigmaX*sigmaY);

	*contrastOut = contrast;
	*energyOut = energy;
	*homogenityOut = homogenity;
	*correlationOut = corelation;
}

//-------------------------------------------------------------------------------------------------------------
Mat COMStandard(Mat ImInFloat, int ofsetY, int ofsetX, int binCount, float maxNorm, float minNorm)
{

	return Mat::zeros(1, 1, CV_32S);
}
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
Mat COMCardoneLatice(Mat ImInFloat, int ofset, float angle, int binCount, float maxNorm, float minNorm)
{
	int maxX = ImInFloat.cols;
	int maxY = ImInFloat.rows;
	int maxXY = maxX*maxY;

	int laticeX = (int)round((double)(ofset)*sin((double)(angle) * PI / 180.0));
	int laticeY = (int)round((double)(ofset)*cos((double)(angle) * PI / 180.0));

	float *wImInF;
	float *wOriginImInF;
	float *wEndImInF;

	float *wSP;

	float gainCoef = (binCount - 1) / (maxNorm - minNorm);
	float offsetCoef = gainCoef * minNorm;

	Mat COM = Mat::zeros(binCount, binCount, CV_32S);

	wImInF = (float*)ImInFloat.data;
	wOriginImInF = (float*)ImInFloat.data;
	wEndImInF = (float*)ImInFloat.data + maxXY;

	wSP = wImInF + laticeY  * maxX + laticeX;

	for (int i = 0; i < maxXY; i++)
	{
		bool pointersInRange = 1;
		if (wSP < wOriginImInF)
			pointersInRange = 0;
		if (wSP > wEndImInF)
			pointersInRange = 0;

		if (pointersInRange)
		{

			float valFP = *wImInF;
			int comX = (int)(round(valFP * gainCoef - offsetCoef));
			if (comX >= binCount)
				comX = binCount - 1;
			if (comX < 0)
				comX = 0;
			float valSP = *wSP;
			int comY = (int)(round(valSP * gainCoef - offsetCoef));
			if (comY >= binCount)
				comY = binCount - 1;
			if (comY < 0)
				comY = 0;

			COM.at<int>(comY, comX)+= 2;
			COM.at<int>(comX, comY)+= 2;
		}
		wSP++;
		wImInF++;
	}

	return COM;
}
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
int FindBestAngleMin(float *Feature, int size)
{
	// find best angle
	int bestAngleIndex = 0;
	float minContrast = Feature[0];
	for (int angleIndex = 1; angleIndex < size; angleIndex++)
	{
		if (minContrast > Feature[angleIndex])
		{
			minContrast = Feature[angleIndex];
			bestAngleIndex = angleIndex;
		}
	}
	return bestAngleIndex;
}
//-------------------------------------------------------------------------------------------------------------
int FindBestAngleMax(float *Feature, int size)
{
	// find best angle
	int bestAngleIndex = 0;
	float maxContrast = Feature[0];
	for (int angleIndex = 1; angleIndex < size; angleIndex++)
	{
		if (maxContrast < Feature[angleIndex])
		{
			maxContrast = Feature[angleIndex];
			bestAngleIndex = angleIndex;
		}
	}
	return bestAngleIndex;
}
//-------------------------------------------------------------------------------------------------------------
float BestAngleQualityScoreMin(float *Feature, int size, int bestAngleIndex)
{
	float sum = 0;
	for (int angleIndex = 0; angleIndex < size; angleIndex++)
	{
		if (angleIndex != bestAngleIndex)
		{
			sum += Feature[angleIndex];
		}
	}
	float mean = sum / (float)(size - 1);
	float dev = 0;
	for (int angleIndex = 0; angleIndex < size; angleIndex++)
	{
		if (angleIndex != bestAngleIndex)
		{
			dev += (mean - Feature[angleIndex]) * (mean - Feature[angleIndex]);
		}
	}
	float std = sqrt(dev / (float)(size - 1));
	float bestAngleQuality = (mean - std) - Feature[bestAngleIndex];
	return abs(bestAngleQuality/mean);
}
//-------------------------------------------------------------------------------------------------------------
float BestAngleQualityScoreMax(float *Feature, int size, int bestAngleIndex)
{
	float sum = 0;
	for (int angleIndex = 0; angleIndex < size; angleIndex++)
	{
		if (angleIndex != bestAngleIndex)
		{
			sum += Feature[angleIndex];
		}
	}
	float mean = sum / (float)(size - 1);
	float dev = 0;
	for (int angleIndex = 0; angleIndex < size; angleIndex++)
	{
		if (angleIndex != bestAngleIndex)
		{
			dev += (mean - Feature[angleIndex]) * (mean - Feature[angleIndex]);
		}
	}
	float std = sqrt(dev / (float)(size - 1));
	float bestAngleQuality = Feature[bestAngleIndex] - (mean + std);
	return abs(bestAngleQuality / mean);
}
//-------------------------------------------------------------------------------------------------------------
float BestAngleQualityScore(float *Feature, int size, int bestAngleIndex)
{
	float sum = 0;
	for (int angleIndex = 0; angleIndex < size; angleIndex++)
	{
		if (angleIndex != bestAngleIndex)
		{
			sum += Feature[angleIndex];
		}
	}
	float mean = sum / (float)(size - 1);
	float dev = 0;
	for (int angleIndex = 0; angleIndex < size; angleIndex++)
	{
		if (angleIndex != bestAngleIndex)
		{
			dev += (mean - Feature[angleIndex]) * (mean - Feature[angleIndex]);
		}
	}
	float max = -1000000.0;
	for (int angleIndex = 0; angleIndex < size; angleIndex++)
	{
		if (max < Feature[angleIndex])
			max = Feature[angleIndex];
	}
	float min = 1000000.0;
	for (int angleIndex = 0; angleIndex < size; angleIndex++)
	{
		if (min > Feature[angleIndex])
			min = Feature[angleIndex];
	}


	float std = sqrt(dev / (float)(size - 1));
	float bestAngleQuality = 100000;
	if (bestAngleQuality > abs(Feature[bestAngleIndex] - (mean + std)))
		bestAngleQuality = abs(Feature[bestAngleIndex] - (mean + std));
	if (bestAngleQuality > abs(Feature[bestAngleIndex] - (mean - std)))
		bestAngleQuality = abs(Feature[bestAngleIndex] - (mean - std));
	if (bestAngleQuality > abs(Feature[bestAngleIndex] + (mean + std)))
		bestAngleQuality = abs(Feature[bestAngleIndex] + (mean + std));
	if (bestAngleQuality > abs(Feature[bestAngleIndex] + (mean - std)))
		bestAngleQuality = abs(Feature[bestAngleIndex] + (mean - std));

	return abs(bestAngleQuality / abs(max - min));
}
//-------------------------------------------------------------------------------------------------------------