#ifndef NOISEREDUCTION_H_
#define NOISEREDUCTION_H_
#include <QDebug>

void performMovingAverage (uchar input[], int xSize, int ySize, int N);

void calculateGaussKernel(double kernel[], int N, double sigma);

void performGaussFilter (uchar input[], int xSize, int ySize, int N, double sigma);
	
void performMedianFilter (uchar input[], int xSize, int ySize, int N);

#endif //NOISEREDUCTION_H_