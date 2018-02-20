#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <string>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "DispLib.h"
#include "gradient.h"
#include "RegionU16Lib.h"

using namespace std;
using namespace boost::filesystem;
using namespace boost;
using namespace cv;

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
cv::Mat CalculateSDAL(cv::Mat ImIn, cv::Mat Roi, int radius)
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
void MainWindow::MaskImage(void)
{
    if(ImIn.empty())
        return;
    int maxX = ImIn.cols;
    int maxY = ImIn.rows;
    int maxXY = maxX * maxY;

    Mat ImShowGray;
        //if(showInputGray)
    if(showInputGray)
        ImShowGray = ShowImage16Gray(ImIn,minShowGray,maxShowGray);

    if(showInputGray)
        imshow("Gray", ImShowGray);

    Mat ImShowPseudocolor;
    if(showInputPseudocolor||showMask)
        ImShowPseudocolor = ShowImage16PseudoColor(ImIn,minShowPseudocolor,maxShowPseudocolor);
    if(showInputPseudocolor)
        imshow("Pseudocolor", ImShowPseudocolor);
    //ImGradient = GradientDown(ImIn);
    ImGradient = GradientMorph(ImIn, 4);

    if(showGradient)
        imshow("Gradient", ShowImage16PseudoColor(ImGradient,minShowGradient,maxShowGradient));

    Mask = Threshold16(ImIn, thresholdImOrg);
    Threshold16(ImGradient, Mask, thresholdGradient);

    if(showThresholded)
    {
        Mat ImShowMask;
        if(showContour)
            ImShowMask = ShowSolidRegionOnImageInBlack(GetContour5(Mask), ImShowPseudocolor);
        else
            ImShowMask = ShowSolidRegionOnImageInBlack(Mask, ImShowPseudocolor);
        imshow("Thresholded", ImShowMask);
    }

    if(closingShape)
    {
        DilationCV(Mask, closingShape);
        ErosionCV(Mask, closingShape);
    }

    if(fillHoles)
    {
        FillBorderWithValue(Mask, 0xFFFF);
        OneRegionFill5Fast1(Mask,  0xFFFF);
        FillHoles(Mask);
        DeleteRegionFromImage(Mask, 0xFFFF);
    }

    if(divideSeparateRegions)
    {
        DivideSeparateRegions(Mask, minRegionSize);
    }
    if(erosionShape)
        ErosionCV(Mask, erosionShape);

    if(0)//(expandMask)
    {
        Mat Kernel = Mat::ones(1,expansionSize*2+1,CV_8U);
        Mat MaskTemp;
        dilate(Mask,MaskTemp,Kernel);

        Mask = MaskTemp-Mask;

    }
    if(expandMaskY)
    {
        Mat Kernel = Mat::zeros(expansionSize + 1,expansionSize * 2 + 1, CV_8U);
        ellipse(Kernel, Point(expansionSize, expansionSize),
            Size(expansionSize, expansionSize), 0.0, 180.0, 360.0,
            1, -1);

        imshow("kernel",Kernel * 255);
        Mat MaskTemp;
        dilate(Mask,MaskTemp,Kernel,Point(expansionSize, expansionSize));

        Mask = MaskTemp-Mask;

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

    if(showMask)
    {
        Mat ImShowMask;
        if(showContour)
            ImShowMask = ShowSolidRegionOnImageInBlack(GetContour5(Mask), ImShowPseudocolor);
        else
            ImShowMask = ShowSolidRegionOnImageInBlack(Mask, ImShowPseudocolor);
        imshow("Mask", ImShowMask);
    }

    ImSDA = CalculateSDAL(ImIn, Mask, kernelSizeSDA);

    //CalculateSDA();
    if(showSDA)
    {
        imshow("SDA", ShowImage16PseudoColor(ImSDA,minShowSDA,maxShowSDA));
    }

    return;

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

        for(int i = 0; i < maxXY; i++)
        {

            if(!*wMaskSDA)
                *wMask = 0;
            wMask++;
            wMaskSDA++;
        }


        //Mask = Mask * MaskSDA;
    }
    if(postErosionShape1)
        ErosionCV(Mask, postErosionShape1);
    if(postDilationShape2)
        DilationCV(Mask, postDilationShape2);
    if(postErosionShape3)
        ErosionCV(Mask, postErosionShape3);
    if(fillHolesOnOutMask)
    {
        FillBorderWithValue(Mask, 0xFFFF);
        OneRegionFill5Fast1(Mask,  0xFFFF);
        FillHoles(Mask);
        DeleteRegionFromImage(Mask, 0xFFFF);
    }
    ShowImages();
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
void MainWindow::ShowImages(void)
{
    if(ImIn.empty())
        return;
    Mat ImShowGray;
    //if(showInputGray)
        ImShowGray = ShowImage16Gray(ImIn,minShowGray,maxShowGray);

    if(showInputGray)
    {

        imshow("Gray", ImShowGray);
    }
    Mat ImShowPseudocolor;
    if(showInputPseudocolor||showMask)
        ImShowPseudocolor = ShowImage16PseudoColor(ImIn,minShowPseudocolor,maxShowPseudocolor);
    if(showInputPseudocolor)
    {

        imshow("Pseudocolor", ImShowPseudocolor);
    }
    if(showGradient)
    {
        //Mat ImShowGradient = ShowImageF32PseudoColor(ImGradient,minShowGradient,maxShowGradient);
        Mat ImShowGradient = ShowImage16PseudoColor(ImGradient,minShowGradient,maxShowGradient);
        imshow("Gradient", ImShowGradient);
    }
    if(showMask)
    {
        Mat ImShowMask;
        if(showContour)
            ImShowMask = ShowSolidRegionOnImageInBlack(GetContour5(Mask), ImShowPseudocolor);
        else
            ImShowMask = ShowSolidRegionOnImageInBlack(Mask, ImShowPseudocolor);
        imshow("Mask", ImShowMask);
    }
    if(showConv && !ImSDA.empty())
    {
        Mat ImShowConverted;

        ImShowConverted = ShowImage16PseudoColor(ImSDA,minShowSDA,maxShowSDA);
        if(!MaskSDA.empty()&& thresholdSDA)
        {
            if(showContour)
                ImShowConverted = ShowSolidRegionOnImageInBlack(GetContour5(MaskSDA), ImShowConverted);
            else
                ImShowConverted = ShowSolidRegionOnImageInBlack(MaskSDA, ImShowConverted);
        }


        imshow("SDA", ImShowConverted);
    }
    ImOut = ShowTransparentRegionOnImage(Mask,ImShowGray,transparency);
    if(showOutput)
    {
        imshow("Output", ImOut);
    }

}
//--------------------------------------------------------------------------------------------------
void MainWindow::OnOffImageWindow(void)
{
    destroyAllWindows();

    if(showInputGray)
        namedWindow("Gray", displayFlag);

    if(showInputPseudocolor)
        namedWindow("Pseudocolor", displayFlag);

    if(showGradient)
        namedWindow("Gradient", displayFlag);

    if(showConv)
        namedWindow("SDA", displayFlag);

    if(showMask)
        namedWindow("Mask", displayFlag);

    if(showOutput)
        namedWindow("Output", displayFlag);



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

    showInputPseudocolor = ui->CheckBoxShowInputImagePC->checkState();
    showInputGray = ui->CheckBoxShowInputImageGray->checkState();
    showGradient = ui->CheckBoxShowGradient->checkState();
    showThresholded = ui->CheckBoxShowThesholded->checkState();
    showMask = ui->CheckBoxShowMask->checkState();
    showSDA = ui->CheckBoxShowSDA->checkState();
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
    transparency = ui->spinBoxTransparency->value();

    closingShape = ui->spinBoxClosingShape->value();
    erosionShape = ui->spinBoxErosionShape->value();
    postErosionShape1 = ui->spinBoxErosionShape1->value();
    postDilationShape2 = ui->spinBoxDilationShape2->value();
    postErosionShape3 = ui->spinBoxErosionShape3->value();

    fillHolesOnOutMask = ui->CheckBoxFillHolesOnOut->checkState();


    OnOffImageWindow();
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
    for (directory_entry& FileToProcess : directory_iterator(InputDirectory))
    {
        if (FileToProcess.path().extension() != ".tif" && FileToProcess.path().extension() != ".png" )
            continue;
        path PathLocal = FileToProcess.path();

        regex FilePattern(".+crop.+");
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

void MainWindow::on_ListWidgetFiles_currentTextChanged(const QString &currentText)
{
    CurrentFileName = currentText.toStdString();
    string FileName(CurrentFileName);

    FileToOpen = InputDirectory;
    FileToOpen.append(FileName);
    ImIn = imread(FileToOpen.string().c_str(),CV_LOAD_IMAGE_ANYDEPTH);
    //ImGradient = GradientDown(ImIn);
    //ImGradient = GradientMorph(ImIn, 4);


    //FileName = regex_replace(FileName,regex("_crop"),"_SDA");
    //path FileToOpen2 = InputDirectory;
    //FileToOpen2.append(FileName);
    //ImConv = imread(FileToOpen2.string().c_str(),CV_LOAD_IMAGE_ANYDEPTH);

    //CalculateSDA();

    //FileName = regex_replace(FileName,regex("_crop.tif"),"_SDA.bmp");
    //path FileToSaveSDA = InputDirectory;
    //FileToSaveSDA.append(FileName );
    //imwrite(FileToSaveSDA.string().c_str(),ImNormInvSDA);

    MaskImage();
    //ProcessImage();

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
    ShowImages();
}

void MainWindow::on_CheckBoxShowInputImagePC_toggled(bool checked)
{
    showInputPseudocolor = checked;
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
    ShowImages();
}



void MainWindow::on_spinBoxThresholdOryginalImage_valueChanged(int arg1)
{
    thresholdImOrg = (unsigned short)arg1;
    MaskImage();
    //ProcessImage();
}

void MainWindow::on_CheckBoxShowMask_toggled(bool checked)
{
    showMask = checked;
    ShowImages();
}

void MainWindow::on_CheckBoxShowConv_toggled(bool checked)
{
    showConv = checked;
    ShowImages();
}

void MainWindow::on_spinBoxMinShowConv_valueChanged(int arg1)
{
    minShowSDA = arg1;
    ShowImages();
}

void MainWindow::on_spinBoxMaxShowConv_valueChanged(int arg1)
{
    maxShowSDA = arg1;
    ShowImages();
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
    ShowImages();

}

void MainWindow::on_spinBoxThresholdGradient_valueChanged(int arg1)
{
    thresholdGradient = (unsigned short)arg1;
    MaskImage();
}

void MainWindow::on_CheckBoxFillHoles_toggled(bool checked)
{
    fillHoles = checked;
    MaskImage();
}

void MainWindow::on_CheckBoxDivideseparateregions_toggled(bool checked)
{
    divideSeparateRegions = checked;
    MaskImage();
}

void MainWindow::on_spinBoxMinRegionSize_valueChanged(int arg1)
{
    minRegionSize = arg1;
    MaskImage();
}

void MainWindow::on_CheckBoxExpandMask_toggled(bool checked)
{
    expandMask = checked;
    MaskImage();
}

void MainWindow::on_spinBoxExpansionSize_valueChanged(int arg1)
{
    expansionSize = arg1;
    MaskImage();
}

void MainWindow::on_CheckBoxCroppMask_toggled(bool checked)
{
    croppMask = checked;
    MaskImage();
}

void MainWindow::on_spinBoxCroppSize_valueChanged(int arg1)
{
    croppSize = arg1;
    MaskImage();
}

void MainWindow::on_CheckBoxThresholdSDA_toggled(bool checked)
{
    thresholdSDA = checked;
    MaskImage();
}

void MainWindow::on_spinBoxThresholdSDA_valueChanged(int arg1)
{
    thresholdImSDA = arg1;
    MaskImage();
}

void MainWindow::on_CheckBoxCalculateSDA_toggled(bool checked)
{
    calculateSDA = checked;
    //CalculateSDA();
    MaskImage();
}

void MainWindow::on_spinBoxSDAKernelSize_valueChanged(int arg1)
{
    kernelSizeSDA = arg1;
}

void MainWindow::on_CheckBoxShowOutput_toggled(bool checked)
{
    showOutput = checked;
     ShowImages();
}

void MainWindow::on_spinBoxTransparency_valueChanged(int arg1)
{
    transparency = arg1;
     ShowImages();
}

void MainWindow::on_pushButtonSaveOut_clicked()
{
    string FileName(CurrentFileName);

    FileName = regex_replace(FileName,regex("_crop.tif"),"_out.bmp");
    path FileToSave = InputDirectory;
    FileToSave.append(FileName);
    imwrite(FileToSave.string(),ImOut);//ImConv = imread(FileToOpen2.string().c_str(),CV_LOAD_IMAGE_ANYDEPTH);

    string FileNameMask(CurrentFileName);

    FileNameMask = regex_replace(FileNameMask,regex("_crop.tif"),"_mask.bmp");
    path FileToSaveMask = InputDirectory;
    FileToSaveMask.append(FileNameMask);
    imwrite(FileToSaveMask.string(),Mask*255);//ImConv = imread(FileToOpen2.string().c_str(),CV_LOAD_IMAGE_ANYDEPTH);
}

void MainWindow::on_CheckBoxContour_toggled(bool checked)
{
    showContour = checked;
    ShowImages();
}

void MainWindow::on_spinBoxClosingShape_valueChanged(int arg1)
{
    closingShape = arg1;
    MaskImage();
}

void MainWindow::on_spinBoxErosionShape_valueChanged(int arg1)
{
    erosionShape = arg1;
    MaskImage();
}

void MainWindow::on_spinBoxErosionShape1_valueChanged(int arg1)
{
    postErosionShape1 = arg1;
    MaskImage();
}

void MainWindow::on_spinBoxDilationShape2_valueChanged(int arg1)
{
    postDilationShape2 = arg1;
    MaskImage();
}

void MainWindow::on_spinBoxErosionShape3_valueChanged(int arg1)
{
    postErosionShape3 = arg1;
    MaskImage();
}

void MainWindow::on_CheckBoxFillHolesOnOut_toggled(bool checked)
{
    fillHolesOnOutMask = checked;
    MaskImage();
}



void MainWindow::on_CheckBoxExpandMaskY_toggled(bool checked)
{
    expandMaskY = checked;
    MaskImage();
}

void MainWindow::on_CheckBoxShowThesholded_toggled(bool checked)
{
    showThresholded = checked;
    MaskImage();
}

void MainWindow::on_CheckBoxShowSDA_toggled(bool checked)
{
    showSDA = checked;
    MaskImage();
}

void MainWindow::on_CheckBoxShowSDAThresholded_toggled(bool checked)
{
    showSDAThresholded = checked;
    MaskImage();
}
