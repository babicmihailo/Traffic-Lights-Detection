
#pragma once

#include <QDebug>

void myAugmentation0(
    const uchar input[],
    int x_size,
    int y_size,
    uchar output[],
    uchar L
);

void myAugmentation1(
    const uchar inputRGB[],
    const uchar inputY[],
    int x_size,
    int y_size,
    uchar outputRGB[],
    double S
);


void myAugmentation2(
    const uchar input[],
    int x_size,
    int y_size,
    uchar output[],
    int m,
    int n,
    double angle
);


void myAugmentation3(
    const uchar input[],
    int x_size,
    int y_size,
    uchar output[],
    double occlusion_probability
);
