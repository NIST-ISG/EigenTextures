#include "StringFcLib.h"
#include <iostream>
//----------------------------------------------------------------------------------------------------------------------------------
string ItoStrLZ(int nr, int leadZerosCount)
{
	if (leadZerosCount > 10)
		return "to much zeros";
	
	int divisor = 1;
	for (int i = 2; i <= leadZerosCount; i++)
		divisor *= 10;

	if (nr > divisor * 10)
		return "to big nr";

	char Buf[12];
	int iBuf = 0;
	if (nr < 0)
	{
		Buf[iBuf] = '-';
		iBuf++;
	}


	for (int i = 0; i < leadZerosCount; i++)
	{
		int leter = nr / divisor;
		int rest = nr % divisor;
		if ((leter <= 9) | (leter >= 0))
			Buf[iBuf] = '0' + (char)(leter);
		else
			Buf[iBuf] = '^';
		iBuf++;
		nr = rest;
		divisor /= 10;
	}
	Buf[iBuf] = 0;
	return Buf;
}