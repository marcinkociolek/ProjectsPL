#include "ZiarnoMainWindow.h"
#include "ui_ZiarnoMainWindow.h"
#include <QFileDialog>
#include <QMessageBox>

#include <QFileSystemModel>
#include <QStringListModel>
#include <QAbstractItemModel>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <iostream>
#include <string>
#include <vector>

#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "tiffio.h"

#include "processimage.h"
#include "gradient.h"
#include "DispLib.h"

#include "mazdaroi.h"
#include "mazdaroiio.h"

typedef MazdaRoi<unsigned int, 2> MR2DType;

//QFileSystemModel *model;
QAbstractItemModel *model;


using namespace std;
using namespace boost::filesystem;
using namespace boost;
using namespace cv;

using namespace std::chrono;

// global variables
QStringList Files;



//---------------------------------------------------------------------------------------------------
// main program
//---------------------------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    ui->SegmentationComboBox->addItem("Gray Threshold Manual");
    ui->SegmentationComboBox->addItem("Gradient Threshold manual");

    if(ui->ShowComboBox->currentIndex())
        showContour = true;
    else
        showContour = false;

    ui->ShowComboBox->addItem("Region");
    ui->ShowComboBox->addItem("Contour");

    threshVal = ui->spinBox->value();
    MinRegSize = ui->MinRegionSizeSpinBox->value();

    ui->RawRegionMorfology1ComboBox->addItem("none");
    ui->RawRegionMorfology1ComboBox->addItem("erode5");
    ui->RawRegionMorfology1ComboBox->addItem("erode9");
    ui->RawRegionMorfology1ComboBox->addItem("erode13");


    ui->RawRegionMorfology2ComboBox->addItem("none");
    ui->RawRegionMorfology2ComboBox->addItem("dilate5");
    ui->RawRegionMorfology2ComboBox->addItem("dilate9");
    ui->RawRegionMorfology2ComboBox->addItem("dilate13");

    ui->RawRegionMorfology3ComboBox->addItem("none");
    ui->RawRegionMorfology3ComboBox->addItem("erode5");
    ui->RawRegionMorfology3ComboBox->addItem("erode9");
    ui->RawRegionMorfology3ComboBox->addItem("erode13");


    ui->RegionMorfology1ComboBox->addItem("none");
    ui->RegionMorfology1ComboBox->addItem("erode5");
    ui->RegionMorfology1ComboBox->addItem("erode9");
    ui->RegionMorfology1ComboBox->addItem("erode13");

    ui->RegionMorfology2ComboBox->addItem("none");
    ui->RegionMorfology2ComboBox->addItem("dilate5");
    ui->RegionMorfology2ComboBox->addItem("dilate9");
    ui->RegionMorfology2ComboBox->addItem("dilate13");

    ui->RegionMorfology3ComboBox->addItem("none");
    ui->RegionMorfology3ComboBox->addItem("erode5");
    ui->RegionMorfology3ComboBox->addItem("erode9");
    ui->RegionMorfology3ComboBox->addItem("erode13");


    // analisis options
    segmentType         = ui->SegmentationComboBox->currentIndex();
    threshVal           = ui->spinBox->value();
    removeSmallReg      = ui->RemoveSmalRegionscheckBox->checkState();
    rawMorphErosion1    = ui->RawRegionMorfology1ComboBox->currentIndex();
    rawMorphDilation2   = ui->RawRegionMorfology2ComboBox->currentIndex();
    rawMorphErosion3    = ui->RawRegionMorfology2ComboBox->currentIndex();
    fillHoles           = ui->FillHolesCheckBox->checkState();
    divideSeparateReg   = ui->DivideRegionsCheckBox->checkState();
    MinRegSize          = ui->MinRegionSizeSpinBox->value();
    regMorphErosion1    = ui->RegionMorfology1ComboBox->currentIndex();
    regMorphDilation2   = ui->RegionMorfology2ComboBox->currentIndex();
    regMorphErosion3    = ui->RegionMorfology3ComboBox->currentIndex();
    removeBorderRegion  = ui->RemoveBorderRegionCheckBox->checkState();
    alignGrains         = ui->AlignCheckBox->checkState();
    findValey           = ui->FindValeyCheckBox->checkState();

    fitEllipseToReg     = ui->FitEllipseCheckBox->checkState();
    rotateImage         = ui->RotateCheckBox->checkState();
    croppImage          = ui->CroppCheckBox->checkState();

    // display options
    showInput           = ui->ShowInputImageCheckBox->checkState();
    showThesholded      = ui->ShowThresholdedImgeCheckBox->checkState();
    show1stMorphology   = ui->ShowAfter1MorphologyCheckBox->checkState();
    showHolesRemoved    = ui->ShowWithoutHolesCheckBox->checkState();
    showMask            = ui->ShowMaskCheckBox->checkState();
    showAreaForAlign    = ui->ShowAreaForAlignCheckBox->checkState();
    showAligned         = ui->ShowAlignedCheckBox->checkState();
    showGradient        = ui->ShowGradientCheckBox->checkState();
    showOutput          = ui->ShowOutputCheckBox->checkState();




    showContourOnInput  = ui->ShowContourOnImageCheckBox->checkState();




    showFitted          = ui->ShowFitedElipseCheckBox->checkState();
    showRotated         = ui->ShowRotatedCheckBox->checkState();
    showAreaForAlign    = ui->ShowAreaForAlignCheckBox->checkState();
    showAligned         = ui->ShowAlignedCheckBox->checkState();
    if(ui->AllowResizeCheckBox->checkState())
        displayFlag = WINDOW_NORMAL;
    else
        displayFlag = WINDOW_AUTOSIZE;


    InputDirectory = "C:\\Data\\Ziarno\\14.04.2016 Rozne klasy\\Dobre\\";
    OutputDirectory = "c:\\";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
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
    ui->DirectoryLineEdit->setText(QString::fromWCharArray(InputDirectory.wstring().c_str()));
    ui->FileListWidget->clear();
    for (directory_entry& FileToProcess : directory_iterator(InputDirectory))
    {
        if (FileToProcess.path().extension() != ".bmp" && FileToProcess.path().extension() != ".png" )
            continue;

        path PathLocal = FileToProcess.path();
        if (!exists(PathLocal))
        {
            //Files << PathLocal.filename().string() << " File not exists" << "\n";
            QMessageBox msgBox;
            msgBox.setText((PathLocal.filename().string() + " File not exists" ).c_str());
            msgBox.exec();
            break;
        }
        //Files << PathLocal.filename().string().c_str();
        ui->FileListWidget->addItem(PathLocal.filename().string().c_str());
    }
   // model = new QStringListModel(Files);
    //ui->FileListView->setModel(model);
//    model->setRootPath("D:\\");  //(PathLocal.filename().string().c_str());
//    ui->FileListView->setModel(model);
//    QModelIndex index = model->index(dialog.getExistingDirectory()) ;
//    ui->FileListView->setRootIndex(index);
//    ui->FileTreeView->setRootIndex(index);
   // ui->FileListWidget->setCurrentRow(0);

}






void MainWindow::on_spinBox_valueChanged(int arg1)
{
    threshVal = arg1;
    ProcessImage();
}

void MainWindow::on_FileListWidget_currentTextChanged(const QString &currentText)
{
    FileToOpen = InputDirectory;
    FileToOpen.append(currentText.toStdWString());
    if (!exists(FileToOpen))
        return;
    ImIn = imread(FileToOpen.string().c_str());
    if (ImIn.empty())
        return;
//    maxX = ImIn.cols/2;
//    maxY = ImIn.rows;
//    ImIn(Rect(0, 0, maxX, maxY)).copyTo(ImIn1);
//    ImIn(Rect(maxX, 0, maxX, maxY)).copyTo(ImIn2);

    threshVal = ui->spinBox->value();

    ProcessImage();
}


//--------------------------------------------------------------------------------------------------
//          My functions
//--------------------------------------------------------------------------------------------------
void MainWindow::ProcessImage(void)
{
    if (ImIn.empty())
        return;
    maxX = ImIn.cols/2;
    maxY = ImIn.rows;
    ImIn(Rect(0, 0, maxX, maxY)).copyTo(ImIn1);
    ImIn(Rect(maxX, 0, maxX, maxY)).copyTo(ImIn2);



    ImShow = Combine2Images(ImIn1, ImIn2);

    steady_clock::time_point t1 = steady_clock::now();


    if(showInput)
        imshow("Input image",ImShow);
    //ShowImageCombination(showInput, , ImIn1, ImIn2);


    Mat Mask1;
    Mat Mask2;

    // thresholding
    switch (segmentType)
    {
    case 1:
        Mask1 = FindMaskFromGradient(ImIn1,threshVal);
        Mask2 = FindMaskFromGradient(ImIn2,threshVal);
        break;
    default:
        Mask1 = FindMaskFromGray(ImIn1,threshVal);
        Mask2 = FindMaskFromGray(ImIn2,threshVal);
        break;
    }

    ShowImageRegionCombination(showThesholded, showContour, "Thresholded", ImShow, Mask1, Mask2);

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

    ShowImageRegionCombination(show1stMorphology, showContour, "Morphology1", ImShow, Mask1, Mask2);

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

    ShowImageRegionCombination(showHolesRemoved, showContour, "WithoutHoles", ImShow, Mask1, Mask2);

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

    ShowImageRegionCombination(showMask, showContour, "Mask", ImShow, Mask1, Mask2);

/*
    RotatedRect fittedRect1,fittedRect2 ;
    fittedRect1.angle = 0.0;
    fittedRect1.center = Point2f(ImIn1.cols/2,ImIn1.rows/2);
    fittedRect1.size = Size2f(100.0,100.0);

    fittedRect2 = fittedRect1;


    Mat Mask1a, Mask2a;
    Mask1a = Mat::zeros(maxY,maxX,Mask1.type());
    Mask2a = Mat::zeros(maxY,maxX,Mask1.type());
*/



    //ShowHLinesOnImage(showAreaForAlign, "AreaForAlign", ImShow, imCenterY - linesToCount - offsetToCount, imCenterY - offsetToCount, imCenterY + offsetToCount, imCenterY + offsetToCount + linesToCount);

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

    ShowHLinesOnImage(showAreaForAlign, "AreaForAlign", ImShow, uStartLine,uStopLine, lStartLine, lStopLine);// - offsetToCount, imCenterY + offsetToCount, imCenterY + offsetToCount + linesToCount);
    string OutText = "";


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

        OutText += " uper pix count" +  to_string(uRegPixelCount) + "\n";
        OutText += " lower pix count" + to_string(lRegPixelCount) + "\n";

        if(uRegPixelCount >= lRegPixelCount)
        {
            flip(ImIn1,ImIn1, -1);
            flip(ImIn2,ImIn2, -1);
            flip(Mask1,Mask1, -1);
            flip(Mask2,Mask2, -1);
            ImShow = Combine2Images(ImIn1, ImIn2);
            OutText += " Flipped\n";
        }
        ui->MesageTextEdit->setText(OutText.c_str());
    }
    ShowImageRegionCombination(showAligned, showContour, "Aligned", ImShow, Mask1, Mask2);


    int rectHeight = 300;
    int rectWidth = 100;
    int rectLCX = ImIn1.cols/2 - rectWidth/2;
    int rectLCY = ImIn1.rows/2 - rectHeight/2;

    Mat Mask1a = Mat::zeros(maxY,maxX,Mask1.type());
    Mat Mask2a = Mat::zeros(maxY,maxX,Mask1.type());
    //rectangle(Mask1a,Rect(rectLCX,rectLCY,rectWidth,rectHeight),1,-1);
    //rectangle(Mask2a,Rect(rectLCX,rectLCY,rectWidth,rectHeight),1,-1);

    int ellipseSizeX = (maxPosX-minPosX)/4;
    int ellipseSizeY = (maxPosY-minPosY)/4;

    if(ellipseSizeX < 0)
        ellipseSizeX = 0;
    if(ellipseSizeY < 0)
        ellipseSizeY = 0;


    ellipse(Mask1a, Point(maxX/2,maxY/2),Size(ellipseSizeX,ellipseSizeY),0,0,360,1,-1);
    ellipse(Mask2a, Point(maxX/2,maxY/2),Size(ellipseSizeX,ellipseSizeY),0,0,360,1,-1);
    //ShowImageRegionCombination(showFitted, showContour, "Fitted", ImShow, Mask1a, Mask2a);




    // find valey(znajdź bruzdke)
    Mat ImGray1, ImGray2;
    cvtColor(ImIn1,ImGray1,CV_BGR2GRAY);
    cvtColor(ImIn2,ImGray2,CV_BGR2GRAY);

    blur(ImGray2,ImGray2,Size(5,5));

    Mat ImGradient1 = HorizontalGradientDown(ImGray1);
    Mat ImGradient2 = HorizontalGradientDown(ImGray2);

    //blur(ImGradient2,ImGradient2,Size(5,5));
    Mat ImShowGradient = Combine2Images(ShowImageF32PseudoColor(ImGradient1, 0.0, 100.0), ShowImageF32PseudoColor(ImGradient2, 0.0, 100.0));

    Mat Mask1b,Mask2b;

    Mask1.copyTo(Mask1b);
    Mask2.copyTo(Mask2b);
/*
    RegionErosion13(Mask1b);
    RegionErosion13(Mask1b);
    RegionErosion13(Mask1b);
    RegionErosion13(Mask2b);
    RegionErosion13(Mask2b);
    RegionErosion13(Mask2b);
*/
    //int kernelSizeX = 41;
    //int kernelSizeY = 41;
    //Mat Kernel = Mat::zeros(kernelSizeY,kernelSizeX,CV_8U);

    //ellipse(Kernel, Point(kernelSizeX/2,kernelSizeY/2),Size(kernelSizeX/2,kernelSizeY/2),0,0,360,1,-1);
    //imshow("Temp",Kernel*250);

    //erode(Mask1b,Mask1b,Kernel);
    //erode(Mask2b,Mask2b,Kernel);


    ShowImageRegionCombination(showGradient, showContour, "Gradient", ImShowGradient, Mask1a, Mask2a);

    bool switchImages;

    if(findValey)
    {

        float sum1 = AverageMaskedPixelsF(Mask1a, ImGradient1);
        float sum2 = AverageMaskedPixelsF(Mask2a, ImGradient2);

        OutText += "\n";
        OutText += " Left sum" +  to_string(sum1) + "\n";
        OutText += " Rigth Sum" + to_string(sum2) + "\n";
        ui->MesageTextEdit->setText(OutText.c_str());

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
        ImShow = Combine2Images(ImIn1, ImIn2);


    }
    ShowImageRegionCombination(showOutput, showContour, "Output", ImShow, Mask1, Mask2);


// save output ROI
    vector <MR2DType*> rois;
    int begin[MR2DType::Dimensions];
    int end[MR2DType::Dimensions];
    begin[0] = 0;
    begin[1] = 0;
    end[0] = maxX-1;
    end[1] = maxY-1;
    unsigned short *wMask;
    path FileToSave = OutputDirectory;
    FileToSave.append(FileToOpen.stem().string());


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
    MazdaRoiIO<MR2DType>::Write((FileToSave.string() +"BruzdkaROI.tif").c_str(), &rois, NULL);
    imwrite((FileToSave.string() + "Bruzdka.png").c_str(),ImIn1);
    while(rois.size() > 0)
    {
        delete rois.back();
        rois.pop_back();
    }

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
    MazdaRoiIO<MR2DType>::Write((FileToSave.string() +"GrzbietROI.tif").c_str(), &rois, NULL);
    imwrite((FileToSave.string() + "Grzbiet.png").c_str(),ImIn2);
    while(rois.size() > 0)
    {
        delete rois.back();
        rois.pop_back();
    }

//end save output ROI

    //Mat ImRotated1, ImRotated2, MaskRotated1, MaskRotated2;

/*
    //crop size calculation
    Size smallImageSize;

    smallImageSize.height = (int)(fittedRect1.size.height * 1.2);
    smallImageSize.width = (int)(fittedRect1.size.width * 1.2);

    if(smallImageSize.height < (int)(fittedRect2.size.height * 1.2))
        smallImageSize.height = (int)(fittedRect2.size.height * 1.2);

    if(smallImageSize.width < (int)(fittedRect2.size.width * 1.2))
        smallImageSize.width = (int)(fittedRect2.size.width * 1.2);

    smallImageSize.width = (smallImageSize.width / 8) * 8;
    smallImageSize.height = (smallImageSize.height / 8) * 8;


    RotatedRect alignedRect1 = fittedRect1;
    alignedRect1.angle = 0.0;
    RotatedRect alignedRect2 = fittedRect2;
    alignedRect2.angle = 0.0;

 */
/*
    if(rotateImage)
    {
        Mat RotationMatrix1,RotationMatrix2;

        Mat RegTemp;

        RotationMatrix1 = getRotationMatrix2D(fittedRect1.center,fittedRect1.angle,1.0);
        warpAffine(Mask1,MaskRotated1,RotationMatrix1,Size(ImIn1.cols,ImIn1.rows));

        // find proper direction
        int croppWidth,croppHeight,croppX,croppY;

        croppWidth = alignedRect1.size.width;
        croppHeight = alignedRect1.size.height;
        croppX = (int)alignedRect1.center.x - croppWidth/2;
        if(croppX < 0)
            croppX = 0;
        croppY = (int)alignedRect1.center.y - croppHeight/2;
        if(croppY < 0)
            croppY = 0;

        MaskRotated1(Rect(croppX,croppY,croppWidth,croppHeight)).copyTo(RegTemp);

        int lineCount = 20;
        int pixelCount = RegTemp.cols * lineCount;
        int startLine = RegTemp.rows /4 - lineCount;

        unsigned short *wRegTemp;
        wRegTemp = (unsigned short*)RegTemp.data + RegTemp.cols * startLine;

        int uRegPixelCount = 0;
        for(int i = 0; i< pixelCount; i++)
        {
            if(*wRegTemp)
                uRegPixelCount++;

            wRegTemp++;
        }

        startLine = RegTemp.rows /4 * 3;

        wRegTemp = (unsigned short*)RegTemp.data + RegTemp.cols * startLine;

        int lRegPixelCount = 0;
        for(int i = 0; i< pixelCount; i++)
        {
            if(*wRegTemp)
                lRegPixelCount++;

            wRegTemp++;
        }

        string OutText = "";
        OutText += " uper pix count" +  to_string(uRegPixelCount) + "\n";
        OutText += " lower pix count" + to_string(lRegPixelCount) + "\n";
        ui->MesageTextEdit->setText(OutText.c_str());

        if(uRegPixelCount >= lRegPixelCount)
        {
            RotationMatrix1 = getRotationMatrix2D(fittedRect1.center,fittedRect1.angle,1.0);
            RotationMatrix2 = getRotationMatrix2D(fittedRect2.center,-fittedRect1.angle,1.0);
        }
        else
        {
            RotationMatrix1 = getRotationMatrix2D(fittedRect1.center,fittedRect1.angle + 180,1.0);
            RotationMatrix2 = getRotationMatrix2D(fittedRect2.center,-fittedRect1.angle + 180,1.0);
        }



        warpAffine(ImIn1,ImRotated1,RotationMatrix1,Size(ImIn1.cols,ImIn1.rows));
        warpAffine(Mask1,MaskRotated1,RotationMatrix1,Size(ImIn1.cols,ImIn1.rows));

        warpAffine(ImIn2,ImRotated2,RotationMatrix2,Size(ImIn2.cols,ImIn2.rows));
        warpAffine(Mask2,MaskRotated2,RotationMatrix2,Size(ImIn2.cols,ImIn2.rows));
    }
    else
    {
        ImRotated1 = ImIn1;//Mat::zeros(ImIn1.rows,ImIn1.cols,CV_8UC3);
        ImRotated2 = ImIn2;//Mat::zeros(ImIn2.rows,ImIn2.cols,CV_8UC3);
        MaskRotated1 = Mask1;
        MaskRotated2 = Mask2;
    }

    if(showRotated)
    {
        Point2f vertices[4];
        Mat ImToShow;


        if(!showContour)
        {
            alignedRect1.points(vertices);
            ImRotated1.copyTo(ImToShow);
        }
        else
        {
            alignedRect2.points(vertices);
            ImRotated2.copyTo(ImToShow);
        }
        for (int i = 0; i < 4; i++)
            line(ImToShow, vertices[i], vertices[(i+1)%4], Scalar(0,255,0));

        imshow("Rotated", ImToShow);
    }



    Mat ImCropped, ImCropped1, ImCropped2, MaskCropped, MaskCropped1, MaskCropped2;

    int croppWidth = ImIn1.cols;
    int croppHeight = ImIn1.rows;
    int croppX = 50;
    int croppY = 50;
    if(croppImage)
    {
        //Size croppedImageSize;


        croppWidth = smallImageSize.width;//(((int)(alignedRect1.size.width * 1.2))/8)*8;
        croppHeight = smallImageSize.height;//(((int)(alignedRect1.size.height * 1.2))/8)*8;

        croppX = (int)alignedRect1.center.x - croppWidth/2;
        if(croppX < 0)
            croppX = 0;
        croppY = (int)alignedRect1.center.y - croppHeight/2;
        if(croppY < 0)
            croppY = 0;

        ImRotated1(Rect(croppX,croppY,croppWidth,croppHeight)).copyTo(ImCropped1);
        MaskRotated1(Rect(croppX,croppY,croppWidth,croppHeight)).copyTo(MaskCropped1);


        croppX = (int)alignedRect2.center.x - croppWidth/2;
        if(croppX < 0)
            croppX = 0;
        croppY = (int)alignedRect2.center.y - croppHeight/2;
        if(croppY < 0)
            croppY = 0;

        ImRotated2(Rect(croppX,croppY,croppWidth,croppHeight)).copyTo(ImCropped2);
        MaskRotated2(Rect(croppX,croppY,croppWidth,croppHeight)).copyTo(MaskCropped2);


        ImCropped = Mat::zeros(Size(croppWidth * 2, croppHeight),ImCropped1.type());
        ImCropped1.copyTo(ImCropped(Rect(0, 0, croppWidth, croppHeight)));
        ImCropped2.copyTo(ImCropped(Rect(croppWidth, 0, croppWidth, croppHeight)));


    }
    else
    {
        ImCropped1 = ImRotated1;
        ImCropped2 = ImRotated2;

        ImCropped = Mat::zeros(Size(croppWidth * 2, croppHeight),ImCropped1.type());
        ImCropped1.copyTo(ImCropped(Rect(0, 0, croppWidth, croppHeight)));
        ImCropped2.copyTo(ImCropped(Rect(croppWidth, 0, croppWidth, croppHeight)));


        //MaskCropped = Mask1;
        MaskCropped1 = MaskRotated1;
        MaskCropped2 = MaskRotated2;
    }

    // find valey(bruzdke)
    Mat ImGraySmall1, ImGraySmall2;
    cvtColor(ImCropped1,ImGraySmall1,CV_BGR2GRAY);
    cvtColor(ImCropped2,ImGraySmall2,CV_BGR2GRAY);

    Mat ImCroppedGradient1 = HorizontalGradientDown(ImGraySmall1);
    Mat ImCroppedGradient2 = HorizontalGradientDown(ImGraySmall2);

    Mat TempMask1,TempMask2;

    MaskCropped1.copyTo(TempMask1);
    MaskCropped2.copyTo(TempMask2);

    RegionErosion13(TempMask1);
    RegionErosion13(TempMask1);
    RegionErosion13(TempMask1);
    RegionErosion13(TempMask2);
    RegionErosion13(TempMask2);
    RegionErosion13(TempMask2);

    float sum1 = AverageMaskedPixelsF(TempMask1, ImCroppedGradient1);
    float sum2 = AverageMaskedPixelsF(TempMask2, ImCroppedGradient2);


    if(sum1 < sum2)
        MaskCropped1 = MaskCropped1 * 2;
    else
        MaskCropped2 = MaskCropped2 * 2;

*/

/*
    if(showCropped)
    {
        Mat ImCroppedGradient;
        ImCroppedGradient = Mat::zeros(Size(croppWidth * 2, croppHeight),ImCroppedGradient1.type());
        ImCroppedGradient1.copyTo(ImCroppedGradient(Rect(0, 0, croppWidth, croppHeight)));
        ImCroppedGradient2.copyTo(ImCroppedGradient(Rect(croppWidth, 0, croppWidth, croppHeight)));

        imshow("ImCroppedGradient",ShowImageF32PseudoColor(ImCroppedGradient, 0.0, 100.0));

        MaskCropped = Mat::zeros(Size(croppWidth * 2, croppHeight),MaskCropped1.type());
        MaskCropped1.copyTo(MaskCropped(Rect(0, 0, croppWidth, croppHeight)));
        MaskCropped2.copyTo(MaskCropped(Rect(croppWidth, 0, croppWidth, croppHeight)));

        Mat ContourCropped = GetContour5(MaskCropped);

        imshow("Cropped", ShowSolidRegionOnImage(ContourCropped,ImCropped));
    }
*/


    steady_clock::time_point t2 = steady_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    ui->DurationLineEdit->setText(  QString::number(time_span.count()));


}
//--------------------------------------------------------------------------------------------------
void MainWindow::ShowImageCombination(bool show, string WinName, Mat Im1, Mat Im2)
{
    if(!show)
        return;
    int imMaxX, imMaxY;
    imMaxX = Im1.cols;
    imMaxY = Im1.rows;
    Mat Im = Mat::zeros(Size(imMaxX *2,imMaxY) ,Im1.type());
    Im1.copyTo(Im(Rect(0, 0, imMaxX, imMaxY)));
    Im2.copyTo(Im(Rect(imMaxX, 0, imMaxX, imMaxY)));
    imshow(WinName.c_str(), Im);

}
//--------------------------------------------------------------------------------------------------
void MainWindow::ShowImageRegionCombination(bool show, bool showContour, string WinName, Mat Im1, Mat Im2, Mat Mask1, Mat Mask2)
{
    if(!show)
        return;
    int imMaxX, imMaxY;
    imMaxX = Im1.cols;
    imMaxY = Im1.rows;
    Mat Im = Mat::zeros(Size(imMaxX *2,imMaxY) ,Im1.type());
    Im1.copyTo(Im(Rect(0, 0, imMaxX, imMaxY)));
    Im2.copyTo(Im(Rect(imMaxX, 0, imMaxX, imMaxY)));

    Mat Mask = Mat::zeros(Size(imMaxX *2,imMaxY) ,Mask1.type());
    Mask1.copyTo(Mask(Rect(0, 0, imMaxX, imMaxY)));
    Mask2.copyTo(Mask(Rect(imMaxX, 0, imMaxX, imMaxY)));

    if(showContour)
    {
        Mask = GetContour5(Mask);
    }
    imshow(WinName.c_str(), ShowSolidRegionOnImage(Mask,Im));

}
//--------------------------------------------------------------------------------------------------
void MainWindow::ShowImageRegionCombination(bool show, bool showContour, string WinName, Mat Im, Mat Mask1, Mat Mask2)
{
    if(!show)
        return;
    int imMaxX, imMaxY;
    imMaxX = Mask1.cols;
    imMaxY = Mask1.rows;

    Mat Mask = Mat::zeros(Size(imMaxX *2,imMaxY) ,Mask1.type());
    Mask1.copyTo(Mask(Rect(0, 0, imMaxX, imMaxY)));
    Mask2.copyTo(Mask(Rect(imMaxX, 0, imMaxX, imMaxY)));

    if(showContour)
    {
        Mask = GetContour5(Mask);
    }
    imshow(WinName.c_str(), ShowSolidRegionOnImage(Mask,Im));

}
//--------------------------------------------------------------------------------------------------
void MainWindow::ShowHLinesOnImage(bool show, string WinName, Mat Im, int lineU, int lineCU, int lineCL, int lineL)
{
    if(!show)
        return;


    Mat ImShow;
    Im.copyTo(ImShow);
    line(ImShow,Point(0,lineU),Point(ImShow.cols,lineU),CV_RGB(255,0,0),1);
    line(ImShow,Point(0,lineCU),Point(ImShow.cols,lineCU),CV_RGB(255,0,0),1);
    line(ImShow,Point(0,lineCL),Point(ImShow.cols,lineCL),CV_RGB(0,255,0),1);
    line(ImShow,Point(0,lineL),Point(ImShow.cols,lineL),CV_RGB(0,255,0),1);


    imshow(WinName.c_str(), ImShow);

}
//--------------------------------------------------------------------------------------------------
Mat MainWindow::Combine2Images( Mat Im1, Mat Im2)
{
    int imMaxX, imMaxY;
    imMaxX = Im1.cols;
    imMaxY = Im1.rows;
    Mat Im = Mat::zeros(Size(imMaxX *2,imMaxY) ,Im1.type());
    Im1.copyTo(Im(Rect(0, 0, imMaxX, imMaxY)));
    Im2.copyTo(Im(Rect(imMaxX, 0, imMaxX, imMaxY)));

    return Im;
}
//--------------------------------------------------------------------------------------------------
void MainWindow::OnOffImageWindow(void)
{
    destroyAllWindows();

    if(showInput)
        namedWindow("Input image", displayFlag);

    if(showThesholded)
        namedWindow("Thresholded", displayFlag);

    if(show1stMorphology)
        namedWindow("Morphology1", displayFlag);

    if(showHolesRemoved)
        namedWindow("WithoutHoles", displayFlag);

    if(showMask)
        namedWindow("Mask", displayFlag);

    if(showContourOnInput)
        namedWindow("Superimposed", displayFlag);

    if(showFitted)
        namedWindow("Fitted", displayFlag);

    if(showRotated)
        namedWindow("Rotated", displayFlag);

    if(showAreaForAlign)
        namedWindow("AreaForAlign", displayFlag);

    if(showGradient)
        namedWindow("Gradient", displayFlag);

    //namedWindow("Histogram", WINDOW_AUTOSIZE);
}


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

void MainWindow::on_RemoveSmalRegionscheckBox_stateChanged(int arg1)
{
    removeSmallReg = arg1;
    ProcessImage();
}

void MainWindow::on_MinRegionSizeSpinBox_valueChanged(int arg1)
{
    MinRegSize = arg1;
    ProcessImage();
}

void MainWindow::on_SegmentationComboBox_currentIndexChanged(int index)
{
    segmentType = index;
    ProcessImage();
}

void MainWindow::on_RawRegionMorfology1ComboBox_currentIndexChanged(int index)
{
    rawMorphErosion1 = index;
    ProcessImage();
}

void MainWindow::on_RawRegionMorfology2ComboBox_currentIndexChanged(int index)
{
    rawMorphDilation2 = index;
    ProcessImage();
}

void MainWindow::on_RawRegionMorfology3ComboBox_currentIndexChanged(int index)
{
    rawMorphErosion3 = index;
    ProcessImage();
}

void MainWindow::on_FillHolesCheckBox_toggled(bool checked)
{
    fillHoles = checked;
    ProcessImage();
}

void MainWindow::on_DivideRegionsCheckBox_toggled(bool checked)
{
    divideSeparateReg = checked;
    ProcessImage();
}

void MainWindow::on_RegionMorfology1ComboBox_currentIndexChanged(int index)
{
    regMorphErosion1 = index;
    ProcessImage();
}

void MainWindow::on_RegionMorfology2ComboBox_currentIndexChanged(int index)
{
    regMorphDilation2 = index;
    ProcessImage();
}

void MainWindow::on_RegionMorfology3ComboBox_currentIndexChanged(int index)
{
    regMorphErosion3 = index;
    ProcessImage();
}

void MainWindow::on_ShowInputImageCheckBox_toggled(bool checked)
{
    showInput = checked;
    if(showInput)
        namedWindow("Input image", displayFlag);
    else
        destroyWindow("Input image");
    ProcessImage();
}

void MainWindow::on_ShowThresholdedImgeCheckBox_toggled(bool checked)
{
    showThesholded = checked;
    if(showThesholded)
        namedWindow("Thresholded", displayFlag);
    else
        destroyWindow("Thresholded");
    ProcessImage();
}

void MainWindow::on_ShowAfter1MorphologyCheckBox_toggled(bool checked)
{
    show1stMorphology = checked;
    if(show1stMorphology)
        namedWindow("Morphology1", displayFlag);
    else
        destroyWindow("Morphology1");
    ProcessImage();
}

void MainWindow::on_ShowWithoutHolesCheckBox_toggled(bool checked)
{
    showHolesRemoved = checked;
    if(showHolesRemoved)
        namedWindow("WithoutHoles", displayFlag);
    else
        destroyWindow("WithoutHoles");
    ProcessImage();
}


void MainWindow::on_pushButtonRegex_clicked()
{
    ui->RegexListWidget->clear();
    regex FilePattern(ui->RegexLineEdit->text().toStdString());

    for (directory_entry& FileToProcess : directory_iterator(InputDirectory))
    {
        if (!regex_match(FileToProcess.path().filename().string().c_str(), FilePattern ))
            continue;

        path PathLocal = FileToProcess.path();
        ui->RegexListWidget->addItem(PathLocal.filename().string().c_str());
    }
}

void MainWindow::on_ShowMaskCheckBox_toggled(bool checked)
{
    showMask = checked;
    if(showMask)
        namedWindow("Mask", displayFlag);
    else
        destroyWindow("Mask");

    ProcessImage();
}

void MainWindow::on_ShowContourOnImageCheckBox_toggled(bool checked)
{
    showContourOnInput = checked;

    if(showContourOnInput)
        namedWindow("Superimposed", displayFlag);
    else
        destroyWindow("Superimposed");
    ProcessImage();
}

void MainWindow::on_AllowResizeCheckBox_toggled(bool checked)
{
    if(checked)
        displayFlag = WINDOW_NORMAL;
    else
        displayFlag = WINDOW_AUTOSIZE;

    OnOffImageWindow();
    ProcessImage();
}

void MainWindow::on_RemoveBorderRegionCheckBox_toggled(bool checked)
{
    removeBorderRegion = checked;
    ProcessImage();
}

void MainWindow::on_FitEllipseCheckBox_toggled(bool checked)
{
    fitEllipseToReg = checked;
    ProcessImage();
}

void MainWindow::on_RotateCheckBox_toggled(bool checked)
{
    rotateImage = checked;
    ProcessImage();
}

void MainWindow::on_ShowFitedElipseCheckBox_toggled(bool checked)
{
    showFitted = checked;
    if(showFitted)
        namedWindow("Fitted", displayFlag);
    else
        destroyWindow("Fitted");

    ProcessImage();
}

void MainWindow::on_ShowRotatedCheckBox_toggled(bool checked)
{
    showRotated = checked;
    if(showRotated)
        namedWindow("Rotated", displayFlag);
    else
        destroyWindow("Rotated");

    ProcessImage();
}

void MainWindow::on_CroppCheckBox_toggled(bool checked)
{
    croppImage = checked;
    ProcessImage();
}


void MainWindow::on_ShowComboBox_currentIndexChanged(int index)
{
    if(index)
        showContour = true;
    else
        showContour = false;

    ProcessImage();
}

void MainWindow::on_AlignCheckBox_toggled(bool checked)
{
    alignGrains = checked;
    ProcessImage();
}

void MainWindow::on_ShowAreaForAlignCheckBox_toggled(bool checked)
{
    showAreaForAlign = checked;
    if(showAreaForAlign)
        namedWindow("AreaForAlign", displayFlag);
    else
        destroyWindow("AreaForAlign");

    ProcessImage();
}

void MainWindow::on_ShowAlignedCheckBox_toggled(bool checked)
{
    showAligned = checked;
    if(showAligned)
        namedWindow("Aligned", displayFlag);
    else
        destroyWindow("Aligned");

    ProcessImage();
}


void MainWindow::on_FindValeyCheckBox_toggled(bool checked)
{
    findValey = checked;
    ProcessImage();
}

void MainWindow::on_ShowGradientCheckBox_toggled(bool checked)
{
    showGradient = checked;
    if(showGradient)
        namedWindow("Gradient", displayFlag);
    else
        destroyWindow("Gradient");

    ProcessImage();
}

void MainWindow::on_ShowOutputCheckBox_toggled(bool checked)
{
    showOutput = checked;
    if(showOutput)
        namedWindow("Output", displayFlag);
    else
        destroyWindow("Output");

    ProcessImage();
}

void MainWindow::on_SelOutFolderPushButton_clicked()
{
    QFileDialog dialog(this, "Open Folder");
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setDirectory(InputDirectory.string().c_str());

    //QStringList FileList= dialog.e
    if(dialog.exec())
    {
        OutputDirectory = dialog.directory().path().toStdWString();
    }
    else
         return;

    //InputDirectory = dialog.getExistingDirectory().toStdWString();//  toStdString());
    if (!exists(OutputDirectory))
    {
        QMessageBox msgBox;
        msgBox.setText((OutputDirectory.string()+ " not exists ").c_str());
        msgBox.exec();
        OutputDirectory = "c:\\";
    }
    if (!is_directory(OutputDirectory))
    {
        QMessageBox msgBox;
        msgBox.setText((OutputDirectory.string()+ " This is not a directory path ").c_str());
        msgBox.exec();
        OutputDirectory = "c:\\";
    }
    //Files.clear();
    ui->OutDirectoryLineEdit->setText(QString::fromWCharArray(OutputDirectory.wstring().c_str()));
}

void MainWindow::on_pushButtonConvertAll_clicked()
{
    for (directory_entry& FileToProcess : directory_iterator(InputDirectory))
    {
        //path InPath = FileToProcess.path();

        // check if the filename follows the input regular expression
//		if (!regex_match(InPath.filename().string().c_str(), FilePattern))
//			continue;



        //Mat ImIn = imread(InPath.string(), CV_LOAD_IMAGE_ANYDEPTH);
        //ImIn.release();
        //ImIn = imread(InPath.string(), CV_LOAD_IMAGE_ANYDEPTH);
        // check if it is an image file



        FileToOpen = FileToProcess.path();

        if (!exists(FileToOpen))
            continue;
        ImIn = imread(FileToOpen.string().c_str());
        if (ImIn.empty())
            continue;
        maxX = ImIn.cols/2;
        maxY = ImIn.rows;
        ImIn(Rect(0, 0, maxX, maxY)).copyTo(ImIn1);
        ImIn(Rect(maxX, 0, maxX, maxY)).copyTo(ImIn2);

        threshVal = ui->spinBox->value();

        ProcessImage();
    }
}
