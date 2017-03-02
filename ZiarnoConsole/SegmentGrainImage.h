#ifndef SegmentImageH
#define SegmentImageH

#include <opencv2/core/core.hpp>
#include "../../qmazda/SharedImage/mazdaroi.h"
#include "../../qmazda/SharedImage/mazdaroiio.h"

typedef MazdaRoi<unsigned int, 2> MR2DType;

using namespace cv;

struct TransformacjaZiarna
{
    double x; // dane o położeniu środka ziarniaka w obrazie oryginalnym
    double y;
    double angle; // kąt obrotu obrazu wynikowego względem oryginalnego
};

Mat Combine2Images( Mat Im1, Mat Im2);
Mat FindMaskFromGray(Mat ImIn,int thesholdVal);
Mat FindMaskFromGradient(Mat ImIn,int thesholdVal);
float AverageMaskedPixelsF(Mat Reg, Mat ImF);
//vector <MR2DType*> SegmentGrainImg(IplImage ImIpl);
//vector <MR2DType*> SegmentGrainImg(Mat *ImIpl);

bool SegmentGrainImg(const std::vector<Mat*> *ImInVect, std::vector<Mat*> *ImOutVect, vector <MR2DType*> * outRoi, std::vector<TransformacjaZiarna> *transf);

#endif 
