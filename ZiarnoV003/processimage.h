#ifndef PROCESSIMAGE
#define PROCESSIMAGE

#include <boost/filesystem.hpp>
#include <opencv2/core/core.hpp>

using namespace boost::filesystem;
using namespace cv;
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
Mat LoadImage(path FileToOpen);
//Mat FindMaskFromGray(Mat ImIn,int thesholdVal);
//Mat FindMaskFromGradient(Mat ImIn,int thesholdVal);
int processFile(path FileToOpen,float thershod);

//float AverageMaskedPixelsF(Mat Reg, Mat ImF);

//---------------------------------------------------------------------------------------------
#endif // PROCESSIMAGE

