#include <QCoreApplication>
#include "SegmentGrainImage.h"

#include <vector>

//#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include "gradient.h"
//#include "DispLib.h"

#include "mazdaroi.h"
#include "mazdaroiio.h"

typedef MazdaRoi<unsigned int, 2> MR2DType;


using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    IplImage *wImIpl = cvLoadImage("E:\\TestFigs\\Ziarno\\OK_00029.png");

    vector <MR2DType*> rois;

    Mat ImIn = cvarrToMat(wImIpl);
    rois = SegmentGrainImg(&ImIn);

    imshow("ImageOut", ImIn);
    waitKey();

    // save output ROI
    MazdaRoiIO<MR2DType>::Write("E:\\TestFigs\\Ziarno\\OK_00029Roi.tif", &rois, NULL);

    while(rois.size() > 0)
    {
         delete rois.back();
         rois.pop_back();
    }
    return 0;



}
