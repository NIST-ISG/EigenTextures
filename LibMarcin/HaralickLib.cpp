#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <math.h>
#include "HaralickLib.h"
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
Mat KernelPrepare(int offset, float angle)
{
	int center = offset + 2;
	int size = (offset + 2) * 2 + 1;
	Mat Kernel = Mat::zeros(size, size,CV_32F);
	Kernel.at<float>(center + offset, center) = 1;
	Mat rot_mat = getRotationMatrix2D(Point(center,center), (double)angle, 1);
	warpAffine(Kernel, Kernel, rot_mat, Kernel.size(),INTER_LINEAR );
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
