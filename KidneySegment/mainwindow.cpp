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
    ui->comboBoxShowModeGradient->setCurrentIndex(1);
    showModeGradient = ui->comboBoxShowModeGradient->currentIndex();
    //showRegionOnGradient = ui->checkBoxShowRegionOnGradient->checkState();
    //showContourOnGradient = ui->checkBoxShowContourOnGradient->checkState();

    showRegOnImagePC = ui->checkBoxRegOnPseudoColor->checkState();


    maxX = 192;
    maxY = 192;

    ui->comboBoxRegioNr->addItem("none");
    ui->comboBoxRegioNr->addItem("Cortex");
    ui->comboBoxRegioNr->addItem("Pelvis");
    ui->comboBoxRegioNr->addItem("Medula");
    ui->comboBoxRegioNr->setCurrentIndex(0);
    regionIndex = 0;

    minThresh = ui->spinBoxThreshMin->value();
    maxThresh = ui->spinBoxThreshMax->value();

    //displayFlag = CV_WINDOW_NORMAL;

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
        default:
            ImShow = ImShowGray;
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
        default:
            ImShow = ImShowPseudoColor;
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
        default:
            ImShow = ImShowGradient;
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
    switch(regionIndex)
    {
    case 1:
        Mask2.at<unsigned short>(y,x) = 1;
        break;
    case 2:
        Mask2.at<unsigned short>(y,x) = 2;
        break;
    case 3:
        Mask2.at<unsigned short>(y,x) = 3;
        break;
    default:
        break;
    }
    if(regionIndex>0 && regionIndex<7)
    {
        if(butPressed == 1)
            Mask2.at<unsigned short>(y,x) = regionIndex;
        if(butPressed == 2)
            Mask2.at<unsigned short>(y,x) = 0;

    }



 /*
    Mat ImShow = ShowSolidRegionOnImage(Mask2,ShowImage16PseudoColor(InIm,minShow,maxShow));

    ui->widgetImage->paintBitmap(ImShow);
    ui->widgetImage->repaint();
 */
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
    FillBorderWithValue(Mask2, 0xFFFF);
    OneRegionFill5Fast1(Mask2,  0xFFFF);
    FillHoles(Mask2);
    DeleteRegionFromImage(Mask2, 0xFFFF);
    ShowImages();
}
