#ifndef _FEATURE_DRAW_H_
#define _FEATURE_DRAW_H_

#include <qglobal.h>

void setPixelValue(uchar img[], int r, int c, int w, int h, uchar value);

void drawCircle(uchar img[], int r, int c, int cR, int w, int h, uchar value);

void drawCircle(uchar img[], int r, int c, int cR, int w, int h, uchar value, float thickness);

void rasterCircle(uchar img[], int r, int c, int radius, int w, int h, uchar value);

void drawOrientation(uchar img[], int x, int y, float ori, int cR, int w, int h, uchar value);

void drawKeypoints(uchar img[], int w, int h, ezsift::SiftKeypointList kpt_list, uchar value);

void drawLine(uchar img[], int w, int h, int r1, int c1, int r2, int c2, uchar value);

void setPixelValueRGB(uchar img[], int r, int c, int w, int h, uchar R, uchar G, uchar B);

void drawCircleRGB(uchar img[], int r, int c, int cR, int w, int h, uchar R, uchar G, uchar B);

void rasterCircleRGB(uchar img[], int r, int c, int radius, int w, int h, uchar R, uchar G, uchar B);

void drawOrientationRGB(uchar img[], int x, int y, float ori, int cR, int w, int h, uchar R, uchar G, uchar B);

void drawKeypointsRGB(uchar img[], int w, int h, ezsift::SiftKeypointList kpt_list, uchar R, uchar G, uchar B);

void drawLineRGB(uchar img[], int w, int h, int r1, int c1, int r2, int c2, uchar R, uchar G, uchar B);

#endif