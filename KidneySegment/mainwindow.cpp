#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "DispLib.h"
#include "StringFcLib.h"
#include "gradient.h"
#include "RegionU16Lib.h"

#include "tiffio.h"

#include "mazdaroi.h"
#include "mazdaroiio.h"

typedef MazdaRoi<unsigned int, 2> MR2DType;

using namespace std;
using namespace boost::filesystem;
using namespace boost;
using namespace cv;
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    InputDirectory = "C:/Data/Nerki/SG2_FF01/TiffVol1/";

    frameNr = ui->spinBoxFrameNr->value();

    minShow = ui->spinBoxMinShow->value();
    maxShow = ui->spinBoxMaxShow->value();


    imageShowScale = ui->spinBoxImageScale->value();

    showGray = ui->checkBoxShowGray->checkState();
    minShowGray = ui->spinBoxMinShowGray->value();
    maxShowGray = ui->spinBoxMaxShowGray->value();
    ui->comboBoxShowModeGray->addItem("Image");
    ui->comboBoxShowModeGray->addItem("Image + Region");
    ui->comboBoxShowModeGray->addItem("Image + Contour");
    ui->comboBoxShowModeGray->addItem("Image + TReg");
    ui->comboBoxShowModeGray->addItem("Image + TCont");
    ui->comboBoxShowModeGray->setCurrentIndex(1);
    showModeGray = ui->comboBoxShowModeGray->currentIndex();


    //showRegionOnGray = ui->checkBoxShowRegionOnGray->checkState();
    //showContourOnGray = ui->checkBoxShowContourOnGray->checkState();

    showPseudoColor = ui->checkBoxShowPseudoColor->checkState();
    minShowPseudoColor = ui->spinBoxMinShowPseudoColor->value();
    maxShowPseudoColor = ui->spinBoxMaxShowPseudoColor->value();
    ui->comboBoxShowModePseudoColor->addItem("Image");
    ui->comboBoxShowModePseudoColor->addItem("Image + Region");
    ui->comboBoxShowModePseudoColor->addItem("Image + Contour");
    ui->comboBoxShowModePseudoColor->addItem("Image + TReg");
    ui->comboBoxShowModePseudoColor->addItem("Image + TCont");
    ui->comboBoxShowModePseudoColor->setCurrentIndex(1);
    showModePseudoColor = ui->comboBoxShowModePseudoColor->currentIndex();
    //showRegionOnPseudoColor = ui->checkBoxShowRegionOnPseudoColor->checkState();
    //showContourOnPseudoColor = ui->checkBoxShowContourOnPseudoColor->checkState();

    showGradient = ui->checkBoxShowGradient->checkState();
    minShowGradient = ui->spinBoxMinShowGradient->value();
    maxShowGradient = ui->spinBoxMaxShowGradient->value();
    ui->comboBoxShowModeGradient->addItem("Image");
    ui->comboBoxShowModeGradient->addItem("Image + Region");
    ui->comboBoxShowModeGradient->addItem("Image + Contour");
    ui->comboBoxShowModeGradient->addItem("Image + TReg");
    ui->comboBoxShowModeGradient->addItem("Image + TCont");
    ui->comboBoxShowModeGradient->setCurrentIndex(1);
    showModeGradient = ui->comboBoxShowModeGradient->currentIndex();
    //showRegionOnGradient = ui->checkBoxShowRegionOnGradient->checkState();
    //showContourOnGradient = ui->checkBoxShowContourOnGradient->checkState();

    penSize = ui->spinBoxPenSize->value();

    maxX = 192;
    maxY = 192;

    ui->comboBoxRegioNr->addItem("none");
    ui->comboBoxRegioNr->addItem("KidneyL");
    ui->comboBoxRegioNr->addItem("PelvisL");
    ui->comboBoxRegioNr->addItem("MedulaL");
    ui->comboBoxRegioNr->addItem("KidneyP");
    ui->comboBoxRegioNr->addItem("PelvisP");
    ui->comboBoxRegioNr->addItem("MedulaP");
    ui->comboBoxRegioNr->setCurrentIndex(0);
    regionIndex = 0;

    //displayFlag = CV_WINDOW_NORMAL;

    showPelvis = ui->checkBoxShowPelvis->checkState();
    showCortex = ui->checkBoxShowCortex->checkState();
    showMedula = ui->checkBoxShowMedula->checkState();

    Mask2 = Mat::zeros(maxY,maxX,CV_16U);
    MaskKidney1 = Mat::zeros(maxY,maxX,CV_16U);
    MaskPelvis1 = Mat::zeros(maxY,maxX,CV_16U);
    MaskMedula1 = Mat::zeros(maxY,maxX,CV_16U);
    MaskKidney2 = Mat::zeros(maxY,maxX,CV_16U);
    MaskPelvis2 = Mat::zeros(maxY,maxX,CV_16U);
    MaskMedula2 = Mat::zeros(maxY,maxX,CV_16U);

    transparency = ui->spinBoxTransparency->value();

    moveAll = ui->checkBoxMoveAll->checkState();

    ScaleImages();
    //OnOffImageWindow();
    //setMouseCallback("Input pseudocolor", mouseWrapper, 0);


}
//----------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}
//----------------------------------------------------------------------------------------------------------
void MainWindow::ProcessFile()
{
    string SliceToReplace = "Slice" + ItoStrLZ(frameNr,2);
    string FileToProcessName = regex_replace(CurrentFrame00FileName,regex("Slice00"), SliceToReplace);
    ui->lineEditFileToProcess->setText(FileToProcessName.c_str());
    path FileToOpen = InputDirectory;
    FileToOpen.append(FileToProcessName);
    if(!exists(FileToOpen))
    {
        QMessageBox msgBox;
        msgBox.setText((FileToProcessName + " File not exists" ).c_str());
        msgBox.exec();
        return;
    }
    InIm = imread(FileToOpen.string().c_str(),CV_LOAD_IMAGE_ANYDEPTH);
    if(InIm.empty())
        return;

    maxX = InIm.cols;
    maxY = InIm.rows;
    if(InIm.empty())
        return;
    ImGradient = GradientDown(InIm);

    ROIFile = InputDirectory;
    ROIFile.append("/ROI/" + FileToOpen.stem().string() +".roi");
    if(exists(ROIFile))
    {
        LoadROI(ROIFile);
        ui->lineEditInfo->setText("Valid Roi");
    }
    else
        ui->lineEditInfo->setText("No Roi For The Frame");
    PrepareShowImages();
    ShowImages();


}
//----------------------------------------------------------------------------------------------------------
void MainWindow::PrepareShowImages()
{
    //prepareshowImages
    if(InIm.empty())
        return;
    ImShowGray = ShowImage16Gray(InIm,minShowGray,maxShowGray);
    ImShowPseudoColor = ShowImage16PseudoColor(InIm,minShowPseudoColor,maxShowPseudoColor);
    ImShowGradient = ShowImageF32PseudoColor(ImGradient,minShowGradient,maxShowGradient);
}
//----------------------------------------------------------------------------------------------------------
void MainWindow::ShowImages()
{
    if(InIm.empty())
        return;
    Mat ImShow;

    Mask2.release();
    Mask2 = Mat::zeros(maxY,maxX,CV_16U);
    int maxXY = maxX*maxY;
    if(showCortex)
    {
        unsigned short *wMask2,*wMask;
        wMask2 = (unsigned short *)Mask2.data;
        wMask = (unsigned short *)MaskKidney1.data;
        for(int i = 0; i < maxXY; i ++)
        {
            if(*wMask)
                *wMask2 = 1;
            wMask2++;
            wMask++;
        }
        wMask2 = (unsigned short *)Mask2.data;
        wMask = (unsigned short *)MaskKidney2.data;
        for(int i = 0; i < maxXY; i ++)
        {
            if(*wMask)
                *wMask2 = 5;
            wMask2++;
            wMask++;
        }
    }
    if(showPelvis)
    {
        unsigned short *wMask2,*wMask;
        wMask2 = (unsigned short *)Mask2.data;
        wMask = (unsigned short *)MaskPelvis1.data;
        for(int i = 0; i < maxXY; i ++)
        {
            if(*wMask)
                *wMask2 = 2;
            wMask2++;
            wMask++;
        }
        wMask2 = (unsigned short *)Mask2.data;
        wMask = (unsigned short *)MaskPelvis2.data;
        for(int i = 0; i < maxXY; i ++)
        {
            if(*wMask)
                *wMask2 = 6;
            wMask2++;
            wMask++;
        }
    }
    if(showMedula)
    {
        unsigned short *wMask2,*wMask;
        wMask2 = (unsigned short *)Mask2.data;
        wMask = (unsigned short *)MaskMedula1.data;
        for(int i = 0; i < maxXY; i ++)
        {
            if(*wMask)
                *wMask2 = 4;
            wMask2++;
            wMask++;
        }
        wMask2 = (unsigned short *)Mask2.data;
        wMask = (unsigned short *)MaskMedula2.data;
        for(int i = 0; i < maxXY; i ++)
        {
            if(*wMask)
                *wMask2 = 7;
            wMask2++;
            wMask++;
        }
    }

    if(showGray)
    {
        switch(showModeGray)
        {
        case 1:
            ImShow = ShowSolidRegionOnImage(Mask2,ImShowGray);
            break;
        case 2:
            ImShow = ShowSolidRegionOnImage(GetContour5(Mask2),ImShowGray);
            break;
        case 3:
            ImShow = ShowTransparentRegionOnImage(Mask2,ImShowGray,transparency);
            break;
        case 4:
            ImShow = ShowTransparentRegionOnImage(GetContour5(Mask2),ImShowGray,transparency);
            break;
        default:
            ImShowGray.copyTo(ImShow);
            break;
        }
        ui->widgetImageGray->paintBitmap(ImShow);
        ui->widgetImageGray->repaint();
    }
    if(showPseudoColor)
    {

        switch(showModePseudoColor)
        {
        case 1:
            ImShow = ShowSolidRegionOnImage(Mask2,ImShowPseudoColor);
            break;
        case 2:
            ImShow = ShowSolidRegionOnImage(GetContour5(Mask2),ImShowPseudoColor);
            break;
        case 3:
            ImShow = ShowTransparentRegionOnImage(Mask2,ImShowPseudoColor,transparency);
            break;
        case 4:
            ImShow = ShowTransparentRegionOnImage(GetContour5(Mask2),ImShowPseudoColor,transparency);
            break;
        default:
            ImShowPseudoColor.copyTo(ImShow);
            break;
        }
        ui->widgetImagePseudoColor->paintBitmap(ImShow);
        ui->widgetImagePseudoColor->repaint();
    }
    if(showGradient)
    {
        switch(showModeGradient)
        {
        case 1:
            ImShow = ShowSolidRegionOnImage(Mask2,ImShowGradient);
            break;
        case 2:
            ImShow = ShowSolidRegionOnImage(GetContour5(Mask2),ImShowGradient);
            break;
        case 3:
            ImShow = ShowTransparentRegionOnImage(Mask2,ImShowGradient,transparency);
            break;
        case 4:
            ImShow = ShowTransparentRegionOnImage(GetContour5(Mask2),ImShowGradient,transparency);
            break;
        default:
            ImShowGradient.copyTo(ImShow);
            break;
        }
        ui->widgetImageGradient->paintBitmap(ImShow);
        ui->widgetImageGradient->repaint();
    }

    ImShow = ShowSolidRegionOnImage(Mask2,ShowImage16PseudoColor(InIm,minShow,maxShow));

    ui->widgetImage->paintBitmap(ImShow);
    ui->widgetImage->repaint();

}

//----------------------------------------------------------------------------------------------------------
Mat MainWindow::SegmetU16BetweentwoThresholds(Mat Im, unsigned short threshMin, unsigned short threshMax)
{
    int maxX = Im.cols;
    int maxY = Im.rows;
    int maxXY = maxX * maxY;
    Mat Mask = Mat::zeros(maxY, maxX, CV_16U);

    if(!maxX || !maxY)
        return Mask;
    if(!Im.isContinuous())
        return Mask;


    unsigned short *wMask = (unsigned short *)Mask.data;
    unsigned short *wIm = (unsigned short *)Im.data;
    for (int i = 0;  i < maxXY; i++)
    {
        if(*wIm >= threshMin && *wIm <= threshMax)
           *wMask = 1;

        wMask++;
        wIm++;
    }
    return Mask;
}

//----------------------------------------------------------------------------------------------------------
void MainWindow::PlaceShapeOnImage(Mat Im, int x, int y, int size,unsigned short val)
{
    int maxXm1 = maxX-1;
    int maxYm1 = maxY-1;
    if(y >= 0 && y < maxY && x >= 0 && x < maxY)
        Im.at<unsigned short>(y,x) = val;
    if(size > 1)
    {
        if(y>0)
            Im.at<unsigned short>(y-1,x) = val;
        if(y < maxYm1)
            Im.at<unsigned short>(y+1,x) = val;
        if(x>0)
            Im.at<unsigned short>(y,x-1) = val;
        if(x < maxXm1)
            Im.at<unsigned short>(y,x+1) = val;
    }
    if(size > 2)
    {
        if(y > 0 && x > 0)
            Im.at<unsigned short>(y-1,x-1) = val;
        if(y > 0 && x < maxXm1)
            Im.at<unsigned short>(y-1,x+1) = val;
        if(y < maxYm1 && x > 0)
            Im.at<unsigned short>(y+1,x-1) = val;
        if(y < maxYm1 && x < maxXm1)
            Im.at<unsigned short>(y+1,x+1) = val;
    }
}

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
void MainWindow::OnOffImageWindow(void)
{
    destroyAllWindows();
    int scaledX = maxX*imageShowScale;
    int scaledY = maxY*imageShowScale;
    if(showGray)
    {
        namedWindow("Input gray", displayFlag);
        resizeWindow("Input gray",scaledX,scaledY);
    }
    if(showPseudoColor)
    {
        namedWindow("Input pseudocolor", displayFlag);
        resizeWindow("Input pseudocolor",scaledX,scaledY);
    }

    if(showPseudoColor)
    {
        namedWindow("Mask treshold", displayFlag);
        resizeWindow("Mask treshold",scaledX,scaledY);
    }

    if(showGradient)
    {
        namedWindow("Gradient", displayFlag);
        resizeWindow("Gradient",scaledX,scaledY);
    }
}

//----------------------------------------------------------------------------------------------------------
void MainWindow::on_OpenFolderPushButton_clicked()
{
    QFileDialog dialog(this, "Open Folder");
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setDirectory(InputDirectory.string().c_str());

    if(dialog.exec())
    {
        InputDirectory = dialog.directory().path().toStdWString();
    }
    else
         return;

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

    ui->InputDirLineEdit->setText(QString::fromWCharArray(InputDirectory.wstring().c_str()));
    ui->InFileListWidget->clear();
    for (directory_entry& FileToProcess : directory_iterator(InputDirectory))
    {
        if (FileToProcess.path().extension() != ".tif" && FileToProcess.path().extension() != ".png" )
            continue;
        path PathLocal = FileToProcess.path();

        regex FilePattern(".+Slice00.+");
        if (!regex_match(FileToProcess.path().filename().string().c_str(), FilePattern ))
            continue;

        if (!exists(PathLocal))
        {
            QMessageBox msgBox;
            msgBox.setText((PathLocal.filename().string() + " File not exists" ).c_str());
            msgBox.exec();
            break;
        }
        ui->InFileListWidget->addItem(PathLocal.filename().string().c_str());
    }
}
//----------------------------------------------------------------------------------------------------------
void MainWindow::on_InFileListWidget_currentTextChanged(const QString &currentText)
{
    CurrentFrame00FileName = currentText.toStdString();
    ProcessFile();
}
//----------------------------------------------------------------------------------------------------------
void MainWindow::on_spinBoxFrameNr_valueChanged(int arg1)
{
    frameNr = arg1;
    ProcessFile();
}
//----------------------------------------------------------------------------------------------------------
void MainWindow::ScaleImages()
{
    int scaledX = maxX*imageShowScale;
    int scaledY = maxY*imageShowScale;
    int positionX = 30;
    int positionY = 40;
    positionY += maxY;
    if(positionY<250)
        positionY = 250;
    if(showGray)
    {
        ui->labelMinDispGray->show();
        ui->labelMaxDispGray->show();
        ui->spinBoxMinShowGray->show();
        ui->spinBoxMaxShowGray->show();
        ui->comboBoxShowModeGray->show();
        ui->widgetImageGray->show();

        ui->labelMinDispGray->setGeometry(positionX,positionY-24,52,24);
        ui->spinBoxMinShowGray->setGeometry(positionX+52,positionY-24,64,24);
        ui->labelMaxDispGray->setGeometry(positionX+120,positionY-24,58,24);
        ui->spinBoxMaxShowGray->setGeometry(positionX+178,positionY-24,64,24);
        ui->comboBoxShowModeGray->setGeometry(positionX + 246,positionY-24,138,24);
        ui->widgetImageGray->setGeometry(positionX,positionY,scaledX,scaledY);
        positionX += 10 + scaledX;
    }
    else
    {
        ui->labelMinDispGray->hide();
        ui->labelMaxDispGray->hide();
        ui->spinBoxMinShowGray->hide();
        ui->spinBoxMaxShowGray->hide();
        ui->comboBoxShowModeGray->hide();
        ui->widgetImageGray->hide();
    }
    if(showPseudoColor)
    {
        ui->labelMinDispPseudoColor->show();
        ui->labelMaxDispPseudoColor->show();
        ui->spinBoxMinShowPseudoColor->show();
        ui->spinBoxMaxShowPseudoColor->show();
        ui->comboBoxShowModePseudoColor->show();
        ui->widgetImagePseudoColor->show();

        ui->labelMinDispPseudoColor->setGeometry(positionX,positionY-24,52,24);
        ui->spinBoxMinShowPseudoColor->setGeometry(positionX+52,positionY-24,64,24);
        ui->labelMaxDispPseudoColor->setGeometry(positionX+120,positionY-24,58,24);
        ui->spinBoxMaxShowPseudoColor->setGeometry(positionX+178,positionY-24,64,24);
        ui->comboBoxShowModePseudoColor->setGeometry(positionX + 246,positionY-24,138,24);
        ui->widgetImagePseudoColor->setGeometry(positionX,positionY,scaledX,scaledY);
        positionX += 10 + scaledX;

    }
    else
    {
        ui->labelMinDispPseudoColor->hide();
        ui->labelMaxDispPseudoColor->hide();
        ui->spinBoxMinShowPseudoColor->hide();
        ui->spinBoxMaxShowPseudoColor->hide();
        ui->comboBoxShowModePseudoColor->hide();
        ui->widgetImagePseudoColor->hide();
    }
    if(showGradient)
    {
        ui->labelMinDispGradient->show();
        ui->labelMaxDispGradient->show();
        ui->spinBoxMinShowGradient->show();
        ui->spinBoxMaxShowGradient->show();
        ui->comboBoxShowModeGradient->show();
        ui->widgetImageGradient->show();

        ui->labelMinDispGradient->setGeometry(positionX,positionY-24,52,24);
        ui->spinBoxMinShowGradient->setGeometry(positionX+52,positionY-24,64,24);
        ui->labelMaxDispGradient->setGeometry(positionX+120,positionY-24,58,24);
        ui->spinBoxMaxShowGradient->setGeometry(positionX+178,positionY-24,64,24);
        ui->comboBoxShowModeGradient->setGeometry(positionX + 246,positionY-24,138,24);
        ui->widgetImageGradient->setGeometry(positionX,positionY,scaledX,scaledY);
        positionX += 10 + scaledX;
    }
    else
    {
        ui->labelMinDispGradient->hide();
        ui->labelMaxDispGradient->hide();
        ui->spinBoxMinShowGradient->hide();
        ui->spinBoxMaxShowGradient->hide();
        ui->comboBoxShowModeGradient->hide();
        ui->widgetImageGradient->hide();
    }


    ui->widgetImage->setGeometry(900,10,maxX,maxY);
    //ProcessFile();

}

//----------------------------------------------------------------------------------------------------------
void MainWindow::LoadROI(boost::filesystem::path InputFile)
{

    unsigned short *wMask;
    vector <MR2DType*> ROIVect;

    if(!exists(InputFile))
        return;

    ROIVect = MazdaRoiIO<MR2DType>::Read(InputFile.string());

    unsigned int imSize[2];
    imSize[0] = MaskKidney1.rows;
    imSize[1] = MaskKidney1.cols;

    MazdaRoiResizer<MR2DType> resizer;
/*
    MR2DType *ROICortexL = resizer.Upsize(ROIVect.at(0),imSize);
    MazdaRoiIterator<MR2DType> iteratorCL(ROICortexL);
    wMask = (unsigned short*)MaskKidney1.data;
    while(! iteratorCL.IsBehind())
    {
        if (iteratorCL.GetPixel())
            *wMask = 1;
        else
            *wMask = 0;
        ++iteratorCL;
        wMask++;
    }
    delete ROICortexL;
*/
    MR2DType *ROIKidneyL = resizer.Upsize(ROIVect.at(0),imSize);
    MazdaRoiIterator<MR2DType> iteratorKL(ROIKidneyL);
    wMask = (unsigned short*)MaskKidney1.data;
    while(! iteratorKL.IsBehind())
    {
        if (iteratorKL.GetPixel())
            *wMask = 1;
        else
            *wMask = 0;
        ++iteratorKL;
        wMask++;
    }
    delete ROIKidneyL;

    MR2DType *ROIMedulaL = resizer.Upsize(ROIVect.at(1),imSize);
    MazdaRoiIterator<MR2DType> iteratorML(ROIMedulaL);
    wMask = (unsigned short*)MaskMedula1.data;
    while(! iteratorML.IsBehind())
    {
        if (iteratorML.GetPixel())
            *wMask = 1;
        else
            *wMask = 0;
        ++iteratorML;
        wMask++;
    }
    delete ROIMedulaL;

    MR2DType *ROIPelvisL = resizer.Upsize(ROIVect.at(2),imSize);
    MazdaRoiIterator<MR2DType> iteratorPL(ROIPelvisL);
    wMask = (unsigned short*)MaskPelvis1.data;
    while(! iteratorPL.IsBehind())
    {
        if (iteratorPL.GetPixel())
            *wMask = 1;
        else
            *wMask = 0;
        ++iteratorPL;
        wMask++;
    }
    delete ROIPelvisL;

    MR2DType *ROIKidneyP = resizer.Upsize(ROIVect.at(3),imSize);
    MazdaRoiIterator<MR2DType> iteratorKP(ROIKidneyP);
    wMask = (unsigned short*)MaskKidney2.data;
    while(! iteratorKP.IsBehind())
    {
        if (iteratorKP.GetPixel())
            *wMask = 1;
        else
            *wMask = 0;
        ++iteratorKP;
        wMask++;
    }
    delete ROIKidneyP;

    MR2DType *ROIMedulaP = resizer.Upsize(ROIVect.at(4),imSize);
    MazdaRoiIterator<MR2DType> iteratorMP(ROIMedulaP);
    wMask = (unsigned short*)MaskMedula2.data;
    while(! iteratorMP.IsBehind())
    {
        if (iteratorMP.GetPixel())
            *wMask = 1;
        else
            *wMask = 0;
        ++iteratorMP;
        wMask++;
    }
    delete ROIMedulaP;

    MR2DType *ROIPelvisP = resizer.Upsize(ROIVect.at(5),imSize);
    MazdaRoiIterator<MR2DType> iteratorPP(ROIPelvisP);
    wMask = (unsigned short*)MaskPelvis2.data;
    while(! iteratorPP.IsBehind())
    {
        if (iteratorPP.GetPixel())
            *wMask = 1;
        else
            *wMask = 0;
        ++iteratorPP;
        wMask++;
    }
    delete ROIPelvisP;


    while(ROIVect.size() > 0)
    {
         delete ROIVect.back();
         ROIVect.pop_back();
    }

}

//----------------------------------------------------------------------------------------------------------
void MainWindow::SaveROI(boost::filesystem::path OutputFile)
{
    unsigned short *wMask;

    vector <MR2DType*> ROIVect;

    int begin[MR2DType::Dimensions];
    int end[MR2DType::Dimensions];

    //Mask 1
    begin[0] = 0;
    begin[1] = 0;
    end[0] = MaskKidney1.cols-1;
    end[1] = MaskKidney1.rows-1;
/*
    MR2DType *ROICortexL;
    ROICortexL = new MR2DType(begin, end);

    MazdaRoiIterator<MR2DType> iteratorCL(ROICortexL);
    wMask = (unsigned short*)MaskKidney1.data;
    while(! iteratorCL.IsBehind())
    {
        if (*wMask)
            iteratorCL.SetPixel();
        ++iteratorCL;
       wMask++;
    }

    ROICortexL->SetName("KidneyL");
    ROICortexL->SetColor(0xff0000);

    ROIVect.push_back(ROICortexL);
*/
    MR2DType *ROIKidneyL;
    ROIKidneyL = new MR2DType(begin, end);

    MazdaRoiIterator<MR2DType> iteratorKL(ROIKidneyL);
    wMask = (unsigned short*)MaskKidney1.data;
    while(! iteratorKL.IsBehind())
    {
        if (*wMask)
            iteratorKL.SetPixel();
        ++iteratorKL;
        wMask++;
    }

    ROIKidneyL->SetName("KidneyL");
    ROIKidneyL->SetColor(0xff0000);

    ROIVect.push_back(ROIKidneyL);


    MR2DType *ROIMedulaL;
    ROIMedulaL = new MR2DType(begin, end);

    MazdaRoiIterator<MR2DType> iteratorML(ROIMedulaL);
    wMask = (unsigned short*)MaskMedula1.data;
    while(! iteratorML.IsBehind())
    {
        if (*wMask)
            iteratorML.SetPixel();
        ++iteratorML;
        wMask++;
    }

    ROIMedulaL->SetName("MedulaL");
    ROIMedulaL->SetColor(0x00ff00);

    ROIVect.push_back(ROIMedulaL);


    MR2DType *ROIPelvisL;
    ROIPelvisL = new MR2DType(begin, end);

    MazdaRoiIterator<MR2DType> iteratorPL(ROIPelvisL);
    wMask = (unsigned short*)MaskPelvis1.data;
    while(! iteratorPL.IsBehind())
    {
        if (*wMask)
            iteratorPL.SetPixel();
        ++iteratorPL;
        wMask++;
    }

    ROIPelvisL->SetName("PelvisL");
    ROIPelvisL->SetColor(0x0000ff);

    ROIVect.push_back(ROIPelvisL);

/*
    MR2DType *ROICortexP;
    ROICortexP = new MR2DType(begin, end);

    MazdaRoiIterator<MR2DType> iteratorCP(ROICortexP);
    wMask = (unsigned short*)MaskKidney2.data;
    while(! iteratorCP.IsBehind())
    {
        if (*wMask)
            iteratorCP.SetPixel();
        ++iteratorCP;
       wMask++;
    }

    ROICortexP->SetName("KidneyP");
    ROICortexP->SetColor(0xffff00);

    ROIVect.push_back(ROICortexP);
*/
    MR2DType *ROIKidneyR;
    ROIKidneyR = new MR2DType(begin, end);

    MazdaRoiIterator<MR2DType> iteratorKR(ROIKidneyR);
    wMask = (unsigned short*)MaskKidney2.data;
    while(! iteratorKR.IsBehind())
    {
        if (*wMask)
            iteratorKR.SetPixel();
        ++iteratorKR;
        wMask++;
    }

    ROIKidneyR->SetName("KidneyR");
    ROIKidneyR->SetColor(0xffff00);

    ROIVect.push_back(ROIKidneyR);

    MR2DType *ROIMedulaR;
    ROIMedulaR = new MR2DType(begin, end);

    MazdaRoiIterator<MR2DType> iteratorMR(ROIMedulaR);
    wMask = (unsigned short*)MaskMedula2.data;
    while(! iteratorMR.IsBehind())
    {
        if (*wMask)
            iteratorMR.SetPixel();
        ++iteratorMR;
        wMask++;
    }

    ROIMedulaR->SetName("MedulaR");
    ROIMedulaR->SetColor(0x00ffff);

    ROIVect.push_back(ROIMedulaR);


    MR2DType *ROIPelvisR;
    ROIPelvisR = new MR2DType(begin, end);

    MazdaRoiIterator<MR2DType> iteratorPR(ROIPelvisR);
    wMask = (unsigned short*)MaskPelvis2.data;
    while(! iteratorPR.IsBehind())
    {
        if (*wMask)
            iteratorPR.SetPixel();
        ++iteratorPR;
        wMask++;
    }

    ROIPelvisR->SetName("PelvisR");
    ROIPelvisR->SetColor(0xff00ff);

    ROIVect.push_back(ROIPelvisR);


    MR2DType *ROICortexL;
    ROICortexL = new MR2DType(begin, end);

    unsigned short* wMaskM;
    unsigned short* wMaskP;

    MazdaRoiIterator<MR2DType> iteratorCL(ROICortexL);
    wMask = (unsigned short*)MaskKidney1.data;
    wMaskM = (unsigned short*)MaskMedula1.data;
    wMaskP = (unsigned short*)MaskPelvis1.data;
    while(! iteratorCL.IsBehind())
    {
        if (*wMask && !*wMaskM && !*wMaskP)
            iteratorCL.SetPixel();
        ++iteratorCL;
        wMask++;
        wMaskM++;
        wMaskP++;
    }

    ROICortexL->SetName("KidneyL");
    ROICortexL->SetColor(0x8F0000);

    ROIVect.push_back(ROICortexL);


    MR2DType *ROICortexR;
    ROICortexR = new MR2DType(begin, end);

    MazdaRoiIterator<MR2DType> iteratorCR(ROICortexR);
    wMask = (unsigned short*)MaskKidney2.data;
    wMaskM = (unsigned short*)MaskMedula2.data;
    wMaskP = (unsigned short*)MaskPelvis2.data;
    while(! iteratorCR.IsBehind())
    {
        if (*wMask && !*wMaskM && !*wMaskP)
            iteratorCR.SetPixel();
        ++iteratorCR;
        wMask++;
        wMaskM++;
        wMaskP++;
    }

    ROICortexR->SetName("KidneyR");
    ROICortexR->SetColor(0x8f8f00);

    ROIVect.push_back(ROICortexR);

    MazdaRoiIO<MR2DType>::Write(OutputFile.string(), &ROIVect, NULL);
    //MazdaRoiIO<MR2DType>::Write((FileToSave.string() +"GrzbietROI.tif").c_str(), &RoiVect, NULL);

    while(ROIVect.size() > 0)
    {
         delete ROIVect.back();
         ROIVect.pop_back();
    }
}

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------

void MainWindow::on_spinBoxMinShow_valueChanged(int arg1)
{
    minShow = arg1;
    ProcessFile();
}

void MainWindow::on_spinBoxMaxShow_valueChanged(int arg1)
{
    maxShow = arg1;
    ProcessFile();
}

void MainWindow::on_spinBoxImageScale_valueChanged(int arg1)
{
    imageShowScale = arg1;
    ScaleImages();
    //ProcessFile();
}

void MainWindow::on_checkBoxShowGray_toggled(bool checked)
{
    showGray = checked;
    ScaleImages();
    //ProcessFile();
}

void MainWindow::on_checkBoxShowPseudoColor_toggled(bool checked)
{
    showPseudoColor = checked;
    ScaleImages();
}

void MainWindow::on_checkBoxShowGradient_toggled(bool checked)
{
    showGradient = checked;
    ScaleImages();
    //ProcessFile();
}

void MainWindow::on_pushButtonGetPixelValues_clicked()
{

}

void MainWindow::on_widgetImage_mousePressed(QPoint point)
{
    //ui->spinBoxValGradient->setValue(point.x());
    //ui->spinBoxValIntensity->setValue(point.y());
}


void MainWindow::on_widgetImage_mouseMoved(QPoint point, int butPressed)
{
    int x = point.x()/imageShowScale;
    int y = point.y()/imageShowScale;

    //if(x < 0 || y < 0 || x >= maxX - 1 || y > = maxY - 1)
    //    return;

    ui->spinBoxValGradient->setValue(x);
    ui->spinBoxValIntensity->setValue(y);

    ui->spinBoxButton->setValue(butPressed);

    if(butPressed == 1)
    {
        switch(regionIndex)
        {
        case 1:
            PlaceShapeOnImage(MaskKidney1, x, y, penSize,1);
            //MaskKidney1.at<unsigned short>(y,x) = 1;
            break;
        case 2:
            PlaceShapeOnImage(MaskPelvis1, x, y, penSize,1);
            break;
        case 3:
            PlaceShapeOnImage(MaskMedula1, x, y, penSize,1);
            break;
        case 4:
            PlaceShapeOnImage(MaskKidney2, x, y, penSize,1);
            break;
        case 5:
            PlaceShapeOnImage(MaskPelvis2, x, y, penSize,1);
            break;
        case 6:
            PlaceShapeOnImage(MaskMedula2, x, y, penSize,1);
            break;

        default:
            break;
        }
    }
    if(butPressed & 0x2)
    {
        switch(regionIndex)
        {
        case 1:
            PlaceShapeOnImage(MaskKidney1, x, y, penSize,0);
            //MaskKidney1.at<unsigned short>(y,x) = 1;
            break;
        case 2:
            PlaceShapeOnImage(MaskPelvis1, x, y, penSize,0);
            break;
        case 3:
            PlaceShapeOnImage(MaskMedula1, x, y, penSize,0);
            break;
        case 4:
            PlaceShapeOnImage(MaskKidney2, x, y, penSize,0);
            break;
        case 5:
            PlaceShapeOnImage(MaskPelvis2, x, y, penSize,0);
            break;
        case 6:
            PlaceShapeOnImage(MaskMedula2, x, y, penSize,0);
            break;

        default:
            break;
        }
    }

    ShowImages();
}

//########################################################################################

void MainWindow::on_comboBoxRegioNr_currentIndexChanged(int index)
{
    regionIndex = index;
}



void MainWindow::on_spinBoxMinShowGray_valueChanged(int arg1)
{
    minShowGray = arg1;
    PrepareShowImages();
    ShowImages();
}

void MainWindow::on_spinBoxMaxShowGray_valueChanged(int arg1)
{
    maxShowGray = arg1;
    PrepareShowImages();
    ShowImages();
}

void MainWindow::on_spinBoxMinShowPseudoColor_valueChanged(int arg1)
{
    minShowPseudoColor = arg1;
    PrepareShowImages();
    ShowImages();
}

void MainWindow::on_spinBoxMaxShowPseudoColor_valueChanged(int arg1)
{
    maxShowPseudoColor = arg1;
    PrepareShowImages();
    ShowImages();
}

void MainWindow::on_spinBoxMinShowGradient_valueChanged(int arg1)
{
    minShowGradient = arg1;
    PrepareShowImages();
    ShowImages();
}

void MainWindow::on_spinBoxMaxShowGradient_valueChanged(int arg1)
{
    maxShowGradient = arg1;
    PrepareShowImages();
    ShowImages();
}

void MainWindow::on_comboBoxShowModeGray_currentIndexChanged(int index)
{
    showModeGray = index;
    ShowImages();
}

void MainWindow::on_comboBoxShowModePseudoColor_currentIndexChanged(int index)
{
    showModePseudoColor = index;
    ShowImages();
}

void MainWindow::on_comboBoxShowModeGradient_currentIndexChanged(int index)
{
    showModeGradient = index;
    ShowImages();
}

void MainWindow::on_pushButtonFillHoles_pressed()
{
    switch(regionIndex)
    {
    case 1:
        FillBorderWithValue(MaskKidney1, 0xFFFF);
        OneRegionFill5Fast1(MaskKidney1,  0xFFFF);
        FillHoles(MaskKidney1);
        DeleteRegionFromImage(MaskKidney1, 0xFFFF);
        break;
    case 2:
        FillBorderWithValue(MaskPelvis1, 0xFFFF);
        OneRegionFill5Fast1(MaskPelvis1,  0xFFFF);
        FillHoles(MaskPelvis1);
        DeleteRegionFromImage(MaskPelvis1, 0xFFFF);
        break;
    case 3:
        FillBorderWithValue(MaskMedula1, 0xFFFF);
        OneRegionFill5Fast1(MaskMedula1,  0xFFFF);
        FillHoles(MaskMedula1);
        DeleteRegionFromImage(MaskMedula1, 0xFFFF);
        break;
    case 4:
        FillBorderWithValue(MaskKidney2, 0xFFFF);
        OneRegionFill5Fast1(MaskKidney2,  0xFFFF);
        FillHoles(MaskKidney2);
        DeleteRegionFromImage(MaskKidney2, 0xFFFF);
        break;
    case 5:
        FillBorderWithValue(MaskPelvis2, 0xFFFF);
        OneRegionFill5Fast1(MaskPelvis2,  0xFFFF);
        FillHoles(MaskPelvis2);
        DeleteRegionFromImage(MaskPelvis2, 0xFFFF);
        break;
    case 6:
        FillBorderWithValue(MaskMedula2, 0xFFFF);
        OneRegionFill5Fast1(MaskMedula2,  0xFFFF);
        FillHoles(MaskMedula2);
        DeleteRegionFromImage(MaskMedula2, 0xFFFF);
        break;
    default:
        break;
    }

    ShowImages();
}

void MainWindow::on_checkBoxShowPelvis_toggled(bool checked)
{
    showPelvis = checked;
    ShowImages();
}

void MainWindow::on_checkBoxShowCortex_toggled(bool checked)
{
    showCortex = checked;
    ShowImages();
}

void MainWindow::on_checkBoxShowMedula_toggled(bool checked)
{
    showMedula = checked;
    ShowImages();
}

void MainWindow::on_pushButtonUp_clicked()
{
    Mat MaskTemp;
    if(regionIndex == 1 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskKidney1(Rect(0, 1, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 0, maxX , maxY -1)));
        MaskTemp.copyTo(MaskKidney1);
    }
    if(regionIndex == 4 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskKidney2(Rect(0, 1, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 0, maxX , maxY -1)));
        MaskTemp.copyTo(MaskKidney2);
    }
    if(regionIndex == 2 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskPelvis1(Rect(0, 1, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 0, maxX , maxY -1)));
        MaskTemp.copyTo(MaskPelvis1);
    }
    if(regionIndex == 5 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskPelvis2(Rect(0, 1, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 0, maxX , maxY -1)));
        MaskTemp.copyTo(MaskPelvis2);
    }
    if(regionIndex == 3 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskMedula1(Rect(0, 1, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 0, maxX , maxY -1)));
        MaskTemp.copyTo(MaskMedula1);
    }
    if(regionIndex == 6 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskMedula2(Rect(0, 1, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 0, maxX , maxY -1)));
        MaskTemp.copyTo(MaskMedula2);
    }
    ShowImages();
}

void MainWindow::on_pushButtonDown_clicked()
{
    Mat MaskTemp;
    if(regionIndex == 1 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskKidney1(Rect(0, 0, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 1, maxX , maxY -1)));
        MaskTemp.copyTo(MaskKidney1);
    }
    if(regionIndex == 4 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskKidney2(Rect(0, 0, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 1, maxX , maxY -1)));
        MaskTemp.copyTo(MaskKidney2);
    }
    if(regionIndex == 2 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskPelvis1(Rect(0, 0, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 1, maxX , maxY -1)));
        MaskTemp.copyTo(MaskPelvis1);
    }
    if(regionIndex == 5 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskPelvis2(Rect(0, 0, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 1, maxX , maxY -1)));
        MaskTemp.copyTo(MaskPelvis2);
    }
    if(regionIndex == 3 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskMedula1(Rect(0, 0, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 1, maxX , maxY -1)));
        MaskTemp.copyTo(MaskMedula1);
    }
    if(regionIndex == 6 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskMedula2(Rect(0, 0, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 1, maxX , maxY -1)));
        MaskTemp.copyTo(MaskMedula2);
    }
    ShowImages();
}

void MainWindow::on_pushButtonRight_clicked()
{
    Mat MaskTemp;
    if(regionIndex == 1 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskKidney1(Rect(0, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(1, 0, maxX  -1, maxY)));
        MaskTemp.copyTo(MaskKidney1);
    }
    if(regionIndex == 4 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskKidney2(Rect(0, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(1, 0, maxX  -1, maxY)));
        MaskTemp.copyTo(MaskKidney2);
    }
    if(regionIndex == 2 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskPelvis1(Rect(0, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(1, 0, maxX  -1, maxY)));
        MaskTemp.copyTo(MaskPelvis1);
    }
    if(regionIndex == 5 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskPelvis2(Rect(0, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(1, 0, maxX  -1, maxY)));
        MaskTemp.copyTo(MaskPelvis2);
    }
    if(regionIndex == 3 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskMedula1(Rect(0, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(1, 0, maxX  -1, maxY)));
        MaskTemp.copyTo(MaskMedula1);
    }
    if(regionIndex == 6 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskMedula2(Rect(0, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(1, 0, maxX  -1, maxY)));
        MaskTemp.copyTo(MaskMedula2);
    }
    ShowImages();
}

void MainWindow::on_pushButtonLeft_clicked()
{
    Mat MaskTemp;

    if(regionIndex == 1 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskKidney1(Rect(1, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(0, 0, maxX  -1, maxY)));
        MaskTemp.copyTo(MaskKidney1);
    }
    if(regionIndex == 4 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskKidney2(Rect(1, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(0, 0, maxX  -1, maxY)));
        MaskTemp.copyTo(MaskKidney2);
    }
    if(regionIndex == 2 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskPelvis1(Rect(1, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(0, 0, maxX  -1, maxY)));
        MaskTemp.copyTo(MaskPelvis1);
    }
    if(regionIndex == 5 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskPelvis2(Rect(1, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(0, 0, maxX  -1, maxY)));
        MaskTemp.copyTo(MaskPelvis2);
    }
    if(regionIndex == 3 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskMedula1(Rect(1, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(0, 0, maxX  -1, maxY)));
        MaskTemp.copyTo(MaskMedula1);
    }
    if(regionIndex == 6 || moveAll)
    {
        MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
        MaskMedula2(Rect(1, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(0, 0, maxX  -1, maxY)));
        MaskTemp.copyTo(MaskMedula2);
    }
    ShowImages();
}

void MainWindow::on_spinBoxPenSize_valueChanged(int arg1)
{
   penSize = arg1;
}

void MainWindow::on_spinBoxTransparency_valueChanged(int arg1)
{
    transparency = arg1;
    ShowImages();
}

void MainWindow::on_pushButtonSaveRoiCommon_clicked()
{
    path OutputFile = InputDirectory;
    OutputFile.append("/ROI/common"+ItoStrLZ(frameNr,2)+".tif");
    SaveROI(OutputFile);

}

void MainWindow::on_pushButtonLoadRoiCommon_clicked()
{
    path InputFile = InputDirectory;
    InputFile.append("/ROI/common"+ItoStrLZ(frameNr,2)+".tif");
    LoadROI(InputFile);
    ShowImages();
}

void MainWindow::on_pushButtonDeleteReg_clicked()
{
    switch(regionIndex)
    {
    case 1:
        DeleteRegionFromImage(MaskKidney1, 1);
        break;
    case 2:
        DeleteRegionFromImage(MaskPelvis1, 1);
        break;
    case 3:
        DeleteRegionFromImage(MaskMedula1, 1);
        break;
    case 4:
        DeleteRegionFromImage(MaskKidney2, 1);
        break;
    case 5:
        DeleteRegionFromImage(MaskPelvis2, 1);
        break;
    case 6:
        DeleteRegionFromImage(MaskMedula2, 1);
        break;
    default:
        break;
    }

    ShowImages();
}

void MainWindow::on_pushButtonLoadCommonP_clicked()
{
    path InputFile = InputDirectory;
    int newFrameNr = frameNr + 1;
    if (newFrameNr > 30)
        newFrameNr = 30;
    InputFile.append("/ROI/common"+ItoStrLZ(newFrameNr,2)+".tif");
    LoadROI(InputFile);
    ShowImages();
}

void MainWindow::on_pushButtonLoadCommonM_clicked(bool checked)
{
    path InputFile = InputDirectory;
    int newFrameNr = frameNr - 1;
    if (newFrameNr < 0)
        newFrameNr = 0;
    InputFile.append("/ROI/common"+ItoStrLZ(newFrameNr,2)+".tif");
    LoadROI(InputFile);
    ShowImages();
}

void MainWindow::on_pushButtonDeleteAll_clicked()
{
    DeleteRegionFromImage(MaskKidney1, 1);
    DeleteRegionFromImage(MaskPelvis1, 1);
    DeleteRegionFromImage(MaskMedula1, 1);
    DeleteRegionFromImage(MaskKidney2, 1);
    DeleteRegionFromImage(MaskPelvis2, 1);
    DeleteRegionFromImage(MaskMedula2, 1);
    ShowImages();
}

void MainWindow::on_pushButtonSaveRoi_clicked()
{
     SaveROI(ROIFile);
}

void MainWindow::on_checkBoxMoveAll_toggled(bool checked)
{
    moveAll = checked;
}
