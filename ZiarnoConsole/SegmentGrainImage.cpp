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
    if (imMaxX < Im2.cols)
        imMaxX = Im2.cols;
    imMaxY = Im1.rows;
    if(imMaxY < Im2.rows)
        imMaxY = Im2.rows;
    Mat Im = Mat::zeros(Size(imMaxX *2,imMaxY) ,Im1.type());
    Im1.copyTo(Im(Rect(0, 0, Im1.cols, Im1.rows)));
    Im2.copyTo(Im(Rect(imMaxX, 0, Im2.cols, Im2.rows)));

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

//--------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
bool SegmentGrainImg(const std::vector<Mat*> *ImInVect, std::vector<Mat*> *ImOutVect, vector <MR2DType*> * outRoi,std::vector<TransformacjaZiarna*> *transf)
{
    // patrameters could be made input or external
    int threshVal = 26;
    bool removeSmallReg = true;
    int rawMorphErosion1 = 3;
    int rawMorphDilation2 = 0;
    int rawMorphErosion3 = 0;
    bool fillHoles = true;
    bool divideSeparateReg = true;
    int MinRegSize = 10000;
    int regMorphErosion1 = 0;
    int regMorphDilation2 = 0;
    int regMorphErosion3 = 0;
    bool removeBorderRegion = true;

    bool fitEllipseToReg = true;
    bool rotateImage = true;
    bool croppImage = true;

    bool alignGrains = true;

    bool addBlurToSecondImage = false;
    bool findValey = true;

    //vector <MR2DType*> rois;

 //   Mat ImIn;
 //   ImIn = ImIpl;
//  if (ImIn->empty())
//        return rois;
//    int maxX = ImIn->cols/2;
//    int maxY = ImIn->rows;
    Mat ImIn1, ImIn2;// ImIn1T, ImIn2T;

//    (*ImIn)(Rect(0, 0, maxX, maxY)).copyTo(ImIn1);
//    (*ImIn)(Rect(maxX, 0, maxX, maxY)).copyTo(ImIn2);

   // ImInVect[0].copyTo(ImIn1);

    //ImIn1T = *ImInVect->at(0);
    //ImIn2 = *ImInVect->at(1);

    //ImIn1T.copyTo(ImIn1);
    (*ImInVect->at(0)).copyTo(ImIn1);
    (*ImInVect->at(1)).copyTo(ImIn2);


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
    centerX1 = sumX/sumPix;
    centerY1 = sumY/sumPix;

    if (sumPix < MinRegSize)
        return 0;

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
    centerX2 = sumX/sumPix;
    centerY2 = sumY/sumPix;

    if (sumPix < MinRegSize)
        return 0;


    //fit ellipse
    RotatedRect fittedRect1,fittedRect2 ;
    fittedRect1.angle = 0.0;
    fittedRect1.center = Point2f(ImIn1.cols/2,ImIn1.rows/2);
    fittedRect1.size = Size2f(100.0,100.0);

    fittedRect2 = fittedRect1;

    if(fitEllipseToReg)
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
    // rotate images
    RotatedRect alignedRect1 = fittedRect1;
    alignedRect1.angle = 0.0;
    RotatedRect alignedRect2 = fittedRect2;
    alignedRect2.angle = 0.0;


    if(rotateImage)
    {
        Mat RotationMatrix1,RotationMatrix2;

        //Mat RegTemp;

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

    if(croppImage)
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
            return 0;
        if (croppWidth1 <= 50)
            return 0;


        Mat ImCropped1, ImCropped2, MaskCropped1, MaskCropped2;

        ImIn1(Rect(croppX1,croppY1,croppWidth1,croppHeight1)).copyTo(ImCropped1);
        Mask1(Rect(croppX1,croppY1,croppWidth1,croppHeight1)).copyTo(MaskCropped1);

        int croppWidth2 = croppX2Max - croppX2Min + 1;
        int croppHeight2 = croppY2Max - croppY2Min + 1;
        int croppX2 = croppX2Min;
        int croppY2 = croppY2Min;

        if (croppHeight2 <= 200)
            return 0;
        if (croppWidth2 <= 50)
            return 0;


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


    // allign Grains
    int maxPosY = 0;
    int minPosY = maxY-1;
    int maxPosX = 0;
    int minPosX = maxX-1;


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

    bool flipped = false;
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
            flipped = true;
        }
    }


//    int rectHeight = 300;
//    int rectWidth = 100;
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

    if(addBlurToSecondImage)
        blur(ImGray2,ImGray2,Size(5,5));

    Mat ImGradient1 = HorizontalGradientDown(ImGray1);
    Mat ImGradient2 = HorizontalGradientDown(ImGray2);

    //Mat ImShowGradient = Combine2Images(ShowImageF32PseudoColor(ImGradient1, 0.0, 100.0), ShowImageF32PseudoColor(ImGradient2, 0.0, 100.0));

    bool switchImages;
    Mat *ImOut1 = new Mat;
    Mat *ImOut2= new Mat;
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
/*
    if(switchImages)
    {
        Mat TempMat;
        TempMat = Mask1;
        Mask1 = Mask2;
        Mask2 = TempMat;

        //ImIn2.copyTo(*ImOut1);
        //ImIn1.copyTo(*ImOut2);

        *ImOut1 = ImIn2;
        *ImOut2 = ImIn1;
    }
    else
    {
        //ImIn2.copyTo(*ImOut2);
        //ImIn1.copyTo(*ImOut1);
        *ImOut1 = ImIn1;
        *ImOut2 = ImIn2;
    }
*/
    *ImOut1 = ImIn1;
    *ImOut2 = ImIn2;
// output ROI
    //vector <MR2DType*> rois;
    int begin[MR2DType::Dimensions];
    int end[MR2DType::Dimensions];


    int beginX, beginY , endX, endY;


/*
//Bruzdka
//find mask bounding box

    wMask = (unsigned short*)Mask1.data;
    beginX = maxX;
    beginY = maxY;
    endX = 0;
    endY = 0;

    for(int y = 0 ; y< maxY; y++)
    {
        for(int x = 0 ; x< maxX; x++)
        {
            if (*wMask)
            {
                if(beginX > x)
                    beginX = x;
                if(beginY > y)
                    beginY = y;
                if(endX < x)
                    endX = x;
                if(endY < y)
                    endY = y;
            }
           wMask++;
        }
    }
    begin[0] = beginX;
    begin[1] = beginY;
    end[0] = endX;
    end[1] = endY;


    //create roi
    MR2DType* roi1;
    roi1 = new MR2DType(begin, end);
    MazdaRoiIterator<MR2DType> iterator1(roi1);

    for(int y = beginY; y<=endY; y++)
    {
        wMask = (unsigned short*)Mask1.data + y *maxX + beginX;
        for(int x = beginX; x<=endX; x++)
        {
            if (*wMask && !iterator1.IsBehind())
                iterator1.SetPixel();
            ++iterator1;
           wMask++;
        }
    }
*/
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

    if(switchImages)
    {
        roi1->SetName("Grzbiet");
        roi1->SetColor(0x00ff00);
    }
    else
    {
        roi1->SetName("Bruzdka");
        roi1->SetColor(0xff0000);
    }

    outRoi->push_back(roi1);


/*
    //Grzbiet
    //find mask bounding box
    wMask = (unsigned short*)Mask2.data;
    beginX = maxX;
    beginY = maxY;
    endX = 0;
    endY = 0;

    for(int y = 0 ; y< maxY; y++)
    {
        for(int x = 0 ; x< maxX; x++)
        {
            if (*wMask)
            {
                if(beginX > x)
                    beginX = x;
                if(beginY > y)
                    beginY = y;
                if(endX < x)
                    endX = x;
                if(endY < y)
                    endY = y;
            }
           wMask++;
        }
    }
    begin[0] = beginX;
    begin[1] = beginY;
    end[0] = endX;
    end[1] = endY;

    MR2DType* roi2;
    roi2 = new MR2DType(begin, end);
    MazdaRoiIterator<MR2DType> iterator2(roi2);
    for(int y = beginY; y<=endY; y++)
    {
        wMask = (unsigned short*)Mask2.data + y *maxX + beginX;
        for(int x = beginX; x<=endX; x++)
        {
            if (*wMask && !iterator2.IsBehind())
                iterator2.SetPixel();
            ++iterator2;
           wMask++;
        }
    }
    */

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
    if(switchImages)
    {
        roi2->SetName("Bruzdka");
        roi2->SetColor(0xff0000);
    }
    else
    {
        roi2->SetName("Grzbiet");
        roi2->SetColor(0x00ff00);
    }

    //rois.push_back(roi2);
    outRoi->push_back(roi2);

    ImOutVect->push_back(ImOut1);
    ImOutVect->push_back(ImOut2);

    TransformacjaZiarna *transformacja = new TransformacjaZiarna;
    transformacja->x = centerX1;
    transformacja->y = centerY1;
    if(flipped)
        transformacja->angle = fittedRect1.angle + 180.0;
    else
        transformacja->angle = fittedRect1.angle;

    transf->push_back(transformacja);

    transformacja->x = centerX2;
    transformacja->y = centerY2;
    if(flipped)
        transformacja->angle = -fittedRect1.angle + 180;
    else
        transformacja->angle = -fittedRect1.angle;

    transf->push_back(transformacja);

    Mask1.release();
    Mask2.release();
    Mask1a.release();
    Mask2a.release();

    ImGradient1.release();
    ImGradient2.release();
    ImGray1.release();
    ImGray1.release();

    return 1;

}
//--------------------------------------------------------------------------------------------------

