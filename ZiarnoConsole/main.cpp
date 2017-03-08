#include <QCoreApplication>
#include "SegmentGrainImage.h"

#include <vector>

//#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include "gradient.h"
#include "DispLib.h"

#include "mazdaroi.h"
#include "mazdaroiio.h"

typedef MazdaRoi<unsigned int, 2> MR2DType;


using namespace std;
using namespace cv;

//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    IplImage *wImIpl = cvLoadImage("C:\\Data\\Ziarno\\TestFigs\\ZIEL_ZAD_00000.png");

    vector <MR2DType*> RoiVect;

    Mat ImIn = cvarrToMat(wImIpl);

    if (ImIn.empty())
        return 0;

    int maxX = ImIn.cols/2;
    int maxY = ImIn.rows;
    //int maxX = ImIn.cols;
    //int maxY = ImIn.rows/2;

    Mat ImIn1, ImIn2;

    (ImIn)(Rect(0, 0, maxX, maxY)).copyTo(ImIn1);
    (ImIn)(Rect(maxX, 0, maxX, maxY)).copyTo(ImIn2);
    //(ImIn)(Rect(0, 0, maxX, maxY)).copyTo(ImIn1);
    //(ImIn)(Rect(0, maxY, maxX, maxY)).copyTo(ImIn2);

    vector<Mat*> ImInVect;
    ImInVect.push_back(&ImIn1);
    ImInVect.push_back(&ImIn2);

    vector<Mat*> ImOutVect;

    ;
    vector<TransformacjaZiarna> TransfVect;
    SegmentParams segParams;
    segParams.defaultHorizontal();

    SegmentGrainImg(&ImInVect, &ImOutVect, &RoiVect,&TransfVect,&segParams);

    //rois = SegmentGrainImg(&ImIn);

    //imshow("ImageIn", Combine2Images(*ImInVect[0],*ImInVect[1]));
    //imshow("ImageOut", Combine2Images(*ImOutVect[0],*ImOutVect[1]));
    ShowImageCombination(true, "ImageIn", *ImInVect[0],*ImInVect[1]);
    ShowImageCombination(true, "ImageOut", *ImOutVect[0],*ImOutVect[1]);
    waitKey();

    // save output ROI
    MazdaRoiIO<MR2DType>::Write("C:\\Data\\Ziarno\\TestFigs\\ZIEL_ZAD_00000Roi.tif", &RoiVect, NULL);

    while(RoiVect.size() > 0)
    {
         delete RoiVect.back();
         RoiVect.pop_back();
    }
    return 0;



}
