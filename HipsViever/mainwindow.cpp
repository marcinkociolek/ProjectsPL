#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <string>
#include <iostream>
#include <fstream>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "DispLib.h"
#include "gradient.h"
#include "RegionU16Lib.h"
#include "StringFcLib.h"

using namespace std;
using namespace boost::filesystem;
using namespace boost;
using namespace cv;
//--------------------------------------------------------------------------------------------------
double JaccardIndex(Mat MaskRef, Mat Mask, int *maskCountOut, int *maskRefCountOut)
{
    if(MaskRef.empty())
        return -1.0;
    if(Mask.empty())
        return -1.0;
    if(MaskRef.cols != Mask.cols)
        return -1.0;
    if(MaskRef.rows != Mask.rows)
        return -1.0;

    int maxXY = Mask.cols * Mask.rows;

    int andCount = 0;
    int orCount = 0;
    int maskRefCount = 0;
    int maskCount = 0;

    unsigned short *wMask = (unsigned short *)Mask.data;
    unsigned short *wMaskRef = (unsigned short *)MaskRef.data;
    for(int i = 0; i < maxXY; i++)
    {
        if(*wMaskRef)
            maskRefCount++;
        if(*wMask)
            maskCount++;
        if(*wMaskRef && *wMask)
            andCount++;
        if(*wMaskRef || *wMask)
            orCount++;

        wMaskRef++;
        wMask++;
    }
    *maskCountOut = maskCount;
    *maskRefCountOut = maskRefCount;
    return (double)andCount/(double)orCount;
}

//--------------------------------------------------------------------------------------------------
//          My functions
//--------------------------------------------------------------------------------------------------
void MainWindow::CalculateSDA(void)
{
    if(!calculateSDA)
        return;
    if(ImIn.empty())
        return;

    //unsigned short *wImIn = (unsigned short*)ImIn.data;
    //float *wImGradient = (float*)ImGradient.data;

    int maxX = ImIn.cols;
    int maxY = ImIn.rows;
    //int maxXY = maxX * maxY;

    int diameter = kernelSizeSDA;
    Mat RoiSmall = CreateRoi16(2, diameter, diameter);

    unsigned short *wRoiSmall = (unsigned short*)RoiSmall.data;
    int roiPixCount = 0;
    for(int y = 0; y < RoiSmall.rows; y++)
    {
        for(int x = 0; x < RoiSmall.cols; x++)
        {
            if(*wRoiSmall)
                roiPixCount++;
            wRoiSmall++;
        }
    }
    kernelPixelCountSDA = roiPixCount;
    ImSDA = Mat::zeros(maxY, maxX, CV_16U);

    unsigned short *wImSDA = (unsigned short*)ImSDA.data;
    unsigned short *wImIn = (unsigned short*)ImIn.data;

    int xLimMin = diameter/2 +1;
    int xLimMax = maxX - diameter/2;
    int yLimMin = diameter/2 + 1;
    int yLimMax = maxY - diameter/2;
    int maxXYSmall = diameter *diameter;
    int radius = diameter/2;

    for(int y = 0; y < maxY; y++)
    {
        for(int x = 0; x < maxX; x++)
        {
            if(x > xLimMin && x < xLimMax && y > yLimMin && y < yLimMax )
            {
                Mat ImSmall;
                unsigned short pixVal = *wImIn;
                int sum = 0;
                ImIn(Rect(x - radius, y - radius, diameter,diameter)).copyTo(ImSmall);
                unsigned short *wImSmall = (unsigned short*)ImSmall.data;
                unsigned short *wRoiSmall = (unsigned short*)RoiSmall.data;
                for(int i = 0; i < maxXYSmall; i++)
                {
                    if(*wImSmall >= pixVal && *wRoiSmall)
                    {
                       sum++;
                    }
                    wImSmall++;
                    wRoiSmall++;
                }
                *wImSDA = sum;
            }

            wImIn++;
            wImSDA++;
        }
    }

    wImSDA = (unsigned short*)ImSDA.data;
    ImNormInvSDA = Mat::zeros(maxY,maxX,CV_8U);
    unsigned char* wImNormInvSDA = (unsigned char* )ImNormInvSDA.data;

    wImSDA = (unsigned short*)ImSDA.data;
    double coeff = 255.0/((double)kernelPixelCountSDA);
    for(int y = 0; y < maxY; y++)
    {
        for(int x = 0; x < maxX; x++)
        {
            if(x > xLimMin && x < xLimMax && y > yLimMin && y < yLimMax )
            {
                double val = ((double)(kernelPixelCountSDA - *wImSDA))*coeff;
                if(val<0)
                   val=0;
                if(val>255)
                   val=255;
                *wImNormInvSDA = (unsigned char)val;

            }
            wImNormInvSDA++;
            wImSDA++;
        }
    }


}
//--------------------------------------------------------------------------------------------------
cv::Mat CalculateSDAL(cv::Mat ImIn, cv::Mat Roi, int radius, int *outSDARoiPixCount = 0)
{

    if(Roi.empty())
        return Mat::zeros(0,0,CV_16U);
    if(ImIn.empty())
        return Mat::zeros(0,0,CV_16U);

    //unsigned short *wImIn = (unsigned short*)ImIn.data;
    //float *wImGradient = (float*)ImGradient.data;

    int maxX = ImIn.cols;
    int maxY = ImIn.rows;
    //int maxXY = maxX * maxY;
    if(Roi.cols != maxX)
        return Mat::zeros(0,0,CV_16U);
    if(Roi.rows != maxY)
        return Mat::zeros(0,0,CV_16U);


    int diameter = radius * 2 + 1;
    Mat RoiSmall = CreateRoi16(2, diameter, diameter);

    unsigned short *wRoiSmall = (unsigned short*)RoiSmall.data;
    int roiPixCount = 0;
    for(int y = 0; y < RoiSmall.rows; y++)
    {
        for(int x = 0; x < RoiSmall.cols; x++)
        {
            if(*wRoiSmall)
                roiPixCount++;
            wRoiSmall++;
        }
    }
    int kernelPixelCountSDA = roiPixCount;
    Mat ImSDA = Mat::zeros(maxY, maxX, CV_16U);

    unsigned short *wImSDA = (unsigned short*)ImSDA.data;
    unsigned short *wImIn = (unsigned short*)ImIn.data;
    unsigned short *wRoi = (unsigned short*)Roi.data;

    int xLimMin = radius +1;
    int xLimMax = maxX - radius;
    int yLimMin = radius + 1;
    int yLimMax = maxY - radius;
    int maxXYSmall = diameter *diameter;

    for(int y = 0; y < maxY; y++)
    {
        for(int x = 0; x < maxX; x++)
        {
            if (*wRoi)
            {
                if(x > xLimMin && x < xLimMax && y > yLimMin && y < yLimMax )
                {
                    Mat ImSmall;
                    unsigned short pixVal = *wImIn;
                    int sum = 0;
                    ImIn(Rect(x - radius, y - radius, diameter,diameter)).copyTo(ImSmall);
                    unsigned short *wImSmall = (unsigned short*)ImSmall.data;
                    unsigned short *wRoiSmall = (unsigned short*)RoiSmall.data;
                    for(int i = 0; i < maxXYSmall; i++)
                    {
                        if(*wImSmall >= pixVal && *wRoiSmall)
                        {
                           sum++;
                        }
                        wImSmall++;
                        wRoiSmall++;
                    }
                    *wImSDA = sum;
                }
            }
            else
                *wImSDA = 0;
            wRoi++;
            wImIn++;
            wImSDA++;
        }
    }
    *outSDARoiPixCount = roiPixCount;
    return ImSDA;

}

//--------------------------------------------------------------------------------------------------
cv::Mat CreateNormalisedSDA(cv::Mat ImSDA, int kernelPixelCountSDA)
{
    unsigned short *wImSDA = (unsigned short*)ImSDA.data;
    int maxX = ImSDA.cols;
    int maxY = ImSDA.rows;
    //int maxXY = maxX * maxY;

    Mat ImNormInvSDA = Mat::zeros(maxY,maxX,CV_8U);
    unsigned char* wImNormInvSDA = (unsigned char* )ImNormInvSDA.data;

    wImSDA = (unsigned short*)ImSDA.data;
    double coeff = 255.0/((double)kernelPixelCountSDA);
    for(int y = 0; y < maxY; y++)
    {
        for(int x = 0; x < maxX; x++)
        {
            //if(x > xLimMin && x < xLimMax && y > yLimMin && y < yLimMax )
            {
                double val = ((double)(kernelPixelCountSDA - *wImSDA))*coeff;
                if(val<0)
                   val=0;
                if(val>255)
                   val=255;
                *wImNormInvSDA = (unsigned char)val;

            }
            wImNormInvSDA++;
            wImSDA++;
        }
    }
    return ImNormInvSDA;
}
//--------------------------------------------------------------------------------------------------
cv::Mat CreateNormalisedSDA(cv::Mat ImSDA, cv::Mat Mask, int kernelPixelCountSDA)
{
    unsigned short *wImSDA = (unsigned short*)ImSDA.data;
    int maxX = ImSDA.cols;
    int maxY = ImSDA.rows;
    //int maxXY = maxX * maxY;

    Mat ImNormInvSDA = Mat::zeros(maxY,maxX,CV_8U);
    unsigned char* wImNormInvSDA = (unsigned char* )ImNormInvSDA.data;

    wImSDA = (unsigned short*)ImSDA.data;
    double coeff = 255.0/((double)kernelPixelCountSDA);
    for(int y = 0; y < maxY; y++)
    {
        for(int x = 0; x < maxX; x++)
        {
            //if(x > xLimMin && x < xLimMax && y > yLimMin && y < yLimMax )
            {
                double val = ((double)(kernelPixelCountSDA - *wImSDA))*coeff;
                if(val<0)
                   val=0;
                if(val>255)
                   val=255;
                *wImNormInvSDA = (unsigned char)val;

            }
            wImNormInvSDA++;
            wImSDA++;
        }
    }
    return ImNormInvSDA;
}
//--------------------------------------------------------------------------------------------------
void FindReginsTouchingImplant(Mat MaskImplant, Mat MaskSDA)
{
    if(MaskSDA.empty())
        return;
    if(MaskImplant.empty())
        return;

    int maxX = MaskSDA.cols;
    int maxY = MaskSDA.rows;

    Mat MaskOut = Mat::zeros(maxY,maxX, CV_16U);

    unsigned short *wMaskImplant = (unsigned short *)MaskImplant.data;
    unsigned short *wMaskImplantXP = wMaskImplant+1;
    unsigned short *wMaskImplantXM = wMaskImplant-1;
    unsigned short *wMaskSDA = (unsigned short *)MaskSDA.data;
    unsigned short *wMaskOut = (unsigned short *)MaskOut.data;
    for(int y = 0; y < maxY; y++)
    {
        for(int x = 0; x < maxX; x++)
        {
            if(*wMaskSDA)
            {
                if(*wMaskImplantXP ||*wMaskImplantXM)
                    *wMaskOut = 1;
            }
            wMaskImplantXP++;
            wMaskImplantXM++;
            wMaskOut++;
            wMaskSDA++;
        }
    }

    for(int i = 0; i < 40; i++)
    {
        Mat MaskTemp = Mat::zeros(maxY, maxX, CV_16U);
        unsigned short *wMaskTemp = (unsigned short *)MaskTemp.data;
        wMaskSDA = (unsigned short *)MaskSDA.data;
        wMaskOut = (unsigned short *)MaskOut.data;
        unsigned short *wMaskOutXP = wMaskOut + 1;
        unsigned short *wMaskOutXM = wMaskOut - 1;
        unsigned short *wMaskOutYP = wMaskOut + maxX;
        unsigned short *wMaskOutYM = wMaskOut - maxX;

        unsigned short *wMaskOut = (unsigned short *)MaskOut.data;
        for(int y = 0; y < maxY; y++)
        {
            for(int x = 0; x < maxX; x++)
            {
                if(*wMaskSDA)
                {
                    if(*wMaskOutXP ||*wMaskOutXM ||*wMaskOutYP ||*wMaskOutYM )
                        *wMaskTemp = 1;
                }
                wMaskOutXP++;
                wMaskOutXM++;
                wMaskOutYP++;
                wMaskOutYM++;
                wMaskOut++;
                wMaskSDA++;
                wMaskTemp++;
            }
        }
        MaskTemp.copyTo(MaskOut);
    }


    MaskOut.copyTo(MaskSDA);
}

//--------------------------------------------------------------------------------------------------
void MainWindow::OpenImage(void)
{
    ImIn = imread(FileToOpen.string().c_str(),CV_LOAD_IMAGE_ANYDEPTH);
    if(ImIn.empty())
        return;
    maxX = ImIn.cols;
    maxY = ImIn.rows;
    maxXY = maxX * maxY;

    //ImGradient = GradientDown(ImIn);
    ImGradient = GradientMorph(ImIn, 4);

    ShowImages();
    MaskImage();
}
//--------------------------------------------------------------------------------------------------
void MainWindow::ShowImages(void)
{
        //if(showInputGray)
    if(ImIn.empty())
        return;
    if(showInputGray)
        ImShowGray = ShowImage16Gray(ImIn,minShowGray,maxShowGray);
    if(showInputGray)
        imshow("Gray", ImShowGray);

    //if(showInputPseudocolor||showMask)
        ImShowPseudocolor = ShowImage16PseudoColor(ImIn,minShowPseudocolor,maxShowPseudocolor);
    if(showInputPseudocolor)
        imshow("Pseudocolor", ImShowPseudocolor);
    if(showGradient)
        imshow("Gradient", ShowImage16PseudoColor(ImGradient,minShowGradient,maxShowGradient));

}
//--------------------------------------------------------------------------------------------------
void MainWindow::MaskImage(void)
{
    if(ImIn.empty())
        return;
    MaskImplant = Threshold16(ImIn, thresholdImOrg);
    Threshold16(ImGradient, MaskImplant, thresholdGradient);

    if(closingShape)
    {
        //ErosionCV(MaskImplant, closingShape);
        DilationCV(MaskImplant, closingShape);
        ErosionCV(MaskImplant, closingShape);
    }

    if(fillHoles)
    {
        FillBorderWithValue(MaskImplant, 0xFFFF);
        OneRegionFill5Fast1(MaskImplant,  0xFFFF);
        FillHoles(MaskImplant);
        DeleteRegionFromImage(MaskImplant, 0xFFFF);
    }

    if(erosionShape)
    {
        ErosionCV(MaskImplant, erosionShape);
        DilationCV(MaskImplant, erosionShape);
    }

    if(divideSeparateRegions)
    {
        DivideSeparateRegions(MaskImplant, minRegionSize);
    }


    if(expandMask)
    {
        Mat Kernel = Mat::ones(1,expansionSize*2+1,CV_8U);
        dilate(MaskImplant,Mask,Kernel);
        Mask = Mask-MaskImplant;
    }
    if(expandMaskY)
    {
        Mat Kernel = Mat::zeros(expansionSize + 1,expansionSize * 2 + 1, CV_8U);
        ellipse(Kernel, Point(expansionSize, expansionSize),
            Size(expansionSize, expansionSize), 0.0, 180.0, 360.0,
            1, -1);

        //imshow("kernel",Kernel * 255);
        dilate(MaskImplant,Mask,Kernel,Point(expansionSize, expansionSize));

        Mask = Mask-MaskImplant;

    }

    if(croppMask)
    {
        unsigned short *wMask = (unsigned short *)Mask.data;
        int cropp = croppSize;
        if (cropp > maxY)
            cropp = maxY;

        for(int y = 0; y < cropp; y++)
        {
            for(int x = 0; x < maxX; x++)
            {
                *wMask = 0;
                wMask++;
            }
        }
    }



    ShowMask();

    EstymateSDA();


}
//--------------------------------------------------------------------------------------------------
void MainWindow::ShowMask(void)
{
    if(MaskImplant.empty())
        return;
    if(showMaskImplant)
    {
        Mat ImShowMask;
        if(showContour)
            ImShowMask = ShowSolidRegionOnImageInBlack(GetContour5(MaskImplant), ImShowPseudocolor);
        else
            ImShowMask = ShowSolidRegionOnImageInBlack(MaskImplant, ImShowPseudocolor);
        imshow("Mask Implant", ImShowMask);
    }
    if(Mask.empty())
        return;
    if(showMask)
    {
        Mat ImShowMask;
        if(showContour)
            ImShowMask = ShowSolidRegionOnImageInBlack(GetContour5(Mask), ImShowPseudocolor);
        else
            ImShowMask = ShowSolidRegionOnImageInBlack(Mask, ImShowPseudocolor);
        imshow("Mask", ImShowMask);
    }
}
//--------------------------------------------------------------------------------------------------
void MainWindow::EstymateSDA(void)
{
    if(ImIn.empty())
        return;
    if(!calculateSDA)
        return;
    //kernelPixelCountSDA;
    switch(sdaSize)
    {
    case 1:
        ImSDA = CalculateSDAL(ImIn, Mask + MaskImplant, kernelSizeSDA, &kernelPixelCountSDA);
        break;
    case 2:
        ImSDA = CalculateSDAL(ImIn, Mat::ones(ImIn.rows,ImIn.cols, CV_16U), kernelSizeSDA, &kernelPixelCountSDA);
        break;

    default:
        ImSDA = CalculateSDAL(ImIn, Mask, kernelSizeSDA, &kernelPixelCountSDA);
        break;
    }
    ImNormInvSDA = CreateNormalisedSDA(ImSDA, kernelPixelCountSDA);
    //


    //CalculateSDA();
    ShowSDA();
    PostSDA();
}
//--------------------------------------------------------------------------------------------------
void MainWindow::ShowSDA(void)
{
    if(ImSDA.empty())
        return;
    ImShowSDA = ShowImage16PseudoColor(ImSDA,minShowSDA,maxShowSDA);
    if(showSDA)
    {

        imshow("SDA", ImShowSDA);
    }
    if(ImNormInvSDA.empty())
        return;
    if(showSDANorm)
    {
        imshow("SDA norm", ImNormInvSDA);
    }
    if(showSDANormPC)
    {
        ShowImage8PseudoColor(ImNormInvSDA, 0, 255);
        imshow("SDA norm PC", ShowImage8PseudoColor(ImNormInvSDA, 0.0, 255.0));
    }
}

//--------------------------------------------------------------------------------------------------
void MainWindow::PostSDA(void)
{
    if(thresholdSDA && !ImSDA.empty())
    {
        unsigned short *wImSDA = (unsigned short*)ImSDA.data;

        MaskSDA = Mat::zeros(maxY, maxX, CV_16U);
        unsigned short *wMaskSDA;

        wMaskSDA = (unsigned short*)MaskSDA.data;

        for(int i = 0; i < maxXY; i++)
        {

            if(*wImSDA > thresholdImSDA)
                *wMaskSDA = 1;
            wImSDA++;
            wMaskSDA++;
        }

        wMaskSDA = (unsigned short*)MaskSDA.data;
        unsigned short *wMask = (unsigned short*)Mask.data;

    }


    if(postErosionShape1)
        ErosionCV(MaskSDA, postErosionShape1);
    if(postDilationShape2)
        DilationCV(MaskSDA, postDilationShape2);
    if(postErosionShape3)
        ErosionCV(MaskSDA, postErosionShape3);
    if(fillHolesOnOutMask)
    {
        FillBorderWithValue(MaskSDA, 0xFFFF);
        OneRegionFill5Fast1(MaskSDA,  0xFFFF);
        FillHoles(MaskSDA);
        DeleteRegionFromImage(MaskSDA, 0xFFFF);
    }

    if(findRegTouchingImplant)
    {
        FindReginsTouchingImplant(MaskImplant, MaskSDA);
    }


    if(divideSeparateRegionsSDA)
    {
        DivideSeparateRegions(MaskSDA, minRegionSizeSDA);
    }


    int maskSDACount;
    int maskSDARefCount;

    jaccard = JaccardIndex(MaskSDARef, MaskSDA, &maskSDACount, &maskSDARefCount);

    if(!stopDisplay)
    {
        LocalString = FileToOpen.stem().string() + "\t";

        LocalString += to_string(kernelSizeSDA) + "\t";
        LocalString += to_string(kernelPixelCountSDA) + "\t";

        LocalString += to_string(thresholdImSDA) + "\t";
        LocalString += to_string(jaccard) + "\t";
        LocalString += to_string(maskSDARefCount) + "\t";
        LocalString += to_string(maskSDACount);
        ui->lineEditLocalOut->setText(LocalString.c_str());

        ShowResults();
    }
}
//--------------------------------------------------------------------------------------------------
void MainWindow::ShowResults(void)
{

    if(MaskSDA.empty())
        return;

    if(showContour)
        ImShowOutput = ShowSolidRegionOnImageInBlack(GetContour5(MaskSDA), ImShowPseudocolor);
    else
        ImShowOutput = ShowSolidRegionOnImageInBlack(MaskSDA, ImShowPseudocolor);
    if(showOutput)
    {
        imshow("Output", ImShowOutput);
    }
    if(ImShowSDA.empty())
        return;


    if(showContour)
        ImShowOutputOnSDA = ShowSolidRegionOnImageInBlack(GetContour5(MaskSDA), ImShowSDA);
    else
        ImShowOutputOnSDA = ShowSolidRegionOnImageInBlack(MaskSDA, ImShowSDA);
    if(showOutputOnSDA)
    {
        imshow("Output on SDA", ImShowOutputOnSDA);
    }
}

//--------------------------------------------------------------------------------------------------
void MainWindow::ProcessImage(void)
{
    if(ImIn.empty())
        return;

    //unsigned short *wImIn = (unsigned short*)ImIn.data;
    //float *wImGradient = (float*)ImGradient.data;

    int maxX = ImIn.cols;
    int maxY = ImIn.rows;
    int maxXY = maxX * maxY;

    if(ImSDA.empty())
        return;

    unsigned short *wImConv = (unsigned short*)ImSDA.data;

    Mask = Mat::zeros(maxY, maxX, CV_16U);
    unsigned short *wMask = (unsigned short*)Mask.data;

    for(int i = 0; i < maxXY; i++)
    {

        if(*wImConv > thresholdImSDA)
            *wMask = 1;
        wImConv++;
        wMask++;
    }




    //ShowImages();
}
//--------------------------------------------------------------------------------------------------
void MainWindow::ShowAll(void)
{
    ShowImages();
    ShowMask();
    ShowSDA();
    ShowResults();
}
//--------------------------------------------------------------------------------------------------
void MainWindow::OnOffImageWindow(void)
{
    //destroyAllWindows();

    if(showInputGray)
        namedWindow("Gray", displayFlag);
    else
        destroyWindow("Gray");

    if(showInputPseudocolor)
        namedWindow("Pseudocolor", displayFlag);
    else
        destroyWindow("Pseudocolor");

    if(showGradient)
        namedWindow("Gradient", displayFlag);
    else
        destroyWindow("Gradient");

    if(showConv)
        namedWindow("SDA", displayFlag);
    else
        destroyWindow("SDA");

    if(showMaskImplant)
        namedWindow("Mask Implant", displayFlag);
    else
        destroyWindow("Mask Implant");

    if(showMask)
        namedWindow("Mask", displayFlag);
    else
        destroyWindow("Mask");
/*
    if(showKernel)
        namedWindow("Mask Implant", displayFlag);
    else
        destroyWindow("Mask Implant");
*/
    if(showSDANormPC)
        namedWindow("SDA", displayFlag);
    else
        destroyWindow("SDA");

    if(showSDANorm)
        namedWindow("SDA norm", displayFlag);
    else
        destroyWindow("SDA norm");

    if(showSDANormPC)
        namedWindow("SDA norm PC", displayFlag);
    else
        destroyWindow("SDA norm PC");


    if(showOutput)
        namedWindow("Output", displayFlag);
    else
        destroyWindow("Output");

    if(showOutputOnSDA)
        namedWindow("Output on SDA", displayFlag);
    else
        destroyWindow("Output on SDA");

    ShowAll();
    //ShowResults();
}
string MainWindow::ParamsToString(void)
{
    string Out = "";

    Out += "Current file name:\t" + CurrentFileName + "\n";
    Out += "File to open:\t" + FileToOpen.string() + "\n";
    Out += "Current directory:\t" + CurrentDir.string() + "\n";
    Out += "Input Directory:\t" + InputDirectory.string() + "\n";


    Out += "max X:\t" + to_string(maxX) + "\n";
    Out += "max Y:\t" + to_string(maxY) + "\n";
    Out += "Threshold input Im :\t" + to_string(thresholdImOrg) + "\n";
    Out += "Threshold gradient Im :\t" + to_string(thresholdGradient) + "\n";

    Out += "Closing element shape :\t" + to_string(closingShape) + "\n";

    Out += "Fil holes :\t" + to_string(fillHoles) + "\n";
    Out += "Divide separate regions :\t" + to_string(divideSeparateRegions) + "\n";


    Out += "Minimum region size :\t" + to_string(minRegionSize) + "\n";

    Out += "Opening element shape :\t" + to_string(erosionShape) + "\n";

    Out += "Expand mask :\t" + to_string(expandMask) + "\n";
    Out += "Expand mask horisontaly :\t" + to_string(expandMaskY) + "\n";
    Out += "Expansion mask size:\t" + to_string(expansionSize) + "\n";

    Out += "crop mask from top :\t" + to_string(croppMask) + "\n";
    Out += "crop mask from top size:\t" + to_string(croppSize) + "\n";

    //Out += "crop mask from top size:\t" + to_string(sdaSize) + "\n";
    //bool calculateSDA;
    Out += "SDA kernel size:\t" + to_string(kernelSizeSDA) + "\n";
    Out += "SDA kernel pix count:\t" + to_string(kernelPixelCountSDA) + "\n";

    Out += "SDA thresholding:\t" + to_string(thresholdSDA) + "\n";
    Out += "SDA threshold val:\t" + to_string(thresholdImSDA) + "\n";

    Out += "Erosion 1 shape:\t" + to_string(postErosionShape1) + "\n";
    Out += "Dilation 2 shape:\t" + to_string(postDilationShape2) + "\n";
    Out += "Erosion 3 shape:\t" + to_string(postErosionShape3) + "\n";

    Out += "Fill holes on out mask:\t" + to_string(fillHolesOnOutMask) + "\n";

    Out += "Divide separate regions on out mask:\t" + to_string(divideSeparateRegionsSDA) + "\n";
    Out += "inimal region size on out mask:\t" + to_string(minRegionSizeSDA) + "\n";

    Out += "\n";

    return Out;

    //int kernelPixelCountSDA;


}
//--------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(ui->CheckBoxAllowResize->checkState())
        displayFlag = WINDOW_NORMAL;
    else
        displayFlag = WINDOW_AUTOSIZE;

    ui->comboBoxSDASize->addItem("SDA in mask only");
    ui->comboBoxSDASize->addItem("SDA in mask + implant");
    ui->comboBoxSDASize->addItem("SDA in whole image");

    sdaSize = 1;
    ui->comboBoxSDASize->setCurrentIndex(sdaSize);

    showInputPseudocolor = ui->CheckBoxShowInputImagePC->checkState();
    showInputGray = ui->CheckBoxShowInputImageGray->checkState();
    showGradient = ui->CheckBoxShowGradient->checkState();
    showMaskImplant = ui->CheckBoxShowMaskImplant->checkState();
    showMask = ui->CheckBoxShowMask->checkState();
    showSDA = ui->CheckBoxShowSDA->checkState();
    showSDANorm = ui->CheckBoxShowSDANorm->checkState();
    showSDANormPC = ui->CheckBoxShowSDANormPC->checkState();
    showSDAThresholded = ui->CheckBoxShowSDAThresholded->checkState();

    showConv = ui->CheckBoxShowConv->checkState();

    minShowGray = ui->spinBoxMinShowGray->value();
    maxShowGray = ui->spinBoxMaxShowGray->value();
    minShowPseudocolor = ui->spinBoxMinShowPseudoColor->value();
    maxShowPseudocolor = ui->spinBoxMaxShowPseudoColor->value();
    minShowGradient = ui->spinBoxMinShowGradient->value();
    maxShowGradient = ui->spinBoxMaxShowGradient->value();
    showContour = ui->CheckBoxContour->checkState();

    minShowSDA = ui->spinBoxMinShowConv->value();
    maxShowSDA = ui->spinBoxMaxShowConv->value();

    minFlood = ui->spinBoxMinFlood->value();
    maxFlood = ui->spinBoxMaxFlood->value();


    thresholdImOrg = ui->spinBoxThresholdOryginalImage->value();
    thresholdGradient = ui->spinBoxThresholdGradient->value();

    fillHoles = ui->CheckBoxFillHoles->checkState();
    divideSeparateRegions = ui->CheckBoxDivideseparateregions->checkState();
    minRegionSize = ui->spinBoxMinRegionSize->value();

    expandMask = ui->CheckBoxExpandMask->checkState();
    expandMaskY = ui->CheckBoxExpandMaskY->checkState();
    expansionSize = ui->spinBoxExpansionSize->value();

    croppMask = ui->CheckBoxCroppMask->checkState();
    croppSize = ui->spinBoxCroppSize->value();

    calculateSDA = ui->CheckBoxCalculateSDA->checkState();
    kernelSizeSDA = ui->spinBoxSDAKernelSize->value();

    thresholdSDA = ui->CheckBoxThresholdSDA->checkState();
    thresholdImSDA  = ui->spinBoxThresholdSDA->value();

    showOutput = ui->CheckBoxShowOutput->checkState();
    showOutputOnSDA = ui->CheckBoxShowOutputOnSDA->checkState();

    transparency = ui->spinBoxTransparency->value();

    closingShape = ui->spinBoxClosingShape->value();
    erosionShape = ui->spinBoxErosionShape->value();
    postErosionShape1 = ui->spinBoxErosionShape1->value();
    postDilationShape2 = ui->spinBoxDilationShape2->value();
    postErosionShape3 = ui->spinBoxErosionShape3->value();

    fillHolesOnOutMask = ui->CheckBoxFillHolesOnOut->checkState();
    findRegTouchingImplant = ui->checkBoxFindRegTouchingImplant->checkState();


    divideSeparateRegionsSDA = ui->CheckBoxDivideseparateregionsSDA->checkState();
    minRegionSizeSDA = ui->spinBoxMinRegionSizeSDA->value();

    FilePatternStr = ui->lineEditFilePattern->text().toStdString();

    string HeaderString =  "File\t\t";
    HeaderString +=  "SDA d\t";
    HeaderString +=  "SDA #\t";
    HeaderString +=  "SDA TH\t";
    HeaderString +=  "Jack\t";
    HeaderString +=  "mask #\t";
    HeaderString +=  "ref #\t";

    ui->lineEditHeader->setText(HeaderString.c_str());

    stopDisplay = 0;

    OnOffImageWindow();
}

void MainWindow::RefreshFileList()
{
    //InputDirectory = dialog.getExistingDirectory().toStdWString();//  toStdString());
    if (!exists(InputDirectory))
    {
        QMessageBox msgBox;
        msgBox.setText((InputDirectory.string()+ " not exists ").c_str());
        msgBox.exec();
        InputDirectory = "c:\\";
    }
    if (!is_directory(InputDirectory))
    {
        QMessageBox msgBox;
        msgBox.setText((InputDirectory.string()+ " This is not a directory path ").c_str());
        msgBox.exec();
        InputDirectory = "c:\\";
    }
    //Files.clear();
    ui->LineEditDirectory->setText(QString::fromWCharArray(InputDirectory.wstring().c_str()));
    ui->ListWidgetFiles->clear();

    regex FilePattern(FilePatternStr);
    for (directory_entry& FileToProcess : directory_iterator(InputDirectory))
    {
        if (FileToProcess.path().extension() != ".tif" && FileToProcess.path().extension() != ".png" )
            continue;
        path PathLocal = FileToProcess.path();


        if ((!regex_match(FileToProcess.path().filename().string().c_str(), FilePattern )))
            continue;

        if (!exists(PathLocal))
        {
            //Files << PathLocal.filename().string() << " File not exists" << "\n";
            QMessageBox msgBox;
            msgBox.setText((PathLocal.filename().string() + " File not exists" ).c_str());
            msgBox.exec();
            break;
        }
        ui->ListWidgetFiles->addItem(PathLocal.filename().string().c_str());
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonSelectInFolder_clicked()
{
    QFileDialog dialog(this, "Open Folder");
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setDirectory(InputDirectory.string().c_str());

    //QStringList FileList= dialog.e
    if(dialog.exec())
    {
        InputDirectory = dialog.directory().path().toStdWString();
    }
    else
         return;

    RefreshFileList();
}

void MainWindow::on_ListWidgetFiles_currentTextChanged(const QString &currentText)
{
    CurrentFileName = currentText.toStdString();
    string FileName(CurrentFileName);

    FileToOpen = InputDirectory;
    FileToOpen.append(FileName);

    ImIn.release();
    ImConv.release();
    ImSDA.release();
    ImNormInvSDA.release();
    ImGradient.release();
    MaskImplant.release();
    Mask.release();

    //MaskSDARef;

    MaskSDA.release();
    ImOut.release();

    ImShowGray.release();
    ImShowPseudocolor.release();
    ImShowSDA.release();

    OpenImage();
}



void MainWindow::on_spinBoxMinShowGray_valueChanged(int arg1)
{
    minShowGray = arg1;
    ShowImages();
}

void MainWindow::on_spinBoxMaxShowGray_valueChanged(int arg1)
{
    maxShowGray = arg1;
    ShowImages();
}

void MainWindow::on_CheckBoxShowInputImageGray_toggled(bool checked)
{
    showInputGray = checked;
    if(showInputGray)
        namedWindow("Gray", displayFlag);
    else
        destroyWindow("Gray");
    ShowImages();
}

void MainWindow::on_CheckBoxShowInputImagePC_toggled(bool checked)
{
    showInputPseudocolor = checked;
    if(showInputPseudocolor)
        namedWindow("Pseudocolor", displayFlag);
    else
        destroyWindow("Pseudocolor");
    ShowImages();
}

void MainWindow::on_spinBoxMinShowPseudoColor_valueChanged(int arg1)
{
    minShowPseudocolor = arg1;
    ShowImages();
}

void MainWindow::on_spinBoxMaxShowPseudoColor_valueChanged(int arg1)
{
    maxShowPseudocolor = arg1;
    ShowImages();
}

void MainWindow::on_spinBoxMinShowGradient_valueChanged(int arg1)
{
    minShowGradient = arg1;
    ShowImages();
}

void MainWindow::on_spinBoxMaxShowGradient_valueChanged(int arg1)
{
    maxShowGradient = arg1;
    ShowImages();
}

void MainWindow::on_CheckBoxShowGradient_toggled(bool checked)
{
    showGradient = checked;
    if(showGradient)
        namedWindow("Gradient", displayFlag);
    else
        destroyWindow("Gradient");
    ShowImages();
}



void MainWindow::on_spinBoxThresholdOryginalImage_valueChanged(int arg1)
{
    thresholdImOrg = (unsigned short)arg1;
    ui->CheckBoxCalculateSDA->setChecked(false);
    MaskImage();

    //ProcessImage();
}

void MainWindow::on_CheckBoxShowMask_toggled(bool checked)
{
    showMask = checked;
    if(showMask)
        namedWindow("Mask", displayFlag);
    else
        destroyWindow("Mask");
    ShowMask();
}

void MainWindow::on_CheckBoxShowConv_toggled(bool checked)
{
    showConv = checked;
    OnOffImageWindow();
    ShowImages();
}

void MainWindow::on_spinBoxMinShowConv_valueChanged(int arg1)
{
    minShowSDA = arg1;
    ShowSDA();
    ShowResults();
}

void MainWindow::on_spinBoxMaxShowConv_valueChanged(int arg1)
{
    maxShowSDA = arg1;
    ShowSDA();
    ShowResults();
}

void MainWindow::on_spinBoxMinFlood_valueChanged(int arg1)
{
    minFlood = arg1;
    MaskImage();
}

void MainWindow::on_spinBoxMaxFlood_valueChanged(int arg1)
{
    maxFlood = arg1;
    MaskImage();
}

void MainWindow::on_CheckBoxAllowResize_toggled(bool checked)
{
    if(ui->CheckBoxAllowResize->checkState())
        displayFlag = WINDOW_NORMAL;
    else
        displayFlag = WINDOW_AUTOSIZE;

    OnOffImageWindow();
    //ShowAll();

}

void MainWindow::on_spinBoxThresholdGradient_valueChanged(int arg1)
{
    thresholdGradient = (unsigned short)arg1;
    ui->CheckBoxCalculateSDA->setChecked(false);
    MaskImage();
}

void MainWindow::on_CheckBoxFillHoles_toggled(bool checked)
{
    fillHoles = checked;
    ui->CheckBoxCalculateSDA->setChecked(false);
    MaskImage();
}

void MainWindow::on_CheckBoxDivideseparateregions_toggled(bool checked)
{
    divideSeparateRegions = checked;
    ui->CheckBoxCalculateSDA->setChecked(false);
    MaskImage();
}

void MainWindow::on_spinBoxMinRegionSize_valueChanged(int arg1)
{
    minRegionSize = arg1;
    ui->CheckBoxCalculateSDA->setChecked(false);
    MaskImage();
}

void MainWindow::on_CheckBoxExpandMask_toggled(bool checked)
{
    expandMask = checked;
    ui->CheckBoxCalculateSDA->setChecked(false);
    MaskImage();
}

void MainWindow::on_spinBoxExpansionSize_valueChanged(int arg1)
{
    expansionSize = arg1;
    ui->CheckBoxCalculateSDA->setChecked(false);
    MaskImage();
}

void MainWindow::on_CheckBoxCroppMask_toggled(bool checked)
{
    croppMask = checked;
    ui->CheckBoxCalculateSDA->setChecked(false);
    MaskImage();
}

void MainWindow::on_spinBoxCroppSize_valueChanged(int arg1)
{
    croppSize = arg1;
    ui->CheckBoxCalculateSDA->setChecked(false);
    MaskImage();
}

void MainWindow::on_CheckBoxThresholdSDA_toggled(bool checked)
{
    thresholdSDA = checked;
    //MaskImage();
}

void MainWindow::on_spinBoxThresholdSDA_valueChanged(int arg1)
{
    thresholdImSDA = arg1;
    PostSDA();

}

void MainWindow::on_CheckBoxCalculateSDA_toggled(bool checked)
{
    calculateSDA = checked;
    //CalculateSDA();
    EstymateSDA();
}

void MainWindow::on_spinBoxSDAKernelSize_valueChanged(int arg1)
{
    ui->CheckBoxCalculateSDA->setChecked(false);
    kernelSizeSDA = arg1;
}

void MainWindow::on_CheckBoxShowOutput_toggled(bool checked)
{
    showOutput = checked;
    if(showOutput)
        namedWindow("Output", displayFlag);
    else
        destroyWindow("Output");

    if(showOutputOnSDA)
        namedWindow("Output on SDA", displayFlag);
    else
        destroyWindow("Output on SDA");
    ShowResults();
    //ShowImages();
}

void MainWindow::on_spinBoxTransparency_valueChanged(int arg1)
{
    transparency = arg1;
    //ShowImages();
}

void MainWindow::on_pushButtonSaveOut_clicked()
{
    string FileNameOut(CurrentFileName);

    FileNameOut = regex_replace(FileNameOut,regex("_crop.tif"),"_outSdaR" + ItoStrLZ(kernelSizeSDA,2) + ".bmp");
    path FileToSaveOut = InputDirectory;
    FileToSaveOut.append(FileNameOut);
    imwrite(FileToSaveOut.string(),ImShowOutput);//ImConv = imread(FileToOpen2.string().c_str(),CV_LOAD_IMAGE_ANYDEPTH);

    string FileNameOutOnSDA(CurrentFileName);

    FileNameOutOnSDA = regex_replace(FileNameOutOnSDA,regex("_crop.tif"),"_outOnSDSdaR" + ItoStrLZ(kernelSizeSDA,2) + ".bmp");
    path FileToSaveOutOnSDA = InputDirectory;
    FileToSaveOutOnSDA.append(FileNameOutOnSDA);
    imwrite(FileToSaveOutOnSDA.string(),ImShowOutputOnSDA);//ImConv = imread(FileToOpen2.string().c_str(),CV_LOAD_IMAGE_ANYDEPTH);

    string FileNameMask(CurrentFileName);

    FileNameMask = regex_replace(FileNameMask,regex("_crop.tif"),"_maskSdaR" + ItoStrLZ(kernelSizeSDA,2) + ".bmp");
    path FileToSaveMask = InputDirectory;
    FileToSaveMask.append(FileNameMask);
    imwrite(FileToSaveMask.string(),MaskSDA*255);//ImConv = imread(FileToOpen2.string().c_str(),CV_LOAD_IMAGE_ANYDEPTH);

    string FileNameSDA(CurrentFileName);

    FileNameSDA = regex_replace(FileNameSDA,regex("_crop.tif"),"_SDAR" + ItoStrLZ(kernelSizeSDA,2) + ".bmp");
    path FileToSaveSDA = InputDirectory;
    FileToSaveSDA.append(FileNameSDA);
    imwrite(FileToSaveSDA.string(),ImShowSDA);//ImConv = imread(FileToOpen2.string().c_str(),CV_LOAD_IMAGE_ANYDEPTH);

}

void MainWindow::on_CheckBoxContour_toggled(bool checked)
{
    showContour = checked;
    ui->CheckBoxCalculateSDA->setChecked(false);
    ShowAll();
    ShowResults();
}

void MainWindow::on_spinBoxClosingShape_valueChanged(int arg1)
{
    closingShape = arg1;
    ui->CheckBoxCalculateSDA->setChecked(false);
    MaskImage();
}

void MainWindow::on_spinBoxErosionShape_valueChanged(int arg1)
{
    erosionShape = arg1;
    ui->CheckBoxCalculateSDA->setChecked(false);
    MaskImage();
}

void MainWindow::on_spinBoxErosionShape1_valueChanged(int arg1)
{
    postErosionShape1 = arg1;
    PostSDA();
    //MaskImage();
}

void MainWindow::on_spinBoxDilationShape2_valueChanged(int arg1)
{
    postDilationShape2 = arg1;
    PostSDA();
    //MaskImage();
}

void MainWindow::on_spinBoxErosionShape3_valueChanged(int arg1)
{
    postErosionShape3 = arg1;
    PostSDA();
    //MaskImage();
}

void MainWindow::on_CheckBoxFillHolesOnOut_toggled(bool checked)
{
    fillHolesOnOutMask = checked;
    PostSDA();
    //MaskImage();
}



void MainWindow::on_CheckBoxExpandMaskY_toggled(bool checked)
{
    expandMaskY = checked;
    ui->CheckBoxCalculateSDA->setChecked(false);
    MaskImage();
}

void MainWindow::on_CheckBoxShowMaskImplant_toggled(bool checked)
{
    showMaskImplant = checked;
    ui->CheckBoxCalculateSDA->setChecked(false);
    if(showMaskImplant)
        namedWindow("Mask Implant", displayFlag);
    else
        destroyWindow("Mask Implant");
    ShowMask();
}

void MainWindow::on_CheckBoxShowSDA_toggled(bool checked)
{
    showSDA = checked;
    if(showSDA)
        namedWindow("SDA", displayFlag);
    else
        destroyWindow("SDA");
    ShowSDA();
    //MaskImage();
}

void MainWindow::on_CheckBoxShowSDAThresholded_toggled(bool checked)
{
    showSDAThresholded = checked;
    //MaskImage();
}



void MainWindow::on_CheckBoxDivideseparateregionsSDA_toggled(bool checked)
{
    divideSeparateRegionsSDA = checked;
    PostSDA();
}

void MainWindow::on_spinBoxMinRegionSizeSDA_valueChanged(int arg1)
{
    minRegionSizeSDA = arg1;
    PostSDA();
}

void MainWindow::on_comboBoxSDASize_currentIndexChanged(int index)
{
    sdaSize = index;
    //ui->CheckBoxCalculateSDA->setChecked(false);
    EstymateSDA();
}

void MainWindow::on_CheckBoxShowSDANormPC_toggled(bool checked)
{
    showSDANormPC = checked;
    if(showSDANormPC)
         namedWindow("SDA norm PC", displayFlag);
     else
         destroyWindow("SDA norm PC");
    ShowSDA();

}

void MainWindow::on_CheckBoxShowSDANorm_toggled(bool checked)
{
   showSDANorm = checked;
   if(showSDANorm)
       namedWindow("SDA norm", displayFlag);
   else
       destroyWindow("SDA norm");
   ShowSDA();
}



void MainWindow::on_pushButtonStoreLocalToOut_clicked()
{
    OutString += "\n" + LocalString;
    ui->textEditOutFile->setText(OutString.c_str());
}

void MainWindow::on_pushButtonSaveRef_clicked()
{
    MaskSDA.copyTo(MaskSDARef);
    kernelSizeSDARef = kernelSizeSDA;
}

void MainWindow::on_pushButtonClearOut_clicked()
{
    OutString = "";
    OutJaccard ="";
    OutString = "";
    ui->textEditOutFile->setText(OutString.c_str());
}

void MainWindow::on_pushButtonFindOptimalTheshold_clicked()
{
    MaskSDARef.release();
    MaskSDA.copyTo(MaskSDARef);
    kernelSizeSDARef = kernelSizeSDA;

    string paramsStr = ParamsToString();

    OutJaccard = "";
    OutThreshold = "";
    OutString = "";
    ui->textEditOutFile->setText(OutString.c_str());

    for (int radius = 50; radius >=30; radius--)
    {

        OutJaccard += to_string(radius) + "\t";
        OutThreshold += to_string(radius) + "\t";
        kernelSizeSDA = radius;
        EstymateSDA();
        double jaccardMax  = 0.0;
        int jaccardMaxThreshold = 0;
        stopDisplay = true;
        int thresholdStop = 0;
        for(int threshold = kernelPixelCountSDA - 1; threshold  >= 0; threshold-= 20)
        {
            thresholdImSDA = threshold;
            PostSDA();
            if(jaccard > 0.5)
            {
                thresholdStop = threshold;
                break;
            }
        }
        for(int threshold = thresholdStop; threshold  > 0; threshold--)
        {
            thresholdImSDA = threshold;
            PostSDA();
            if(jaccardMax < jaccard)
            {
                jaccardMax = jaccard;
                jaccardMaxThreshold = threshold;
            }
            if(jaccard < 0.4)
                break;
            OutJaccard += to_string(jaccard) + "\t";
            OutThreshold += to_string(threshold) + "\t";
        }
        OutJaccard += "\n";
        OutThreshold += "\n";
        stopDisplay = false;
        ui->spinBoxThresholdSDA->setValue(jaccardMaxThreshold);
        thresholdImSDA = jaccardMaxThreshold;
        PostSDA();
        OutString += "\n" + LocalString;
        ui->textEditOutFile->append(LocalString.c_str());
        waitKey(80);
    }
    string FileNameJacc(CurrentFileName);
    //FileNameJacc = regex_replace(FileNameJacc,regex("_crop.tif"),"_Jacc.txt");
    path FileToSaveJacc = InputDirectory;
    FileToSaveJacc.append(FileNameJacc +"_JaccL.txt");
    std::ofstream outJ(FileToSaveJacc.string());
    outJ << OutJaccard << "\n\n" << paramsStr;
    outJ.close();

    string FileNameThr(CurrentFileName);
    //FileNameThr = regex_replace(FileNameThr,regex("_crop.tif"),"_Thr.txt");
    path FileToSaveThr = InputDirectory;
    FileToSaveThr.append(FileNameThr+"_ThrL.txt");
    std::ofstream outT(FileToSaveThr.string());
    outT << OutThreshold << "\n\n" << paramsStr;
    outT.close();

    string FileNameOut(CurrentFileName);
    //FileNameOut = regex_replace(FileNameOut,regex("_crop.tif"),"_Out.txt");
    path FileToSaveOut = InputDirectory;
    FileToSaveOut.append(FileNameOut+"_OutL.txt");
    std::ofstream out(FileToSaveOut.string());
    out << OutString << "\n\n" << paramsStr;
    out.close();


}

void MainWindow::on_pushButtonDataFor2dPlot_clicked()
{
    MaskSDARef.release();
    MaskSDA.copyTo(MaskSDARef);
    kernelSizeSDARef = kernelSizeSDA;

    string paramsStr = ParamsToString();

    OutJaccard = "";
    OutThreshold = "";
    OutString = "";
    ui->textEditOutFile->setText(OutString.c_str());

    for (int radius = 50; radius >=30; radius--)
    {

        OutJaccard += to_string(radius) + "\t";
        OutThreshold += to_string(radius) + "\t";
        kernelSizeSDA = radius;
        EstymateSDA();
        double jaccardMax  = 0.0;
        int jaccardMaxThreshold = 0;
        stopDisplay = true;

        int thresholdStart = 6500;
        int thresholdStep = 5;
        for(int threshold = thresholdStart; threshold  > kernelPixelCountSDA; threshold -= thresholdStep)
        {
            OutJaccard += to_string(0) + "\t";
            OutThreshold += to_string(threshold) + "\t";
            thresholdStart = threshold - thresholdStep;
        }

        for(int threshold = thresholdStart; threshold  > 1200; threshold-= thresholdStep)
        {
            thresholdImSDA = threshold;
            PostSDA();
            if(jaccardMax < jaccard)
            {
                jaccardMax = jaccard;
                jaccardMaxThreshold = threshold;
            }

            OutJaccard += to_string(jaccard) + "\t";
            OutThreshold += to_string(threshold) + "\t";
        }
        OutJaccard += "\n";
        OutThreshold += "\n";
        stopDisplay = false;
        ui->spinBoxThresholdSDA->setValue(jaccardMaxThreshold);
        thresholdImSDA = jaccardMaxThreshold;
        PostSDA();
        OutString += "\n" + LocalString;
        ui->textEditOutFile->append(LocalString.c_str());
        waitKey(80);
    }
    string FileNameJacc(CurrentFileName);
    //FileNameJacc = regex_replace(FileNameJacc,regex("_crop.tif"),"_Jacc.txt");
    path FileToSaveJacc = InputDirectory;
    FileToSaveJacc.append(FileNameJacc +"_Jacc.txt");
    std::ofstream outJ(FileToSaveJacc.string());
    outJ << OutJaccard << "\n\n" << paramsStr;
    outJ.close();

    string FileNameThr(CurrentFileName);
    //FileNameThr = regex_replace(FileNameThr,regex("_crop.tif"),"_Thr.txt");
    path FileToSaveThr = InputDirectory;
    FileToSaveThr.append(FileNameThr+"_Thr.txt");
    std::ofstream outT(FileToSaveThr.string());
    outT << OutThreshold << "\n\n" << paramsStr;
    outT.close();

    string FileNameOut(CurrentFileName);
    //FileNameOut = regex_replace(FileNameOut,regex("_crop.tif"),"_Out.txt");
    path FileToSaveOut = InputDirectory;
    FileToSaveOut.append(FileNameOut+"_Out.txt");
    std::ofstream out(FileToSaveOut.string());
    out << OutString << "\n\n" << paramsStr;
    out.close();
}

void MainWindow::on_lineEditFilePattern_returnPressed()
{
    FilePatternStr = ui->lineEditFilePattern->text().toStdString();
     RefreshFileList();
}

void MainWindow::on_checkBoxFindRegTouchingImplant_toggled(bool checked)
{
    findRegTouchingImplant = checked;
    PostSDA();
}
