#ifndef GRADIENT
#define GRADIENT

#include <opencv2/core/core.hpp>

// using namespace cv;


cv::Mat GradientDown(cv::Mat ImIn);
cv::Mat HorizontalGradientDown(cv::Mat ImIn);
cv::Mat HorizontalAbsGradient(cv::Mat ImIn);

cv::Mat GradientThresh(cv::Mat ImIn,float threshold);
cv::Mat RemovingTinyReg9(cv::Mat ImReg);
void FillBorderWithValue(cv::Mat ImReg, float value);
void OneRegionFill5Fast1(cv::Mat ImR,  unsigned short fillReg);
void FillHoles(cv::Mat ImR);
void DeleteRegionFromImage(cv::Mat ImR, unsigned short regToRemove);
unsigned short DivideSeparateRegions(cv::Mat ImReg, unsigned int minRegSize);
void RegionExchange(unsigned short *ImReg, int stop,
                    unsigned short rI, unsigned  short rO);

void DeleteRegTouchingBorder(cv::Mat ImReg);

void RegionDilation5(cv::Mat ImR);
void RegionDilation9(cv::Mat ImR);
void RegionDilation13(cv::Mat ImR);

void RegionErosion5(cv::Mat ImR);
void RegionErosion9(cv::Mat ImR);
void RegionErosion13(cv::Mat ImR);

//cv::Mat GetContour5(cv::Mat ImR);

//void ShowRegionOnImage16(Mat ImReg, Mat ImRGB);

#endif // GRADIENT

