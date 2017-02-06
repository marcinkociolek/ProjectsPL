#include <QCoreApplication>
#include "SegmentGrainImage.h"

#include <vector>

//#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//#include "tiffio.h"

#include "gradient.h"
#include "DispLib.h"

#include "mazdaroi.h"
#include "mazdaroiio.h"

typedef MazdaRoi<unsigned int, 2> MR2DType;

//QFileSystemModel *model;
//QAbstractItemModel *model;


using namespace std;
//using namespace boost::filesystem;
//using namespace boost;
using namespace cv;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    IplImage ImIpl = imread("D:\\ProjectsPL\\ZiarnoConsole\\OK_00000.png");
    //Mat ImIpl = imread("D:\\ProjectsPL\\ZiarnoConsole\\OK_00000.png");
    vector <MR2DType*> rois;
    rois = SegmentGrainImg(ImIpl);



    // save output ROI
    MazdaRoiIO<MR2DType>::Write("D:\\ProjectsPL\\ZiarnoConsole\\ROIs.tif", &rois, NULL);

    while(rois.size() > 0)
    {
         delete rois.back();
         rois.pop_back();
    }
    return a.exec();



}
