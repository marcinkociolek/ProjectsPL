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
    showPseudoColor = ui->checkBoxShowPseudoColor->checkState();
    showRegOnImagePC = ui->checkBoxRegOnPseudoColor->checkState();
    showGradient = ui->checkBoxShowGradient->checkState();

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

    displayFlag = CV_WINDOW_NORMAL;

    Mask2 = Mat::zeros(maxY,maxX,CV_16U);


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

    //ui->widgetImage->paint
    if(showGray)
    {
        Mat ImShow = ShowImage16Gray(InIm,minShow,maxShow);
        QImage QIm((unsigned char*)ImShow.data, ImShow.cols, ImShow.rows, QImage::Format_RGB888);
        ui->labelGray->setPixmap(QPixmap::fromImage(QIm));
    }
    if(showPseudoColor)
    {
        Mat ImShow = ShowImage16PseudoColor(InIm,minShow,maxShow);
        cvtColor(ImShow,ImShow,CV_BGR2RGB);
        QImage QIm((unsigned char*)ImShow.data, ImShow.cols, ImShow.rows, QImage::Format_RGB888);
        ui->labelImage->setPixmap(QPixmap::fromImage(QIm));
    }
    if(showGradient)
    {
        Mat ImGradient =  GradientDown(InIm);
        Mat ImShow = ShowImageF32PseudoColor(ImGradient,minShow,maxShow);
        cvtColor(ImShow,ImShow,CV_BGR2RGB);
        QImage QIm((unsigned char*)ImShow.data, ImShow.cols, ImShow.rows, QImage::Format_RGB888);
        ui->labelGradient->setPixmap(QPixmap::fromImage(QIm));
    }
    Mask = SegmetU16BetweentwoThresholds(InIm, minThresh, maxThresh);

    if(showRegOnImagePC)
    {
        Mat ImShow = ShowSolidRegionOnImageInGray(Mask, ShowImage16PseudoColor(InIm,minShow,maxShow), 255);
        cvtColor(ImShow,ImShow,CV_BGR2RGB);
        QImage QIm((unsigned char*)ImShow.data, ImShow.cols, ImShow.rows, QImage::Format_RGB888);
        ui->labelMask->setPixmap(QPixmap::fromImage(QIm));

    }
    Mat ImShow = ShowSolidRegionOnImage(Mask2,ShowImage16PseudoColor(InIm,minShow,maxShow));

    ui->widgetImage->paintBitmap(ImShow);
    ui->widgetImage->repaint();
    //ui->widgetImage->Im = ShowImage16PseudoColor(InIm,minShow,maxShow);
    //ui->widgetImage->Dupa(1);


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
    int positionY = 20;
    positionY += maxY*imageShowScale;
    if(positionY<330)
        positionY = 330;
    if(showGray)
    {
        ui->labelGray->setGeometry(positionX,positionY,scaledX,scaledY);
        positionX += 10 + scaledX;
    }
    else
        ui->labelGray->setGeometry(0,positionY,0,0);
    if(showPseudoColor)
    {
        ui->labelImage->setGeometry(positionX,positionY,scaledX,scaledY);
        positionX += 10 + scaledX;
    }
    else
        ui->labelImage->setGeometry(0,positionY,0,0);
    if(showGradient)
    {
        ui->labelGradient->setGeometry(positionX,positionY,scaledX,scaledY);
        positionX += 10 + scaledX;
    }
    else
        ui->labelGradient->setGeometry(0,positionY,0,0);
    if(showRegOnImagePC)
    {
        ui->labelMask->setGeometry(positionX,positionY,scaledX,scaledY);
        positionX += 10 + scaledX;
    }
    else
        ui->labelMask->setGeometry(0,positionY,0,0);

    ui->widgetImage->setGeometry(750,10,scaledX,scaledY);
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
    ProcessFile();
}

void MainWindow::on_checkBoxShowGray_toggled(bool checked)
{
    showGray = checked;
    ScaleImages();
    ProcessFile();
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
    ProcessFile();
}

void MainWindow::on_checkBoxShowGradient_toggled(bool checked)
{
    showGradient = checked;
    ScaleImages();
    ProcessFile();
}

void MainWindow::on_pushButtonGetPixelValues_clicked()
{

}

void MainWindow::on_widgetImage_mousePressed(QPoint point)
{
    //ui->spinBoxValGradient->setValue(point.x());
    //ui->spinBoxValIntensity->setValue(point.y());
}


void MainWindow::on_widgetImage_mouseMoved(QPoint point)
{
    int x = point.x()/imageShowScale;
    int y = point.y()/imageShowScale;

    ui->spinBoxValGradient->setValue(x);
    ui->spinBoxValIntensity->setValue(y);

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
    Mat ImShow = ShowSolidRegionOnImage(Mask2,ShowImage16PseudoColor(InIm,minShow,maxShow));

    ui->widgetImage->paintBitmap(ImShow);
    ui->widgetImage->repaint();
}

//########################################################################################

void MainWindow::on_comboBoxRegioNr_currentIndexChanged(int index)
{
    regionIndex = index;
}
