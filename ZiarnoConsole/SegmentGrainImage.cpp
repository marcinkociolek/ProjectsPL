//#include <iostream>
//#include <string>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "gradient.h"

#define TERAZ_DEBUG



#include "mazdaroi.h"
#include "mazdaroiio.h"
//#include "../../qmazda/SharedImage/mazdaroi.h"
//#include "../../qmazda/SharedImage/mazdaroiio.h"

#include "SegmentGrainImage.h"



typedef MazdaRoi<unsigned int, 2> MR2DType;

using namespace std;
using namespace cv;

//---------------------------------------------------------------------------------------------
//              functions
//--------------------------------------------------------------------------------------------------

#ifdef TERAZ_DEBUG
#include "DispLib.h"

#endif

//---------------------------------------------------------------------------------------------
Mat FindMaskFromGray(Mat ImIn,int thesholdVal)
{
    Mat ImGray;
    Mat ImMask;

//pms
//    if(ImIn.empty())
//        return ImMask;

    cvtColor(ImIn,ImGray,CV_BGR2GRAY);

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

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------

bool SegmentGrainImg(const std::vector<Mat*> *ImInVect, std::vector<Mat*> *ImOutVect, vector <MR2DType*> * outRoi, std::vector<TransformacjaZiarna> *transf, SegmentParams *params)
{
    bool minSizeReached = true;
/*
    const int params_threshVal = 16;
    const bool params_removeSmallReg = true;
    const int params_rawMorphErosion1 = 3;
    const int params_rawMorphDilation2 = 0;
    const int params_rawMorphErosion3 = 0;
    const bool params_fillHoles = true;
    const bool params_divideSeparateReg = true;
    const int params_MinRegSize = 10000;
    const int params_regMorphErosion1 = 0;
    const int params_regMorphDilation2 = 0;
    const int params_regMorphErosion3 = 0;
    const bool params_removeBorderRegion = false;
    const bool params_fitEllipseToReg = true;
    const bool params_rotateImage = true;
    const bool params_croppImage = true;
    const bool params_alignGrains = true;
    const bool params_addBlurToSecondImage = false;
    const bool params_findValey = true;
*/

/* Vertical split

    const bool params_threshVal = 26;
    const bool params_removeSmallReg = true;
    const bool params_rawMorphErosion1 = 3;
    const bool params_rawMorphDilation2 = 0;
    const bool params_rawMorphErosion3 = 0;
    const bool params_fillHoles = true;
    const bool params_divideSeparateReg = true;
    const bool params_MinRegSize = 10000;
    const bool params_regMorphErosion1 = 0;
    const bool params_regMorphDilation2 = 0;
    const bool params_regMorphErosion3 = 0;
    const bool params_removeBorderRegion = true;
    const bool params_fitEllipseToReg = true;
    const bool params_rotateImage = true;
    const bool params_croppImage = true;
    const bool params_alignGrains = true;
    const bool params_addBlurToSecondImage = false;
    const bool params_findValey = true;
*/


#ifdef TERAZ_DEBUG
/*
    const bool params_showContour = false;
    const bool params_showInput = false;
    const bool params_showThesholded = false;
    const bool params_show1stMorphology = false;
    const bool params_showHolesRemoved = false;
    const bool params_showMask = false;
    const bool params_showContourOnInput = false;
    const bool params_showFitted = false;
    const bool params_showRotated = false;
    const bool params_showCropped = false;
    const bool params_showAreaForAlign = false;
    const bool params_showAligned = false;
    const bool params_showGradient = false;
    const bool params_showOutput = false;
*/
#endif
  
    Mat ImIn1, ImIn2;

//pms zbedne kopiowanie
// mk bez tego kopiowania rozmywam obydwa obrazy wejściowe obracam i przycinam
    (*ImInVect->at(0)).copyTo(ImIn1);
    (*ImInVect->at(1)).copyTo(ImIn2);

#ifdef TERAZ_DEBUG
    ShowImageCombination(params->showInput,"Input image",ImIn1, ImIn2);
#endif

    int maxX = ImIn1.cols;
    int maxY = ImIn1.rows;

    Mat Mask1;
    Mat Mask2;

    // thresholding
    Mask1 = FindMaskFromGray(ImIn1,params->threshVal);
    Mask2 = FindMaskFromGray(ImIn2,params->threshVal);

#ifdef TERAZ_DEBUG
    ShowImageRegionCombination(params->showThesholded, params->showContour, "Thresholded", ImIn1, ImIn2, Mask1, Mask2);
#endif

    // remove regions of size 1 pix
    if(params->removeSmallReg)
    {
        Mask1 = RemovingTinyReg9(Mask1);
        Mask2 = RemovingTinyReg9(Mask2);
    }

    // morphology on thresholded image
    switch (params->rawMorphErosion1)
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

    switch (params->rawMorphDilation2)
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

    switch (params->rawMorphErosion3)
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

#ifdef TERAZ_DEBUG
    ShowImageRegionCombination(params->show1stMorphology, params->showContour, "Morphology1", ImIn1, ImIn2, Mask1, Mask2);
#endif

    // procedure for removing choles inside object, obligatory for gradient thesholded image, usefull in other case
    if(params->fillHoles)
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

#ifdef TERAZ_DEBUG
    ShowImageRegionCombination(params->showHolesRemoved, params->showContour, "WithoutHoles", ImIn1, ImIn2, Mask1, Mask2);
#endif

    // labelling
    if(params->divideSeparateReg)
    {
        DivideSeparateRegions(Mask1, params->MinRegSize);
        DivideSeparateRegions(Mask2, params->MinRegSize);
    }

    //morphology after labelling
    switch (params->regMorphErosion1)
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

    switch (params->regMorphDilation2)
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

    switch (params->regMorphErosion3)
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
    if(params->removeBorderRegion)
    {
        DeleteRegTouchingBorder(Mask1);
        DeleteRegTouchingBorder(Mask2);
    }

#ifdef TERAZ_DEBUG
    ShowImageRegionCombination(params->showMask, params->showContour, "Mask", ImIn1, ImIn2, Mask1, Mask2);
#endif

    // find centroids
    int centerX1 = maxX;
    int centerY1 = maxY;
    int centerX2 = maxX;
    int centerY2 = maxY;
    unsigned short *wMask1;
    unsigned short *wMask;

    maxX = Mask1.cols;
    maxY = Mask1.rows;

    int sumX = 0;
    int sumY = 0;
    int sumPix = 0;

    wMask = (unsigned short *)Mask1.data;
    for(int y = 0; y < maxY; y++)
    {
        for(int x = 0; x < maxX; x++)
        {
            if(*wMask)
            {
                sumX += x;
                sumY += y;
                sumPix ++;
            }
            wMask++;
        }
    }

    if (sumPix < params->MinRegSize)
    {
        Mask1.release();
        Mask2.release();
        return 0;
    }

    centerX1 = sumX/sumPix;
    centerY1 = sumY/sumPix;

    maxX = Mask2.cols;
    maxY = Mask2.rows;
    sumX = 0;
    sumY = 0;
    sumPix = 0;

    wMask = (unsigned short *)Mask2.data;
    for(int y = 0; y < maxY; y++)
    {
        for(int x = 0; x < maxX; x++)
        {
            if(*wMask)
            {
                sumX += x;
                sumY += y;
                sumPix ++;
            }
            wMask++;
        }
    }
    if (sumPix < params->MinRegSize)
    {
        Mask1.release();
        Mask2.release();
        return 0;
    }

    centerX2 = sumX/sumPix;
    centerY2 = sumY/sumPix;

    //fit ellipse
    RotatedRect fittedRect1,fittedRect2 ;
    fittedRect1.angle = 0.0;
    fittedRect1.center = Point2f((float)(ImIn1.cols)/2,(float)(ImIn1.rows)/2);
    fittedRect1.size = Size2f(100.0,100.0);

    fittedRect2 = fittedRect1;

    if(params->fitEllipseToReg)
    {
        Mat ImTemp;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        Mat pointsF;

        Mask1.convertTo(ImTemp,CV_8U);
        findContours(ImTemp,contours,hierarchy,CV_RETR_LIST,CHAIN_APPROX_NONE);
        if(contours.size())
        {
            Mat(contours[0]).convertTo(pointsF, CV_32F);
            fittedRect1 = fitEllipse(pointsF);
        }

        contours.clear();
        hierarchy.clear();

        Mask2.convertTo(ImTemp,CV_8U);
        findContours(ImTemp,contours,hierarchy,CV_RETR_LIST,CHAIN_APPROX_NONE);
        if(contours.size())
        {
            Mat(contours[0]).convertTo(pointsF, CV_32F);
            fittedRect2 = fitEllipse(pointsF);
        }
    }

#ifdef TERAZ_DEBUG
    if(params->showFitted)
    {
        Point2f vertices[4];
        Mat ImToShow1,ImToShow2;
        fittedRect1.points(vertices);
        ImIn1.copyTo(ImToShow1);
        for (int i = 0; i < 4; i++)
            line(ImToShow1, vertices[i], vertices[(i+1)%4], Scalar(0,255,0));

        fittedRect2.points(vertices);
        ImIn2.copyTo(ImToShow2);
        for (int i = 0; i < 4; i++)
            line(ImToShow2, vertices[i], vertices[(i+1)%4], Scalar(0,255,0));

        Mat ImShow = Mat::zeros(Size(maxX *2,maxY) ,ImIn1.type());
        ImToShow1.copyTo(ImShow(Rect(0, 0, maxX, maxY)));
        ImToShow2.copyTo(ImShow(Rect(maxX, 0, maxX, maxY)));
        imshow("Fitted", ImShow);
    }
#endif

    // rotate images
    RotatedRect alignedRect1 = fittedRect1;
    alignedRect1.angle = 0.0;
    RotatedRect alignedRect2 = fittedRect2;
    alignedRect2.angle = 0.0;

    if(params->rotateImage)
    {
        Mat RotationMatrix1,RotationMatrix2;

        RotationMatrix1 = getRotationMatrix2D(fittedRect1.center,fittedRect1.angle,1.0);

        RotationMatrix1 = getRotationMatrix2D(fittedRect1.center,fittedRect1.angle,1.0);
        RotationMatrix2 = getRotationMatrix2D(fittedRect2.center,-fittedRect1.angle,1.0);

        Mat ImRotated1, ImRotated2, MaskRotated1, MaskRotated2;

        warpAffine(ImIn1,ImRotated1,RotationMatrix1,Size(ImIn1.cols,ImIn1.rows));
        warpAffine(Mask1,MaskRotated1,RotationMatrix1,Size(ImIn1.cols,ImIn1.rows));

        warpAffine(ImIn2,ImRotated2,RotationMatrix2,Size(ImIn2.cols,ImIn2.rows));
        warpAffine(Mask2,MaskRotated2,RotationMatrix2,Size(ImIn2.cols,ImIn2.rows));

        ImIn1.release();
        ImIn2.release();
        Mask1.release();
        Mask2.release();
        ImIn1 = ImRotated1;
        ImIn2 = ImRotated2;
        Mask1 = MaskRotated1;
        Mask2 = MaskRotated2;
        maxX = Mask1.cols;
        maxY = Mask1.rows;
    }

#ifdef TERAZ_DEBUG
    ShowImageRegionCombination(params->showRotated, params->showContour, "Rotated" , ImIn1, ImIn2, Mask1, Mask2);
#endif

    if(params->croppImage)
    {
        int croppX1Min = Mask1.cols;
        int croppX1Max = 0;

        int croppY1Min = Mask1.rows;
        int croppY1Max = 0;

        maxX = Mask1.cols;
        maxY = Mask1.rows;

        wMask = (unsigned short *)Mask1.data;
        for(int y = 0; y < maxY; y++)
        {
            for(int x = 0; x < maxX; x++)
            {
                if(*wMask)
                {
                    if(croppX1Min > x)
                        croppX1Min = x;
                    if(croppX1Max < x)
                        croppX1Max = x;

                    if(croppY1Min > y)
                        croppY1Min = y;
                    if(croppY1Max < y)
                        croppY1Max = y;
                }
                wMask++;
            }
        }

        int croppX2Min = Mask2.cols;
        int croppX2Max = 0;

        int croppY2Min = Mask2.rows;
        int croppY2Max = 0;

        maxX = Mask2.cols;
        maxY = Mask2.rows;

        wMask = (unsigned short *)Mask2.data;
        for(int y = 0; y < maxY; y++)
        {
            for(int x = 0; x < maxX; x++)
            {
                if(*wMask)
                {
                    if(croppX2Min > x)
                        croppX2Min = x;
                    if(croppX2Max < x)
                        croppX2Max = x;

                    if(croppY2Min > y)
                        croppY2Min = y;
                    if(croppY2Max < y)
                        croppY2Max = y;
                }
                wMask++;
            }
        }

        //Size croppedImageSize;
        int croppWidth1 = croppX1Max - croppX1Min + 1;
        int croppHeight1 = croppY1Max - croppY1Min + 1;
        int croppX1 = croppX1Min;
        int croppY1 = croppY1Min;

        if (croppHeight1 <= 200)
            minSizeReached = false; //return 0;
        if (croppWidth1 <= 50)
            minSizeReached = false; //return 0;


        Mat ImCropped1, ImCropped2, MaskCropped1, MaskCropped2;

        ImIn1(Rect(croppX1,croppY1,croppWidth1,croppHeight1)).copyTo(ImCropped1);
        Mask1(Rect(croppX1,croppY1,croppWidth1,croppHeight1)).copyTo(MaskCropped1);

        int croppWidth2 = croppX2Max - croppX2Min + 1;
        int croppHeight2 = croppY2Max - croppY2Min + 1;
        int croppX2 = croppX2Min;
        int croppY2 = croppY2Min;

        if (croppHeight2 <= 200)
            minSizeReached = false; //return 0;
        if (croppWidth2 <= 50)
            minSizeReached = false; //return 0;

        ImIn2(Rect(croppX2,croppY2,croppWidth2,croppHeight2)).copyTo(ImCropped2);
        Mask2(Rect(croppX2,croppY2,croppWidth2,croppHeight2)).copyTo(MaskCropped2);

        ImIn1.release();
        ImIn2.release();
        Mask1.release();
        Mask2.release();
        ImIn1 = ImCropped1;
        ImIn2 = ImCropped2;
        Mask1 = MaskCropped1;
        Mask2 = MaskCropped2;
        maxX = Mask1.cols;
        maxY = Mask1.rows;
    }

#ifdef TERAZ_DEBUG
    ShowImageRegionCombination(params->showCropped, params->showContour, "Cropped" , ImIn1, ImIn2, Mask1, Mask2);
#endif

    // allign Grains
    int maxPosY = 0;
    int minPosY = maxY-1;
    int maxPosX = 0;
    int minPosX = maxX-1;

    int maxXY = maxX*maxY;
    if(params->alignGrains && minSizeReached)
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

    bool flipped = false;
    if(params->alignGrains && minSizeReached)
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
            flipped = true;
        }
    }

#ifdef TERAZ_DEBUG
    ShowHLinesOnImage(params->showAreaForAlign, "AreaForAlign", ImIn1, ImIn2,uStartLine,uStopLine, lStartLine, lStopLine);// - offsetToCount, imCenterY + offsetToCount, imCenterY + offsetToCount + linesToCount);
    ShowImageRegionCombination(params->showAligned, params->showContour, "Aligned", ImIn1,ImIn2, Mask1, Mask2);
#endif

    Mat Mask1a = Mat::zeros(Mask1.rows ,Mask1.cols ,Mask1.type());
    Mat Mask2a = Mat::zeros(Mask2.rows ,Mask2.cols ,Mask2.type());

    int ellipseSizeX = (maxPosX-minPosX)/4;
    int ellipseSizeY = (maxPosY-minPosY)/4;

    if(ellipseSizeX < 0)
        ellipseSizeX = 0;
    if(ellipseSizeY < 0)
        ellipseSizeY = 0;

    ellipse(Mask1a, Point(Mask1a.cols/2,Mask1a.rows/2),Size(ellipseSizeX,ellipseSizeY),0,0,360,1,-1);
    ellipse(Mask2a, Point(Mask2a.cols/2,Mask2a.rows/2),Size(ellipseSizeX,ellipseSizeY),0,0,360,1,-1);

    // find valey(znajdź bruzdke)
    Mat ImGray1, ImGray2;
    cvtColor(ImIn1,ImGray1,CV_BGR2GRAY);
    cvtColor(ImIn2,ImGray2,CV_BGR2GRAY);

    if(params->addBlurToSecondImage)
        blur(ImGray2,ImGray2,Size(5,5));

    Mat ImGradient1 = HorizontalGradientDown(ImGray1);
    Mat ImGradient2 = HorizontalGradientDown(ImGray2);

#ifdef TERAZ_DEBUG
    ShowImageRegionCombination(params->showGradient, params->showContour, "Gradient", ShowImageF32PseudoColor(ImGradient1, 0.0, 100.0),ShowImageF32PseudoColor(ImGradient2, 0.0, 100.0), Mask1a, Mask2a);
#endif

    bool switchImages = 0;
    Mat *ImOut1 = new Mat;
    Mat *ImOut2= new Mat;
    if(params->findValey)
    {
        float sum1 = AverageMaskedPixelsF(Mask1a, ImGradient1);
        float sum2 = AverageMaskedPixelsF(Mask2a, ImGradient2);

        if(sum1 < sum2)
        {
            switchImages = 1;
        }
    }

#ifdef TERAZ_DEBUG
    if(switchImages)
    {
        ShowImageRegionCombination(params->showOutput, params->showContour, "Output", ImIn2, ImIn1, Mask2, Mask1);
    }
    else
    {
        ShowImageRegionCombination(params->showOutput, params->showContour, "Output", ImIn1, ImIn2, Mask1, Mask2);
    }
#endif

    //subsegmentation
    Mat Mask1b,Mask2b;
    Mask1.copyTo(Mask1b);
    Mask2.copyTo(Mask2b);
    if(params->subsegment)
    {
        FindLefLowerEdge2(Mask1b);
        FindRightLowerEdge2(Mask1b);
        FindTop(Mask1b);

        FindLefLowerEdge2(Mask2b);
        FindRightLowerEdge2(Mask2b);
        FindTop(Mask2b);

        if(switchImages)
        {
            MarkEllipse(Mask1b,0.65,0.25,0.3);
            MarkEllipse(Mask2b,0.7,0.2,0.25);
        }
        else
        {
            MarkEllipse(Mask1b,0.7,0.2,0.25);
            MarkEllipse(Mask2b,0.65,0.25,0.3);
        }
    }

    if(params->subsegment)
    {


    }
/*
    if(switchImages)
    {
        AddValToRegNr(Mask1, 5);
    }
    else
    {
        AddValToRegNr(Mask2, 5);
    }
*/
#ifdef TERAZ_DEBUG
    if(switchImages)
    {
        ShowImageRegionCombination(params->showOutput2, params->showContour, "Output2", ImIn2, ImIn1, Mask2b, Mask1b);
    }
    else
    {
        ShowImageRegionCombination(params->showOutput2, params->showContour, "Output2", ImIn1, ImIn2, Mask1b, Mask2b);
    }
#endif


    *ImOut1 = ImIn1;
    *ImOut2 = ImIn2;
// output ROI
    int begin[MR2DType::Dimensions];
    int end[MR2DType::Dimensions];

    //Mask 1
    begin[0] = 0;
    begin[1] = 0;
    end[0] = Mask1.cols-1;
    end[1] = Mask1.rows-1;
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

    MR2DType* roi11;
    roi11 = new MR2DType(begin, end);
    MazdaRoiIterator<MR2DType> iterator11(roi11);
    wMask = (unsigned short*)Mask1b.data;
    while(! iterator11.IsBehind())
    {
        if (*wMask==2)
            iterator11.SetPixel();
        ++iterator11;
       wMask++;
    }

    MR2DType* roi12;
    roi12 = new MR2DType(begin, end);
    MazdaRoiIterator<MR2DType> iterator12(roi12);
    wMask = (unsigned short*)Mask1b.data;
    while(! iterator12.IsBehind())
    {
        if (*wMask==3)
            iterator12.SetPixel();
        ++iterator12;
       wMask++;
    }

    MR2DType* roi13;
    roi13 = new MR2DType(begin, end);
    MazdaRoiIterator<MR2DType> iterator13(roi13);
    wMask = (unsigned short*)Mask1b.data;
    while(! iterator13.IsBehind())
    {
        if (*wMask==4)
            iterator13.SetPixel();
        ++iterator13;
       wMask++;
    }

    MR2DType* roi14;
    roi14 = new MR2DType(begin, end);
    MazdaRoiIterator<MR2DType> iterator14(roi14);
    wMask = (unsigned short*)Mask1b.data;
    while(! iterator14.IsBehind())
    {
        if (*wMask==5)
            iterator14.SetPixel();
        ++iterator14;
       wMask++;
    }



    if(switchImages)
    {
        roi1->SetName("D");
        roi1->SetColor(0x00ff00);
        roi11->SetName("D1");
        roi11->SetColor(0x00ff00);
        roi12->SetName("D2");
        roi12->SetColor(0x00ff00);
        roi13->SetName("D3");
        roi13->SetColor(0x00ff00);
        roi14->SetName("D4");
        roi14->SetColor(0x00ff00);
    }
    else
    {
        roi1->SetName("V");
        roi1->SetColor(0xff0000);
        roi11->SetName("V1");
        roi11->SetColor(0xff0000);
        roi12->SetName("V2");
        roi12->SetColor(0xff0000);
        roi13->SetName("V3");
        roi13->SetColor(0xff0000);
        roi14->SetName("V4");
        roi14->SetColor(0xff0000);


    }

    outRoi->push_back(roi1);
    outRoi->push_back(roi11);
    outRoi->push_back(roi12);
    outRoi->push_back(roi13);
    outRoi->push_back(roi14);

    // Mask 2
    begin[0] = 0;
    begin[1] = 0;
    end[0] = Mask2.cols-1;
    end[1] = Mask2.rows-1;
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
    MR2DType* roi21;
    roi21 = new MR2DType(begin, end);
    MazdaRoiIterator<MR2DType> iterator21(roi21);
    wMask = (unsigned short*)Mask2b.data;
    while(! iterator21.IsBehind())
    {
        if (*wMask==2)
            iterator21.SetPixel();
        ++iterator21;
       wMask++;
    }

    MR2DType* roi22;
    roi22 = new MR2DType(begin, end);
    MazdaRoiIterator<MR2DType> iterator22(roi22);
    wMask = (unsigned short*)Mask2b.data;
    while(! iterator22.IsBehind())
    {
        if (*wMask==3)
            iterator22.SetPixel();
        ++iterator22;
       wMask++;
    }

    MR2DType* roi23;
    roi23 = new MR2DType(begin, end);
    MazdaRoiIterator<MR2DType> iterator23(roi23);
    wMask = (unsigned short*)Mask2b.data;
    while(! iterator23.IsBehind())
    {
        if (*wMask==4)
            iterator23.SetPixel();
        ++iterator23;
       wMask++;
    }

    MR2DType* roi24;
    roi24 = new MR2DType(begin, end);
    MazdaRoiIterator<MR2DType> iterator24(roi24);
    wMask = (unsigned short*)Mask2b.data;
    while(! iterator24.IsBehind())
    {
        if (*wMask==5)
            iterator24.SetPixel();
        ++iterator24;
       wMask++;
    }




    if(switchImages)
    {
        roi2->SetName("V");
        roi2->SetColor(0xff0000);
        roi21->SetName("V1");
        roi21->SetColor(0xff0000);
        roi22->SetName("V2");
        roi22->SetColor(0xff0000);
        roi23->SetName("V3");
        roi23->SetColor(0xff0000);
        roi24->SetName("V4");
        roi24->SetColor(0xff0000);
    }
    else
    {
        roi2->SetName("D");
        roi2->SetColor(0x00ff00);
        roi21->SetName("D1");
        roi21->SetColor(0x00ff00);
        roi22->SetName("D2");
        roi22->SetColor(0x00ff00);
        roi23->SetName("D3");
        roi23->SetColor(0x00ff00);
        roi24->SetName("D4");
        roi24->SetColor(0x00ff00);
    }

    outRoi->push_back(roi2);
    outRoi->push_back(roi21);
    outRoi->push_back(roi22);
    outRoi->push_back(roi23);
    outRoi->push_back(roi24);

    ImOutVect->push_back(ImOut1);
    ImOutVect->push_back(ImOut2);

    TransformacjaZiarna transformacja;// = new TransformacjaZiarna;
    transformacja.x = centerX1;
    transformacja.y = centerY1;
    if(flipped)
        transformacja.angle = fittedRect1.angle + 180.0;
    else
        transformacja.angle = fittedRect1.angle;

    transf->push_back(transformacja);

    transformacja.x = centerX2;
    transformacja.y = centerY2;
    if(flipped)
        transformacja.angle = -fittedRect1.angle + 180;
    else
        transformacja.angle = -fittedRect1.angle;

    transf->push_back(transformacja);

    Mask1.release();
    Mask2.release();
    Mask1a.release();
    Mask2a.release();
    Mask1b.release();
    Mask2b.release();

    ImGradient1.release();
    ImGradient2.release();
    ImGray1.release();
    ImGray1.release();

    return 1;

}
//--------------------------------------------------------------------------------------------------

bool SegmentGrainImg(const std::vector<Mat*> *ImInVect, std::vector<Mat*> *ImOutVect, vector <MR2DType*> * outRoi, std::vector<TransformacjaZiarna> *transf)
{
    SegmentParams segParams;
    segParams.defaultHorizontal();
    return SegmentGrainImg(ImInVect, ImOutVect,  outRoi, transf, &segParams);
}
