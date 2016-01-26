#include "processimage.h"

#include "ZiarnoMainWindow.h"
#include "ui_ZiarnoMainWindow.h"

#include "gradient.h"

#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;
//---------------------------------------------------------------------------------------------
//              funtions
//---------------------------------------------------------------------------------------------
Mat LoadImage(path FileToOpen)
{
    return imread(FileToOpen.string().c_str());
}
//---------------------------------------------------------------------------------------------
Mat FindMaskFromGray(Mat ImIn,int thesholdVal)
{
    Mat ImGray;
    cvtColor(ImIn,ImGray,CV_BGR2GRAY);

    Mat ImMask;
    threshold(ImGray, ImMask, thesholdVal ,1,THRESH_BINARY);
    ImMask.convertTo(ImMask, CV_16U);
    return ImMask;
}
//---------------------------------------------------------------------------------------------
Mat FindMaskFromGradient(Mat ImIn,int thesholdVal)
{
    Mat ImGray;
    cvtColor(ImIn,ImGray,CV_BGR2GRAY);

    Mat ImGradient = GradientDown(ImGray);


    Mat ImMask = GradientThresh(ImGradient,thesholdVal);

    return ImMask;
}
//---------------------------------------------------------------------------------------------

int processFile(path FileToOpen,float threshVal)
{
    //load file
    Mat ImIn = imread(FileToOpen.string().c_str());
    int maxX,maxY;
    maxX = ImIn.cols;
    maxY = ImIn.rows/2;
    Mat ImIn1, ImIn2;
    ImIn(Rect(0, 0, maxX, maxY)).copyTo(ImIn1);
    ImIn(Rect(0, maxY, maxX, maxY)).copyTo(ImIn2);

      //histogram calculation and display
    vector<Mat> bgr_planes;
    split(ImIn1,bgr_planes);

    int histSize = 256;
    float range[] = {0,256};
    const float* histRange = {range};
    bool histUniform = true;
    bool histAccumulate = false;

    Mat b_hist, g_hist, r_hist, gray_hist;

    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, histUniform, histAccumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, histUniform, histAccumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, histUniform, histAccumulate );

    //calcHist(&ImGray1,1,0,Mat(),gray_hist,1,&histSize, &histRange, histUniform, histAccumulate );

/*

    int hist_w = 1536; int hist_h = 500;
    int bin_w = cvRound( (double) hist_w/histSize );

    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 255,255,255) );

    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

    for( int i = 0; i < histSize; i++ )
    {
        line( histImage, Point( i * 9 + 0, hist_h) ,
                         Point( i * 9 + 0, hist_h - cvRound(b_hist.at<float>(i)) ),
                         Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( i * 9 + 3, hist_h) ,
                         Point( i * 9 + 3, hist_h - cvRound(g_hist.at<float>(i)) ),
                         Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( i * 9 + 6, hist_h),
                         Point( i * 9 + 6, hist_h - cvRound(r_hist.at<float>(i)) ),
                         Scalar( 0, 0, 255), 2, 8, 0  );
       // /*
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                         Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                         Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                         Scalar( 0, 0, 255), 2, 8, 0  );
       //
    }
*/

    int hist_w = 256; int hist_h = 500;
    int bin_w = cvRound( (double) hist_w/histSize );

    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 255,255,255) );

    normalize(gray_hist, gray_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    for( int i = 0; i < histSize; i++ )
    {
        line( histImage, Point( i, hist_h) ,
                         Point( i, hist_h - cvRound(gray_hist.at<float>(i)) ),
                         Scalar( 255, 0, 0), 2, 8, 0  );
    }

    imshow("Histogram", histImage );




    return 0;
}
