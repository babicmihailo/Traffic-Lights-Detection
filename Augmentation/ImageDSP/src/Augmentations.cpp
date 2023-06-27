#include <math.h>
#include "Augmentations.h"

void calculateHistogram(
    const uchar input[],
    int x_size,
    int y_size,
    int histogram[]
) {
    // TODO
    for (int i = 0; i < x_size; ++i) {
        for (int j = 0; j < y_size; ++j) {
            histogram[input[i * x_size + j]]++;
        }
    }
}


/**
 * Equalize image histogram
 */
void myAugmentation0(
    const uchar input[],
    int x_size,
    int y_size,
    uchar output[],
    uchar L
) {
    // TODO
    int histogram[256] = {0};
    calculateHistogram(input, x_size, y_size, histogram);

    int cdf[256] = {0};
    int sum = 0;
    for (int i = 0; i < 256; ++i) {
        sum += histogram[i];
        cdf[i] = sum;
    }

    int minCdf = 0;
    for (int i = 0; i < 256; ++i) {
        if (cdf[i] > 0)
        {
            minCdf = cdf[i];
            break;
        }
    }

    for (int i = 0; i < x_size*y_size; ++i) {
        output[i] = round(float((L*(cdf[input[i]] - minCdf)))/float(x_size*y_size - minCdf));
    }
}

double odradiGa(double input) {
    if (input > 0) {
            if (input > 255)
                input = 255;
        }
    else
        input = 0;
    return input;
}

void myAugmentation1(
    const uchar inputRGB[],
    const uchar inputY[],
    int x_size,
    int y_size,
    uchar outputRGB[],
    double S
) {
    // TODO
    double R, G, B;
    for (int i = 0; i < x_size*y_size; ++i) {
        R = inputRGB[3*i] * S + inputY[i] * (1 - S);
        G = inputRGB[3*i + 1] * S + inputY[i] * (1 - S);
        B = inputRGB[3*i + 2] * S + inputY[i] * (1 - S);


        outputRGB[3*i] = odradiGa(R);
        outputRGB[3*i+1] = odradiGa(G);
        outputRGB[3*i+2] = odradiGa(B);
    }
}


void myAugmentation2(
    const uchar input[],
    int x_size,
    int y_size,
    uchar output[],
    int m,
    int n,
    double angle
) {
    angle = (angle * M_PI) / 180.0;

    for(int y = 0; y < y_size; y++){
        for(int x = 0; x < x_size; x++){
            output[3 * (y*x_size + x)] = 0;
            output[3 * (y*x_size + x) + 1] = 0;
            output[3 * (y*x_size + x) + 2] = 0;
        }
    }

    for(int y = 0; y < y_size; y++){
        for(int x = 0; x < x_size; x++){
            int oy = y * cos(angle) + x * sin(angle) - m* sin(angle) - n* cos(angle) + n;
            int ox = x * cos(angle) - y * sin(angle) - m* cos(angle) + n* sin(angle) + m;
            if ((oy < y_size && oy > 0) && (ox < x_size && ox > 0) ) {
                output[3 * (y*x_size + x)] = input[3 * (oy*x_size + ox)];
                output[3 * (y*x_size + x) + 1] = input[3 * (oy*x_size + ox) + 1];
                output[3 * (y*x_size + x) + 2] = input[3 * (oy*x_size + ox) + 2];
            }
        }
    }
}

// define occlusion colors
const uchar occlusion_colors[][3] = {
    {255, 0, 0},     // red
    {255, 255, 0},   // yellow
    {0, 255, 0}      // green
};

void myAugmentation3(
    const uchar input[],
    int x_size,
    int y_size,
    uchar output[],
    double occlusion_probability
) {
    srand(time(NULL)); // initialize random seed

    for (int y = 0; y < y_size; y++) {
        for (int x = 0; x < x_size; x++) {
            if ((rand() % 100) < occlusion_probability * 10) {
                const uchar* occlusion_color = occlusion_colors[rand() % 3];
                output[3 * (y * x_size + x)] = occlusion_color[0];
                output[3 * (y * x_size + x) + 1] = occlusion_color[1];
                output[3 * (y * x_size + x) + 2] = occlusion_color[2];
            }
            else {
                output[3 * (y * x_size + x)] = input[3 * (y * x_size + x)];
                output[3 * (y * x_size + x) + 1] = input[3 * (y * x_size + x) + 1];
                output[3 * (y * x_size + x) + 2] = input[3 * (y * x_size + x) + 2];
            }
        }
    }
}

