#ifndef SegmentImageH
#define SegmentImageH

#include <opencv2/core/core.hpp>


#include "mazdaroi.h"
#include "mazdaroiio.h"
//#include "../../qmazda/SharedImage/mazdaroi.h"
//#include "../../qmazda/SharedImage/mazdaroiio.h"

//#define TERAZ_DEBUG


typedef MazdaRoi<unsigned int, 2> MR2DType;

using namespace cv;

struct TransformacjaZiarna
{
    double x; // dane o położeniu środka ziarniaka w obrazie oryginalnym
    double y;
    double angle; // kąt obrotu obrazu wynikowego względem oryginalnego
};

class SegmentParams
{
public:
    int threshVal;
    bool removeSmallReg;
    int rawMorphErosion1;
    int rawMorphDilation2;
    int rawMorphErosion3;
    bool fillHoles;
    bool divideSeparateReg;
    int MinRegSize;
    int regMorphErosion1;
    int regMorphDilation2;
    int regMorphErosion3;
    bool removeBorderRegion;
    bool fitEllipseToReg;
    bool rotateImage;
    bool croppImage;
    bool alignGrains;
    bool addBlurToSecondImage;
    bool findValey;

    bool showContour;
    bool showInput;
    bool showThesholded;
    bool show1stMorphology;
    bool showHolesRemoved;
    bool showMask;
    bool showContourOnInput;
    bool showFitted;
    bool showRotated;
    bool showCropped;
    bool showAreaForAlign;
    bool showAligned;
    bool showGradient;
    bool showOutput;


    void defaultHorizontal(void)
    {
        threshVal = 16;
        removeSmallReg = true;
        rawMorphErosion1 = 3;
        rawMorphDilation2 = 0;
        rawMorphErosion3 = 0;
        fillHoles = true;
        divideSeparateReg = true;
        MinRegSize = 10000;
        regMorphErosion1 = 0;
        regMorphDilation2 = 0;
        regMorphErosion3 = 0;

        removeBorderRegion = false;
        fitEllipseToReg = true;
        rotateImage = true;

        croppImage = true;
        alignGrains = true;
        addBlurToSecondImage = false;
        findValey = true;

        showContour = false;
        showInput = false;
        showThesholded = false;
        show1stMorphology = false;
        showHolesRemoved = false;
        showMask = false;
        showContourOnInput = false;
        showFitted = false;
        showRotated = false;
        showCropped = false;
        showAreaForAlign = false;
        showAligned = false;
        showGradient = false;
        showOutput = false;
    }

    void defaultVertical(void)
    {
        threshVal = 26;
        removeSmallReg = true;
        rawMorphErosion1 = 3;
        rawMorphDilation2 = 0;
        rawMorphErosion3 = 0;
        fillHoles = true;
        divideSeparateReg = true;
        MinRegSize = 10000;
        regMorphErosion1 = 0;
        regMorphDilation2 = 0;
        regMorphErosion3 = 0;

        removeBorderRegion = true;

        fitEllipseToReg = true;
        rotateImage = true;
        croppImage = true;

        alignGrains = true;
        addBlurToSecondImage = false;
        findValey = true;
        showContour = false;
        showInput = false;
        showThesholded = false;
        show1stMorphology = false;
        showHolesRemoved = false;
        showMask = false;
        showContourOnInput = false;
        showFitted = false;
        showRotated = false;
        showCropped = false;
        showAreaForAlign = false;
        showAligned = false;
        showGradient = false;
        showOutput = false;
    }
};


//Mat Combine2Images( Mat Im1, Mat Im2);
void ShowImageCombination(bool show, string WinName, Mat Im1, Mat Im2);
Mat FindMaskFromGray(Mat ImIn,int thesholdVal);
Mat FindMaskFromGradient(Mat ImIn,int thesholdVal);
float AverageMaskedPixelsF(Mat Reg, Mat ImF);

bool SegmentGrainImg(const std::vector<Mat*> *ImInVect, std::vector<Mat*> *ImOutVect, vector <MR2DType*> * outRoi,
                     std::vector<TransformacjaZiarna> *transf, SegmentParams * SegParams);

#endif
