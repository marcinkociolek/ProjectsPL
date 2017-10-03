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

    showRegOnImagePC = ui->checkBoxRegOnPseudoColor->checkState();


    maxX = 192;
    maxY = 192;

    ui->comboBoxRegioNr->addItem("none");
    ui->comboBoxRegioNr->addItem("CortexL");
    ui->comboBoxRegioNr->addItem("PelvisL");
    ui->comboBoxRegioNr->addItem("MedulaL");
    ui->comboBoxRegioNr->addItem("CortexP");
    ui->comboBoxRegioNr->addItem("PelvisP");
    ui->comboBoxRegioNr->addItem("MedulaP");
    ui->comboBoxRegioNr->setCurrentIndex(0);
    regionIndex = 0;

    minThresh = ui->spinBoxThreshMin->value();
    maxThresh = ui->spinBoxThreshMax->value();

    //displayFlag = CV_WINDOW_NORMAL;

    showPelvis = ui->checkBoxShowPelvis->checkState();
    showCortex = ui->checkBoxShowCortex->checkState();
    showMedula = ui->checkBoxShowMedula->checkState();

    Mask2 = Mat::zeros(maxY,maxX,CV_16U);
    MaskCortex1 = Mat::zeros(maxY,maxX,CV_16U);
    MaskPelvis1 = Mat::zeros(maxY,maxX,CV_16U);
    MaskMedula1 = Mat::zeros(maxY,maxX,CV_16U);
    MaskCortex2 = Mat::zeros(maxY,maxX,CV_16U);
    MaskPelvis2 = Mat::zeros(maxY,maxX,CV_16U);
    MaskMedula2 = Mat::zeros(maxY,maxX,CV_16U);


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
/*
    if(showRegOnImagePC)
    {
        Mat ImShow = ShowSolidRegionOnImageInGray(Mask, ShowImage16PseudoColor(InIm,minShow,maxShow), 255);
        cvtColor(ImShow,ImShow,CV_BGR2RGB);
        QImage QIm((unsigned char*)ImShow.data, ImShow.cols, ImShow.rows, QImage::Format_RGB888);
        ui->labelMask->setPixmap(QPixmap::fromImage(QIm));

    }
    Mat ImShow = ShowSolidRegionOnImage(Mask2,ShowImage16PseudoColor(InIm,minShow,maxShow));
*/

    if(InIm.empty())
        return;
    ImGradient = GradientDown(InIm);
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
        wMask = (unsigned short *)MaskCortex1.data;
        for(int i = 0; i < maxXY; i ++)
        {
            if(*wMask)
                *wMask2 = 1;
            wMask2++;
            wMask++;
        }
        wMask2 = (unsigned short *)Mask2.data;
        wMask = (unsigned short *)MaskCortex2.data;
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
            ImShow = ShowTransparentRegionOnImage(Mask2,ImShowGray);
            break;
        case 4:
            ImShow = ShowTransparentRegionOnImage(GetContour5(Mask2),ImShowGray);
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
            ImShow = ShowTransparentRegionOnImage(Mask2,ImShowPseudoColor);
            break;
        case 4:
            ImShow = ShowTransparentRegionOnImage(GetContour5(Mask2),ImShowPseudoColor);
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
            ImShow = ShowTransparentRegionOnImage(Mask2,ImShowGradient);
            break;
        case 4:
            ImShow = ShowTransparentRegionOnImage(GetContour5(Mask2),ImShowGradient);
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
    positionY += maxY*2;
    if(positionY<340)
        positionY = 340;
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
    if(showRegOnImagePC)
    {
        ui->labelMask->setGeometry(positionX,positionY,scaledX,scaledY);
        positionX += 10 + scaledX;
    }
    else
        ui->labelMask->setGeometry(0,positionY,0,0);

    ui->widgetImage->setGeometry(750,10,maxX*2,maxY*2);
    //ProcessFile();

}

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
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

void MainWindow::on_spinBoxThreshMin_valueChanged(int arg1)
{
    minThresh = arg1;
    ProcessFile();
    ProcessFile();
}

void MainWindow::on_spinBoxThreshMax_valueChanged(int arg1)
{
    maxThresh = arg1;
    ProcessFile();
    ProcessFile();
}

void MainWindow::on_checkBoxRegOnPseudoColor_toggled(bool checked)
{
    showRegOnImagePC = checked;
    ScaleImages();
    //ProcessFile();
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

    ui->spinBoxValGradient->setValue(x);
    ui->spinBoxValIntensity->setValue(y);

    ui->spinBoxButton->setValue(butPressed);

    if(butPressed == 1)
    {
        switch(regionIndex)
        {
        case 1:
            MaskCortex1.at<unsigned short>(y,x) = 1;
            break;
        case 2:
            MaskPelvis1.at<unsigned short>(y,x) = 1;
            break;
        case 3:
            MaskMedula1.at<unsigned short>(y,x) = 1;
            break;
        case 4:
            MaskCortex2.at<unsigned short>(y,x) = 1;
            break;
        case 5:
            MaskPelvis2.at<unsigned short>(y,x) = 1;
            break;
        case 6:
            MaskMedula2.at<unsigned short>(y,x) = 1;
            break;

        default:
            break;
        }
    }
    if(butPressed == 2)
    {
        switch(regionIndex)
        {
        case 1:
            MaskCortex1.at<unsigned short>(y,x) = 0;
            break;
        case 2:
            MaskPelvis1.at<unsigned short>(y,x) = 0;
            break;
        case 3:
            MaskMedula1.at<unsigned short>(y,x) = 0;
            break;
        case 4:
            MaskCortex2.at<unsigned short>(y,x) = 0;
            break;
        case 5:
            MaskPelvis2.at<unsigned short>(y,x) = 0;
            break;
        case 6:
            MaskMedula2.at<unsigned short>(y,x) = 0;
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
        FillBorderWithValue(MaskCortex1, 0xFFFF);
        OneRegionFill5Fast1(MaskCortex1,  0xFFFF);
        FillHoles(MaskCortex1);
        DeleteRegionFromImage(MaskCortex1, 0xFFFF);
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
        FillBorderWithValue(MaskCortex2, 0xFFFF);
        OneRegionFill5Fast1(MaskCortex2,  0xFFFF);
        FillHoles(MaskCortex2);
        DeleteRegionFromImage(MaskCortex2, 0xFFFF);
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
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskCortex1(Rect(0, 1, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 0, maxX , maxY -1)));
    MaskTemp.copyTo(MaskCortex1);
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskCortex2(Rect(0, 1, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 0, maxX , maxY -1)));
    MaskTemp.copyTo(MaskCortex2);
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskPelvis1(Rect(0, 1, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 0, maxX , maxY -1)));
    MaskTemp.copyTo(MaskPelvis1);
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskPelvis2(Rect(0, 1, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 0, maxX , maxY -1)));
    MaskTemp.copyTo(MaskPelvis2);
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskMedula1(Rect(0, 1, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 0, maxX , maxY -1)));
    MaskTemp.copyTo(MaskMedula1);
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskMedula2(Rect(0, 1, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 0, maxX , maxY -1)));
    MaskTemp.copyTo(MaskMedula2);
    ShowImages();
}

void MainWindow::on_pushButtonDown_clicked()
{
    Mat MaskTemp;
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskCortex1(Rect(0, 0, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 1, maxX , maxY -1)));
    MaskTemp.copyTo(MaskCortex1);
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskCortex2(Rect(0, 0, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 1, maxX , maxY -1)));
    MaskTemp.copyTo(MaskCortex2);
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskPelvis1(Rect(0, 0, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 1, maxX , maxY -1)));
    MaskTemp.copyTo(MaskPelvis1);
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskPelvis2(Rect(0, 0, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 1, maxX , maxY -1)));
    MaskTemp.copyTo(MaskPelvis2);
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskMedula1(Rect(0, 0, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 1, maxX , maxY -1)));
    MaskTemp.copyTo(MaskMedula1);
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskMedula2(Rect(0, 0, maxX , maxY -1)).copyTo(MaskTemp(Rect(0, 1, maxX , maxY -1)));
    MaskTemp.copyTo(MaskMedula2);
    ShowImages();
}

void MainWindow::on_pushButtonRight_clicked()
{
    Mat MaskTemp;
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskCortex1(Rect(0, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(1, 0, maxX  -1, maxY)));
    MaskTemp.copyTo(MaskCortex1);
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskCortex2(Rect(0, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(1, 0, maxX  -1, maxY)));
    MaskTemp.copyTo(MaskCortex2);
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskPelvis1(Rect(0, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(1, 0, maxX  -1, maxY)));
    MaskTemp.copyTo(MaskPelvis1);
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskPelvis2(Rect(0, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(1, 0, maxX  -1, maxY)));
    MaskTemp.copyTo(MaskPelvis2);
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskMedula1(Rect(0, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(1, 0, maxX  -1, maxY)));
    MaskTemp.copyTo(MaskMedula1);
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskMedula2(Rect(0, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(1, 0, maxX  -1, maxY)));
    MaskTemp.copyTo(MaskMedula2);

    ShowImages();
}

void MainWindow::on_pushButtonLeft_clicked()
{
    Mat MaskTemp;

    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskCortex1(Rect(1, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(0, 0, maxX  -1, maxY)));
    MaskTemp.copyTo(MaskCortex1);
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskCortex2(Rect(1, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(0, 0, maxX  -1, maxY)));
    MaskTemp.copyTo(MaskCortex2);
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskPelvis1(Rect(1, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(0, 0, maxX  -1, maxY)));
    MaskTemp.copyTo(MaskPelvis1);
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskPelvis2(Rect(1, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(0, 0, maxX  -1, maxY)));
    MaskTemp.copyTo(MaskPelvis2);
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskMedula1(Rect(1, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(0, 0, maxX  -1, maxY)));
    MaskTemp.copyTo(MaskMedula1);
    MaskTemp = Mat::zeros(maxY,maxX,CV_16U);
    MaskMedula2(Rect(1, 0, maxX -1 , maxY)).copyTo(MaskTemp(Rect(0, 0, maxX  -1, maxY)));
    MaskTemp.copyTo(MaskMedula2);
    ShowImages();
}