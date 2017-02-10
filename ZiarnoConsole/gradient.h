#ifndef GRADIENT
#define GRADIENT

#include <opencv2/core/core.hpp>

using namespace cv;


Mat GradientDown(Mat ImIn);
Mat HorizontalGradientDown(Mat ImIn);
Mat HorizontalAbsGradient(Mat ImIn);

Mat GradientThresh(Mat ImIn,float threshold);
Mat RemovingTinyReg9(Mat ImReg);
void FillBorderWithValue(Mat ImReg, float value);
void OneRegionFill5Fast1(Mat ImR,  unsigned short fillReg);
void FillHoles(Mat ImR);
void DeleteRegionFromImage(Mat ImR, unsigned short regToRemove);
unsigned short DivideSeparateRegions(Mat ImReg, unsigned int minRegSize);
void RegionExchange(unsigned short *ImReg, int stop,
                    unsigned short rI, unsigned  short rO);

void DeleteRegTouchingBorder(Mat ImReg);

void RegionDilation5(Mat ImR);
void RegionDilation9(Mat ImR);
void RegionDilation13(Mat ImR);

void RegionErosion5(Mat ImR);
void RegionErosion9(Mat ImR);
void RegionErosion13(Mat ImR);

Mat GetContour5(Mat ImR);

//void ShowRegionOnImage16(Mat ImReg, Mat ImRGB);

#endif // GRADIENT

