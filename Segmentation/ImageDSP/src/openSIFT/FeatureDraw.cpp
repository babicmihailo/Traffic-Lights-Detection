#include <qglobal.h>
#include "ezsift.h"
#include <cmath>

void setPixelValue(uchar img[], int r, int c, int w, int h, uchar value)
{
	if ((r >= 0) && (r < h) && (c >= 0) && (c < w)) {
        img[r * w + c] = value;
	}
}

void drawCircle(uchar img[], int r, int c, int cR, int w, int h, uchar value)
{
	int cx = -cR, cy = 0, err = 2 - 2 * cR; // II. Quadrant
	do {
		setPixelValue(img, r - cx, c + cy, w, h, value); //   I. Quadrant
		setPixelValue(img, r - cy, c - cx, w, h, value); //  II. Quadrant
		setPixelValue(img, r + cx, c - cy, w, h, value); // III. Quadrant
		setPixelValue(img, r + cy, c + cx, w, h, value); //  IV. Quadrant
		cR = err;
		if (cR > cx)
			err += ++cx * 2 + 1; // e_xy+e_x > 0
		if (cR <= cy)
			err += ++cy * 2 + 1; // e_xy+e_y < 0
	} while (cx < 0);
}

void drawCircle(uchar img[], int r, int c, int cR, int w, int h, uchar value, float thickness)
{
	int x, y;
	float f = thickness;
	for (x = -cR; x <= +cR; x++) // column
	{
		for (y = -cR; y <= +cR; y++) // row
		{
			if ((((x * x) + (y * y)) > (cR * cR) - (f / 2)) &&
				(((x * x) + (y * y)) < (cR * cR) + (f / 2)))
				setPixelValue(img, y + r, x + c, w, h, value);
		}
	}
}

void rasterCircle(uchar img[], int r, int c, int radius, int w, int h, uchar value)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;

	int x0 = r;
	int y0 = c;

	setPixelValue(img, x0, y0 + radius, w, h, value);
	setPixelValue(img, x0, y0 - radius, w, h, value);
	setPixelValue(img, x0 + radius, y0, w, h, value);
	setPixelValue(img, x0 - radius, y0, w, h, value);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		setPixelValue(img, x0 + x, y0 + y, w, h, value);
		setPixelValue(img, x0 - x, y0 + y, w, h, value);
		setPixelValue(img, x0 + x, y0 - y, w, h, value);
		setPixelValue(img, x0 - x, y0 - y, w, h, value);
		setPixelValue(img, x0 + y, y0 + x, w, h, value);
		setPixelValue(img, x0 - y, y0 + x, w, h, value);
		setPixelValue(img, x0 + y, y0 - x, w, h, value);
		setPixelValue(img, x0 - y, y0 - x, w, h, value);
	}
}

void drawOrientation(uchar img[], int x, int y, float ori, int cR, int w, int h, uchar value)
{
	int xe = (int)(x + cos(ori) * cR), ye = (int)(y + sin(ori) * cR);
	// Bresenham's line algorithm
	int dx = abs(xe - x), sx = x < xe ? 1 : -1;
	int dy = -abs(ye - y), sy = y < ye ? 1 : -1;
	int err = dx + dy, e2; /* error value e_xy */

	for (;;) { /* loop */
		setPixelValue(img, y, x, w, h, value);
		if (x == xe && y == ye)
			break;
		e2 = 2 * err;
		if (e2 >= dy) {
			err += dy;
			x += sx;
		} /* e_xy+e_x > 0 */
		if (e2 <= dx) {
			err += dx;
			y += sy;
		} /* e_xy+e_y < 0 */
	}
}


void drawKeypoints(uchar img[], int w, int h, ezsift::SiftKeypointList kpt_list, uchar value)
{
	ezsift::SiftKeypointList::iterator it;
	int r, c;

	/*******************************
	* cR:
	* radius of the circle
	* cR = sigma * 4 * (2^O)
	*******************************/
	int cR;

	for (it = kpt_list.begin(); it != kpt_list.end(); it++) {
		// derive circle radius cR
		cR = (int)it->scale;
		if (cR <= 1) { // avoid zero radius
			cR = 1;
		}
		r = (int)it->r;
		c = (int)it->c;
		//  draw_red_circle(&imgPPM, r, c, cR);
		rasterCircle(img, r, c, cR, w, h, value);
		rasterCircle(img, r, c, cR + 1, w, h, value);
		float ori = it->ori;
		drawOrientation(img, c, r, ori, cR, w, h, value);
	}
}

void drawLine(uchar img[], int w, int h, int r1, int c1, int r2, int c2, uchar value)
{
	float k = (float)(r2 - r1) / (float)(c2 - c1);
	for (int c = c1; c < c2; c++) {
		// Line equation
		int r = (int)(k * (c - c1) + r1);
		setPixelValue(img, r, c, w, h, value);
	}
}

void setPixelValueRGB(uchar img[], int r, int c, int w, int h, uchar R, uchar G, uchar B)
{
	if ((r >= 0) && (r < h) && (c >= 0) && (c < w)) {
		img[r * w * 3 + c * 3] = R;
		img[r * w * 3 + c * 3 + 1] = G;
		img[r * w * 3 + c * 3 + 2] = B;
	}
}

void drawCircleRGB(uchar img[], int r, int c, int cR, int w, int h, uchar R, uchar G, uchar B)
{
	int cx = -cR, cy = 0, err = 2 - 2 * cR; // II. Quadrant
	do {
		setPixelValueRGB(img, r - cx, c + cy, w, h, R, G, B); //   I. Quadrant
		setPixelValueRGB(img, r - cy, c - cx, w, h, R, G, B); //  II. Quadrant
		setPixelValueRGB(img, r + cx, c - cy, w, h, R, G, B); // III. Quadrant
		setPixelValueRGB(img, r + cy, c + cx, w, h, R, G, B); //  IV. Quadrant
		cR = err;
		if (cR > cx)
			err += ++cx * 2 + 1; // e_xy+e_x > 0
		if (cR <= cy)
			err += ++cy * 2 + 1; // e_xy+e_y < 0
	} while (cx < 0);
}

void rasterCircleRGB(uchar img[], int r, int c, int radius, int w, int h, uchar R, uchar G, uchar B)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;

	int x0 = r;
	int y0 = c;

	setPixelValueRGB(img, x0, y0 + radius, w, h, R, G, B);
	setPixelValueRGB(img, x0, y0 - radius, w, h, R, G, B);
	setPixelValueRGB(img, x0 + radius, y0, w, h, R, G, B);
	setPixelValueRGB(img, x0 - radius, y0, w, h, R, G, B);

	while (x < y) {
		// ddF_x == 2 * x + 1;
		// ddF_y == -2 * y;
		// f == x*x + y*y - radius*radius + 2*x - y + 1;
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		setPixelValueRGB(img, x0 + x, y0 + y, w, h, R, G, B);
		setPixelValueRGB(img, x0 - x, y0 + y, w, h, R, G, B);
		setPixelValueRGB(img, x0 + x, y0 - y, w, h, R, G, B);
		setPixelValueRGB(img, x0 - x, y0 - y, w, h, R, G, B);
		setPixelValueRGB(img, x0 + y, y0 + x, w, h, R, G, B);
		setPixelValueRGB(img, x0 - y, y0 + x, w, h, R, G, B);
		setPixelValueRGB(img, x0 + y, y0 - x, w, h, R, G, B);
		setPixelValueRGB(img, x0 - y, y0 - x, w, h, R, G, B);
	}
}

void drawOrientationRGB(uchar img[], int x, int y, float ori, int cR, int w, int h, uchar R, uchar G, uchar B)
{
	int xe = (int)(x + cos(ori) * cR), ye = (int)(y + sin(ori) * cR);
	// Bresenham's line algorithm
	int dx = abs(xe - x), sx = x < xe ? 1 : -1;
	int dy = -abs(ye - y), sy = y < ye ? 1 : -1;
	int err = dx + dy, e2; /* error value e_xy */

	for (;;) { /* loop */
		setPixelValueRGB(img, y, x, w, h, R, G, B);
		if (x == xe && y == ye)
			break;
		e2 = 2 * err;
		if (e2 >= dy) {
			err += dy;
			x += sx;
		} /* e_xy+e_x > 0 */
		if (e2 <= dx) {
			err += dx;
			y += sy;
		} /* e_xy+e_y < 0 */
	}
}


void drawKeypointsRGB(uchar img[], int w, int h, ezsift::SiftKeypointList kpt_list, uchar R, uchar G, uchar B)
{
	ezsift::SiftKeypointList::iterator it;
	int r, c;

	/*******************************
	* cR:
	* radius of the circle
	* cR = sigma * 4 * (2^O)
	*******************************/
	int cR;

	int i, j;

	for (it = kpt_list.begin(); it != kpt_list.end(); it++) {
		// derive circle radius cR
		cR = (int)it->scale;
		if (cR <= 1) { // avoid zero radius
			cR = 1;
		}
		r = (int)it->r;
		c = (int)it->c;
		//  draw_red_circle(&imgPPM, r, c, cR);
		rasterCircleRGB(img, r, c, cR, w, h, R, G, B);
		rasterCircleRGB(img, r, c, cR + 1, w, h, R, G, B);
		float ori = it->ori;
		drawOrientationRGB(img, c, r, ori, cR, w, h, R, G, B);
	}
}

void drawLineRGB(uchar img[], int w, int h, int r1, int c1, int r2, int c2, uchar R, uchar G, uchar B)
{
	float k = (float)(r2 - r1) / (float)(c2 - c1);
	for (int c = c1; c < c2; c++) {
		// Line equation
		int r = (int)(k * (c - c1) + r1);
		setPixelValueRGB(img, r, c, w, h, R, G, B);
	}
}
