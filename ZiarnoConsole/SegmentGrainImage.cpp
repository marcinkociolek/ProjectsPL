//#include <iostream>
//#include <string>
#include <vector>

//#include <chrono>

#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//#include "tiffio.h"

#include "gradient.h"
//#include "DispLib.h"

#include "mazdaroi.h"
#include "mazdaroiio.h"

#include "SegmentGrainImage.h"

typedef MazdaRoi<unsigned int, 2> MR2DType;

//QFileSystemModel *model;
//QAbstractItemModel *model;


using namespace std;
//using namespace boost::filesystem;
//using namespace boost;
using namespace cv;

//---------------------------------------------------------------------------------------------
//              funtions
//--------------------------------------------------------------------------------------------------
Mat Combine2Images( Mat Im1, Mat Im2)
{
    int imMaxX, imMaxY;
    imMaxX = Im1.cols;
    imMaxY = Im1.rows;
    Mat Im = Mat::zeros(Size(imMaxX *2,imMaxY) ,Im1.type());
    Im1.copyTo(Im(Rect(0, 0, imMaxX, imMaxY)));
    Im2.copyTo(Im(Rect(imMaxX, 0, imMaxX, imMaxY)));

    return Im;
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


    Mat ImMask = GradientThresh(ImGradient,(float)thesholdVal);

    return ImMask;
}
//---------------------------------------------------------------------------------------------
float AverageMaskedPixelsF(Mat Reg, Mat ImF)
{
    int maxX = ImF.cols;
    int maxY = ImF.rows;
    int maxXY = maxX * maxY;
    if(!ImF.isContinuous())
        return -1;

    unsigned short *wReg = (unsigned short *)Reg.data;
    float *wImF = (float *)ImF.data;

    //deph


    float sumIm = 0;
    float sumReg = 0;
    for (int i = 0; i < maxXY; i++)
    {
            if(*wReg)
            {
                sumIm  += *wImF;
                sumReg += 1.0;
            }
            wReg++;
            wImF++;
    }
    if(!sumReg)
        sumReg = 1;

    return sumIm/sumReg;
}
//---------------------------------------------------------------------------------------------
vector <MR2DType*> SegmentGrainImgXX(Mat *ImIn)
{
    // patrameters could be made input or external
    int threshVal = 26;
    bool removeSmallReg = true;
	int rawMorphErosion1 = 0;
	int rawMorphDilation2 = 0;
	int rawMorphErosion3 = 0;
	bool fillHoles = true;
	bool divideSeparateReg = true;
	int MinRegSize = 10000;
	int regMorphErosion1 = 0;
	int regMorphDilation2 = 0;
	int regMorphErosion3 = 0;
	bool removeBorderRegion = false;
	bool alignGrains = true;
	bool findValey = true;
	
    vector <MR2DType*> rois;
	
 //   Mat ImIn;
 //   ImIn = ImIpl;
    if (ImIn->empty())
        return rois;
    int maxX = ImIn->cols/2;
    int maxY = ImIn->rows;
    Mat ImIn1, ImIn2;

    (*ImIn)(Rect(0, 0, maxX, maxY)).copyTo(ImIn1);
    (*ImIn)(Rect(maxX, 0, maxX, maxY)).copyTo(ImIn2);

    Mat Mask1;
    Mat Mask2;

    // thresholding
    Mask1 = FindMaskFromGray(ImIn1,threshVal);
    Mask2 = FindMaskFromGray(ImIn2,threshVal);
 
 
    // remove regions of size 1 pix
    if(removeSmallReg)
    {
        Mask1 = RemovingTinyReg9(Mask1);
        Mask2 = RemovingTinyReg9(Mask2);
    }

    // morphology on thresholded image
    switch (rawMorphErosion1)
    {
    case 1:
        RegionErosion5(Mask1);
        RegionErosion5(Mask2);
        break;
    case 2:
        RegionErosion9(Mask1);
        RegionErosion9(Mask2);
        break;
    case 3:
        RegionErosion13(Mask1);
        RegionErosion13(Mask2);
        break;
    default:
        break;
    }

    switch (rawMorphDilation2)
    {
    case 1:
        RegionDilation5(Mask1);
        RegionDilation5(Mask2);
        break;
    case 2:
        RegionDilation9(Mask1);
        RegionDilation9(Mask2);
        break;
    case 3:
        RegionDilation13(Mask1);
        RegionDilation13(Mask2);
        break;
    default:
        break;
    }

    switch (rawMorphErosion3)
    {
    case 1:
        RegionErosion5(Mask1);
        RegionErosion5(Mask2);
        break;
    case 2:
        RegionErosion9(Mask1);
        RegionErosion9(Mask2);
        break;
    case 3:
        RegionErosion13(Mask1);
        RegionErosion13(Mask2);
        break;
    default:
        break;
    }

    // procedure for removing choles inside object, obligatory for gradient thesholded image, usefull in other case
    if(fillHoles)
    {
        FillBorderWithValue(Mask1, 0xFFFF);
        OneRegionFill5Fast1(Mask1,  0xFFFF);
        FillHoles(Mask1);
        DeleteRegionFromImage(Mask1, 0xFFFF);

        FillBorderWithValue(Mask2, 0xFFFF);
        OneRegionFill5Fast1(Mask2,  0xFFFF);
        FillHoles(Mask2);
        DeleteRegionFromImage(Mask2, 0xFFFF);
    }

    // labelling
    if(divideSeparateReg)
    {
        DivideSeparateRegions(Mask1, MinRegSize);
        DivideSeparateRegions(Mask2, MinRegSize);
    }

    //morphology after labelling
    switch (regMorphErosion1)
    {
    case 1:
        RegionErosion5(Mask1);
        RegionErosion5(Mask2);
        break;
    case 2:
        RegionErosion9(Mask1);
        RegionErosion9(Mask2);
        break;
    case 3:
        RegionErosion13(Mask1);
        RegionErosion13(Mask2);
        break;
    default:
        break;
    }

    switch (regMorphDilation2)
    {
    case 1:
        RegionDilation5(Mask1);
        RegionDilation5(Mask2);
        break;
    case 2:
        RegionDilation9(Mask1);
        RegionDilation9(Mask2);
        break;
    case 3:
        RegionDilation13(Mask1);
        RegionDilation13(Mask2);
        break;
    default:
        break;
    }

    switch (regMorphErosion3)
    {
    case 1:
        RegionErosion5(Mask1);
        RegionErosion5(Mask2);
        break;
    case 2:
        RegionErosion9(Mask1);
        RegionErosion9(Mask2);
        break;
    case 3:
        RegionErosion13(Mask1);
        RegionErosion13(Mask2);
        break;
    default:
        break;
    }

    // removal of regions touchig image border
    if(removeBorderRegion)
    {
        DeleteRegTouchingBorder(Mask1);
        DeleteRegTouchingBorder(Mask2);
    }

	// allign Grains
    int maxPosY = 0;
    int minPosY = maxY-1;
    int maxPosX = 0;
    int minPosX = maxX-1;
    unsigned short *wMask1;

    int maxXY = maxX*maxY;
    if(alignGrains)
    {
        wMask1 = (unsigned short*)Mask1.data;
        for(int i = 0; i < maxXY; i++)
        {
            int y = i/maxX;
            int x = i%maxX;

            if(*wMask1)
            {
                if(maxPosY < y)
                    maxPosY = y;
                if(minPosY > y)
                    minPosY = y;
                if(maxPosX < x)
                    maxPosX = x;
                if(minPosX > x)
                    minPosX = x;
            }

            wMask1++;
        }

    }

    int linesToCount = 200;
    int pixelCount = linesToCount * maxX;

    int uStartLine = minPosY;
    if(uStartLine <0)
        uStartLine = 0;
    int uStopLine =  uStartLine + linesToCount;
    if(uStopLine >= maxY)
    {
        uStopLine = maxY - 1;
        uStartLine = uStopLine - linesToCount;
    }
    int lStopLine  =  maxPosY;
    if(lStopLine >= maxY)
        lStopLine = maxY - 1;
    int lStartLine = lStopLine - linesToCount;
    if(lStartLine < 0)
    {
        lStartLine = 0;
        lStopLine = lStartLine + linesToCount;
    }

    if(alignGrains)
    {
        wMask1 = (unsigned short*)Mask1.data + maxX * uStartLine;//(imCenterY - linesToCount - offsetToCount);
        int uRegPixelCount = 0;
        for(int i = 0; i < pixelCount; i++)
        {
            if(*wMask1)
                uRegPixelCount++;

            wMask1++;
        }

        wMask1 = (unsigned short*)Mask1.data + maxX * lStartLine;//(imCenterY + offsetToCount);
        int lRegPixelCount = 0;
        for(int i = 0; i< pixelCount; i++)
        {
            if(*wMask1)
                lRegPixelCount++;

            wMask1++;
        }


        if(uRegPixelCount >= lRegPixelCount)
        {
            flip(ImIn1,ImIn1, -1);
            flip(ImIn2,ImIn2, -1);
            flip(Mask1,Mask1, -1);
            flip(Mask2,Mask2, -1);
        }
    }


    int rectHeight = 300;
    int rectWidth = 100;
//    int rectLCX = ImIn1.cols/2 - rectWidth/2;
//    int rectLCY = ImIn1.rows/2 - rectHeight/2;

    Mat Mask1a = Mat::zeros(maxY,maxX,Mask1.type());
    Mat Mask2a = Mat::zeros(maxY,maxX,Mask1.type());

	
    int ellipseSizeX = (maxPosX-minPosX)/4;
    int ellipseSizeY = (maxPosY-minPosY)/4;

    if(ellipseSizeX < 0)
        ellipseSizeX = 0;
    if(ellipseSizeY < 0)
        ellipseSizeY = 0;


    ellipse(Mask1a, Point(maxX/2,maxY/2),Size(ellipseSizeX,ellipseSizeY),0,0,360,1,-1);
    ellipse(Mask2a, Point(maxX/2,maxY/2),Size(ellipseSizeX,ellipseSizeY),0,0,360,1,-1);

    // find valey(znajdź bruzdke)
    Mat ImGray1, ImGray2;
    cvtColor(ImIn1,ImGray1,CV_BGR2GRAY);
    cvtColor(ImIn2,ImGray2,CV_BGR2GRAY);

    blur(ImGray2,ImGray2,Size(5,5));

    Mat ImGradient1 = HorizontalGradientDown(ImGray1);
    Mat ImGradient2 = HorizontalGradientDown(ImGray2);

    //Mat ImShowGradient = Combine2Images(ShowImageF32PseudoColor(ImGradient1, 0.0, 100.0), ShowImageF32PseudoColor(ImGradient2, 0.0, 100.0));

    bool switchImages;

    if(findValey)
    {

        float sum1 = AverageMaskedPixelsF(Mask1a, ImGradient1);
        float sum2 = AverageMaskedPixelsF(Mask2a, ImGradient2);


        if(sum1 < sum2)
        {
            Mask1 = Mask1 * 2;
            switchImages = 1;
        }
        else
        {
            Mask2 = Mask2 * 2;
            switchImages = 0;
        }
    }
    if(switchImages)
    {
        Mat TempMat;
        TempMat = Mask1;
        Mask1 = Mask2;
        Mask2 = TempMat;

        TempMat = ImIn1;
        ImIn1 = ImIn2;
        ImIn2 = TempMat;
    }

// output ROI
    //vector <MR2DType*> rois;
    int begin[MR2DType::Dimensions];
    int end[MR2DType::Dimensions];
    begin[0] = 0;
    begin[1] = 0;
    end[0] = maxX*2 -1;
    end[1] = maxY-1;
    unsigned short *wMask;

    Mat Mask = Combine2Images(Mask1, Mask2);

    //Bruzdka
    MR2DType* roi1;
    roi1 = new MR2DType(begin, end);
    MazdaRoiIterator<MR2DType> iterator1(roi1);
    wMask = (unsigned short*)Mask.data;
    while(! iterator1.IsBehind())
    {
        if (*wMask == 1)
            iterator1.SetPixel();
        ++iterator1;
       wMask++;
    }
    roi1->SetName("Bruzdka");
    roi1->SetColor(0xff0000);
    rois.push_back(roi1);

    //Grzbiet
    MR2DType* roi2;
    roi2 = new MR2DType(begin, end);
    MazdaRoiIterator<MR2DType> iterator2(roi2);
    wMask = (unsigned short*)Mask.data;
    while(! iterator2.IsBehind())
    {
        if (*wMask ==2)
            iterator2.SetPixel();
        ++iterator2;
       wMask++;
    }
    roi2->SetName("Grzbiet");
    roi2->SetColor(0x00ff00);
    rois.push_back(roi2);

/*
// output ROI
    //vector <MR2DType*> rois;
    int begin[MR2DType::Dimensions];
    int end[MR2DType::Dimensions];
    begin[0] = 0;
    begin[1] = 0;
    end[0] = maxX-1;
    end[1] = maxY-1;
    unsigned short *wMask;

    //Bruzdka
    MR2DType* roi1;
    roi1 = new MR2DType(begin, end);
    MazdaRoiIterator<MR2DType> iterator1(roi1);
    wMask = (unsigned short*)Mask1.data;
    while(! iterator1.IsBehind())
    {
        if (*wMask)
            iterator1.SetPixel();
        ++iterator1;
       wMask++;
    }
    roi1->SetName("Bruzdka");
    roi1->SetColor(0xff0000);
    rois.push_back(roi1);

    //Grzbiet
    MR2DType* roi2;
    roi2 = new MR2DType(begin, end);
    MazdaRoiIterator<MR2DType> iterator2(roi2);
    wMask = (unsigned short*)Mask2.data;
    while(! iterator2.IsBehind())
    {
        if (*wMask)
            iterator2.SetPixel();
        ++iterator2;
       wMask++;
    }
    roi2->SetName("Grzbiet");
    roi2->SetColor(0x00ff00);
    rois.push_back(roi2);
*/


    ImIn->release();
    *ImIn = Combine2Images(ImIn1, ImIn2);


    Mask1.release();
    Mask2.release();
    Mask1a.release();
    Mask2a.release();

    ImIn1.release();
    ImIn2.release();
    ImGradient1.release();
    ImGradient2.release();
    ImGray1.release();
    ImGray1.release();

    return rois;

}
//--------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
bool SegmentGrainImg(const std::vector<Mat*> *ImInVect, std::vector<Mat*> *ImOutVect, vector <MR2DType*> * outRoi,std::vector<TransformacjaZiarna> *transf)
{
    // patrameters could be made input or external
    int threshVal = 26;
    bool removeSmallReg = true;
    int rawMorphErosion1 = 0;
    int rawMorphDilation2 = 0;
    int rawMorphErosion3 = 0;
    bool fillHoles = true;
    bool divideSeparateReg = true;
    int MinRegSize = 10000;
    int regMorphErosion1 = 0;
    int regMorphDilation2 = 0;
    int regMorphErosion3 = 0;
    bool removeBorderRegion = false;
    bool alignGrains = true;
    bool findValey = true;

    //vector <MR2DType*> rois;

 //   Mat ImIn;
 //   ImIn = ImIpl;
//  if (ImIn->empty())
//        return rois;
//    int maxX = ImIn->cols/2;
//    int maxY = ImIn->rows;
    Mat ImIn1, ImIn2;

//    (*ImIn)(Rect(0, 0, maxX, maxY)).copyTo(ImIn1);
//    (*ImIn)(Rect(maxX, 0, maxX, maxY)).copyTo(ImIn2);

    ImIn1 = *ImInVect->at(0);
    ImIn2 = *ImInVect->at(1);

    int maxX = ImIn1.cols;
    int maxY = ImIn1.rows;


    Mat Mask1;
    Mat Mask2;

    // thresholding
    Mask1 = FindMaskFromGray(ImIn1,threshVal);
    Mask2 = FindMaskFromGray(ImIn2,threshVal);


    // remove regions of size 1 pix
    if(removeSmallReg)
    {
        Mask1 = RemovingTinyReg9(Mask1);
        Mask2 = RemovingTinyReg9(Mask2);
    }

    // morphology on thresholded image
    switch (rawMorphErosion1)
    {
    case 1:
        RegionErosion5(Mask1);
        RegionErosion5(Mask2);
        break;
    case 2:
        RegionErosion9(Mask1);
        RegionErosion9(Mask2);
        break;
    case 3:
        RegionErosion13(Mask1);
        RegionErosion13(Mask2);
        break;
    default:
        break;
    }

    switch (rawMorphDilation2)
    {
    case 1:
        RegionDilation5(Mask1);
        RegionDilation5(Mask2);
        break;
    case 2:
        RegionDilation9(Mask1);
        RegionDilation9(Mask2);
        break;
    case 3:
        RegionDilation13(Mask1);
        RegionDilation13(Mask2);
        break;
    default:
        break;
    }

    switch (rawMorphErosion3)
    {
    case 1:
        RegionErosion5(Mask1);
        RegionErosion5(Mask2);
        break;
    case 2:
        RegionErosion9(Mask1);
        RegionErosion9(Mask2);
        break;
    case 3:
        RegionErosion13(Mask1);
        RegionErosion13(Mask2);
        break;
    default:
        break;
    }

    // procedure for removing choles inside object, obligatory for gradient thesholded image, usefull in other case
    if(fillHoles)
    {
        FillBorderWithValue(Mask1, 0xFFFF);
        OneRegionFill5Fast1(Mask1,  0xFFFF);
        FillHoles(Mask1);
        DeleteRegionFromImage(Mask1, 0xFFFF);

        FillBorderWithValue(Mask2, 0xFFFF);
        OneRegionFill5Fast1(Mask2,  0xFFFF);
        FillHoles(Mask2);
        DeleteRegionFromImage(Mask2, 0xFFFF);
    }

    // labelling
    if(divideSeparateReg)
    {
        DivideSeparateRegions(Mask1, MinRegSize);
        DivideSeparateRegions(Mask2, MinRegSize);
    }

    //morphology after labelling
    switch (regMorphErosion1)
    {
    case 1:
        RegionErosion5(Mask1);
        RegionErosion5(Mask2);
        break;
    case 2:
        RegionErosion9(Mask1);
        RegionErosion9(Mask2);
        break;
    case 3:
        RegionErosion13(Mask1);
        RegionErosion13(Mask2);
        break;
    default:
        break;
    }

    switch (regMorphDilation2)
    {
    case 1:
        RegionDilation5(Mask1);
        RegionDilation5(Mask2);
        break;
    case 2:
        RegionDilation9(Mask1);
        RegionDilation9(Mask2);
        break;
    case 3:
        RegionDilation13(Mask1);
        RegionDilation13(Mask2);
        break;
    default:
        break;
    }

    switch (regMorphErosion3)
    {
    case 1:
        RegionErosion5(Mask1);
        RegionErosion5(Mask2);
        break;
    case 2:
        RegionErosion9(Mask1);
        RegionErosion9(Mask2);
        break;
    case 3:
        RegionErosion13(Mask1);
        RegionErosion13(Mask2);
        break;
    default:
        break;
    }

    // removal of regions touchig image border
    if(removeBorderRegion)
    {
        DeleteRegTouchingBorder(Mask1);
        DeleteRegTouchingBorder(Mask2);
    }

    // allign Grains
    int maxPosY = 0;
    int minPosY = maxY-1;
    int maxPosX = 0;
    int minPosX = maxX-1;
    unsigned short *wMask1;

    int maxXY = maxX*maxY;
    if(alignGrains)
    {
        wMask1 = (unsigned short*)Mask1.data;
        for(int i = 0; i < maxXY; i++)
        {
            int y = i/maxX;
            int x = i%maxX;

            if(*wMask1)
            {
                if(maxPosY < y)
                    maxPosY = y;
                if(minPosY > y)
                    minPosY = y;
                if(maxPosX < x)
                    maxPosX = x;
                if(minPosX > x)
                    minPosX = x;
            }

            wMask1++;
        }

    }

    int linesToCount = 200;
    int pixelCount = linesToCount * maxX;

    int uStartLine = minPosY;
    if(uStartLine <0)
        uStartLine = 0;
    int uStopLine =  uStartLine + linesToCount;
    if(uStopLine >= maxY)
    {
        uStopLine = maxY - 1;
        uStartLine = uStopLine - linesToCount;
    }
    int lStopLine  =  maxPosY;
    if(lStopLine >= maxY)
        lStopLine = maxY - 1;
    int lStartLine = lStopLine - linesToCount;
    if(lStartLine < 0)
    {
        lStartLine = 0;
        lStopLine = lStartLine + linesToCount;
    }

    if(alignGrains)
    {
        wMask1 = (unsigned short*)Mask1.data + maxX * uStartLine;//(imCenterY - linesToCount - offsetToCount);
        int uRegPixelCount = 0;
        for(int i = 0; i < pixelCount; i++)
        {
            if(*wMask1)
                uRegPixelCount++;

            wMask1++;
        }

        wMask1 = (unsigned short*)Mask1.data + maxX * lStartLine;//(imCenterY + offsetToCount);
        int lRegPixelCount = 0;
        for(int i = 0; i< pixelCount; i++)
        {
            if(*wMask1)
                lRegPixelCount++;

            wMask1++;
        }


        if(uRegPixelCount >= lRegPixelCount)
        {
            flip(ImIn1,ImIn1, -1);
            flip(ImIn2,ImIn2, -1);
            flip(Mask1,Mask1, -1);
            flip(Mask2,Mask2, -1);
        }
    }


    int rectHeight = 300;
    int rectWidth = 100;
//    int rectLCX = ImIn1.cols/2 - rectWidth/2;
//    int rectLCY = ImIn1.rows/2 - rectHeight/2;

    Mat Mask1a = Mat::zeros(maxY,maxX,Mask1.type());
    Mat Mask2a = Mat::zeros(maxY,maxX,Mask1.type());


    int ellipseSizeX = (maxPosX-minPosX)/4;
    int ellipseSizeY = (maxPosY-minPosY)/4;

    if(ellipseSizeX < 0)
        ellipseSizeX = 0;
    if(ellipseSizeY < 0)
        ellipseSizeY = 0;


    ellipse(Mask1a, Point(maxX/2,maxY/2),Size(ellipseSizeX,ellipseSizeY),0,0,360,1,-1);
    ellipse(Mask2a, Point(maxX/2,maxY/2),Size(ellipseSizeX,ellipseSizeY),0,0,360,1,-1);

    // find valey(znajdź bruzdke)
    Mat ImGray1, ImGray2;
    cvtColor(ImIn1,ImGray1,CV_BGR2GRAY);
    cvtColor(ImIn2,ImGray2,CV_BGR2GRAY);

    blur(ImGray2,ImGray2,Size(5,5));

    Mat ImGradient1 = HorizontalGradientDown(ImGray1);
    Mat ImGradient2 = HorizontalGradientDown(ImGray2);

    //Mat ImShowGradient = Combine2Images(ShowImageF32PseudoColor(ImGradient1, 0.0, 100.0), ShowImageF32PseudoColor(ImGradient2, 0.0, 100.0));

    bool switchImages;

    if(findValey)
    {

        float sum1 = AverageMaskedPixelsF(Mask1a, ImGradient1);
        float sum2 = AverageMaskedPixelsF(Mask2a, ImGradient2);


        if(sum1 < sum2)
        {
            Mask1 = Mask1 * 2;
            switchImages = 1;
        }
        else
        {
            Mask2 = Mask2 * 2;
            switchImages = 0;
        }
    }
    if(switchImages)
    {
        Mat TempMat;
        TempMat = Mask1;
        Mask1 = Mask2;
        Mask2 = TempMat;

        TempMat = ImIn1;
        ImIn1 = ImIn2;
        ImIn2 = TempMat;
    }

// output ROI
    //vector <MR2DType*> rois;
    int begin[MR2DType::Dimensions];
    int end[MR2DType::Dimensions];
    begin[0] = 0;
    begin[1] = 0;
    end[0] = maxX*2 -1;
    end[1] = maxY-1;
    unsigned short *wMask;

    Mat Mask = Combine2Images(Mask1, Mask2);

    //Bruzdka
    MR2DType* roi1;
    roi1 = new MR2DType(begin, end);
    MazdaRoiIterator<MR2DType> iterator1(roi1);
    wMask = (unsigned short*)Mask.data;
    while(! iterator1.IsBehind())
    {
        if (*wMask == 1)
            iterator1.SetPixel();
        ++iterator1;
       wMask++;
    }
    roi1->SetName("Bruzdka");
    roi1->SetColor(0xff0000);

    //rois.push_back(roi1);

    outRoi->push_back(roi1);
    //Grzbiet
    MR2DType* roi2;
    roi2 = new MR2DType(begin, end);
    MazdaRoiIterator<MR2DType> iterator2(roi2);
    wMask = (unsigned short*)Mask.data;
    while(! iterator2.IsBehind())
    {
        if (*wMask ==2)
            iterator2.SetPixel();
        ++iterator2;
       wMask++;
    }
    roi2->SetName("Grzbiet");
    roi2->SetColor(0x00ff00);
    //rois.push_back(roi2);
    outRoi->push_back(roi2);

/*
// output ROI
    //vector <MR2DType*> rois;
    int begin[MR2DType::Dimensions];
    int end[MR2DType::Dimensions];
    begin[0] = 0;
    begin[1] = 0;
    end[0] = maxX-1;
    end[1] = maxY-1;
    unsigned short *wMask;

    //Bruzdka
    MR2DType* roi1;
    roi1 = new MR2DType(begin, end);
    MazdaRoiIterator<MR2DType> iterator1(roi1);
    wMask = (unsigned short*)Mask1.data;
    while(! iterator1.IsBehind())
    {
        if (*wMask)
            iterator1.SetPixel();
        ++iterator1;
       wMask++;
    }
    roi1->SetName("Bruzdka");
    roi1->SetColor(0xff0000);
    rois.push_back(roi1);

    //Grzbiet
    MR2DType* roi2;
    roi2 = new MR2DType(begin, end);
    MazdaRoiIterator<MR2DType> iterator2(roi2);
    wMask = (unsigned short*)Mask2.data;
    while(! iterator2.IsBehind())
    {
        if (*wMask)
            iterator2.SetPixel();
        ++iterator2;
       wMask++;
    }
    roi2->SetName("Grzbiet");
    roi2->SetColor(0x00ff00);
    rois.push_back(roi2);
*/


    //ImIn->release();
    //*ImIn = Combine2Images(ImIn1, ImIn2);
    ImOutVect->push_back(&ImIn1);
    ImOutVect->push_back(&ImIn2);

    Mask1.release();
    Mask2.release();
    Mask1a.release();
    Mask2a.release();

    //ImIn1.release();
    //ImIn2.release();
    ImGradient1.release();
    ImGradient2.release();
    ImGray1.release();
    ImGray1.release();

    return 1;

}
//--------------------------------------------------------------------------------------------------

