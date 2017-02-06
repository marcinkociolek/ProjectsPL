#ifndef SegmentImageH
#define SegmentImageH

#include <opencv2/core/core.hpp>
#include "mazdaroi.h"
#include "mazdaroiio.h"

typedef MazdaRoi<unsigned int, 2> MR2DType;

using namespace cv;

Mat FindMaskFromGray(Mat ImIn,int thesholdVal);
Mat FindMaskFromGradient(Mat ImIn,int thesholdVal);
float AverageMaskedPixelsF(Mat Reg, Mat ImF);
//vector <MR2DType*> SegmentGrainImg(IplImage ImIpl);
vector <MR2DType*> SegmentGrainImg(Mat ImIpl);

#endif 
