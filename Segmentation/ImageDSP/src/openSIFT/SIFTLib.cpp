#include "ezsift.h"
#include <qglobal.h>
#include <QPoint>
#include "FeatureDraw.h"


using namespace ezsift;
using namespace std;
/*******************************************************************************************************************************/
/* Find SIFT keypoints */
/*******************************************************************************************************************************/
SiftKeypointList calculateSIFT(const uchar Y_buff[], int width, int height)
{
	ezsift::Image<unsigned char>* image = new Image<unsigned char>(width, height);
	SiftKeypointList kpList;

	for (int i = 0; i < width*height; i++)
	{
		image->data[i] = Y_buff[i];
	}

	bool bExtractDescriptor = true;
	

	// Double the original image as the first octive.
	ezsift::double_original_image(true);

	// Perform SIFT computation on CPU.
	ezsift::sift_cpu(*image, kpList, bExtractDescriptor);

    delete image;
	return kpList;
}

/*******************************************************************************************************************************/
/* Mark SIFT keypoints in monochromatic Image */
/*******************************************************************************************************************************/
void markSIFTKeypointsMono(uchar img[], int width, int height, SiftKeypointList kpList, uchar markValue)
{
	drawKeypoints(img, width, height, kpList, markValue);
}

/*******************************************************************************************************************************/
/* Mark SIFT keypoints in RGB image */
/*******************************************************************************************************************************/
void markSIFTKeypointsRGB(uchar imgRGB[], int width, int height, SiftKeypointList kpList, uchar R, uchar G, uchar B)
{
	drawKeypointsRGB(imgRGB, width, height, kpList, R, G, B);
}

/*******************************************************************************************************************************/
/* Mark matched features in RGB image */
/*******************************************************************************************************************************/
void drawMatchedFeaturesLinesMono(uchar input[], int xSize, int ySize, list<pair<QPoint, QPoint>> matchPairs, uchar value)
{
	for (pair<QPoint, QPoint> matchPair : matchPairs)
	{
		drawLine(input, xSize, ySize, matchPair.first.x(), matchPair.first.y(), matchPair.second.x(), matchPair.second.y(), value);
	}
}

/*******************************************************************************************************************************/
/* Mark matched features in RGB image */
/*******************************************************************************************************************************/
void drawMatchedFeaturesLinesRGB(uchar input[], int xSize, int ySize, list<pair<QPoint, QPoint>> matchPairs, uchar R, uchar G, uchar B)
{
	for (pair<QPoint, QPoint> matchPair : matchPairs)
	{
		drawLineRGB(input, xSize, ySize, matchPair.first.x(), matchPair.first.y(), matchPair.second.x(), matchPair.second.y(), R, G, B);
	}
}
