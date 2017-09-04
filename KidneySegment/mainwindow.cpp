#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

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

    minThresh = ui->spinBoxThreshMin->value();
    maxThresh = ui->spinBoxThreshMax->value();

    displayFlag = CV_WINDOW_NORMAL;
    OnOffImageWindow();


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
    if(showPseudoColor)
        imshow("Input pseudocolor",ShowImage16PseudoColor(InIm,minShow,maxShow));


    int denominator = maxShow-minShow;
    if (denominator == 0)
        denominator = 1;
    int coef = 65535/denominator;

    if(showGray)
        imshow("Input gray",InIm * coef);

    Mask = SegmetU16BetweentwoThresholds(InIm, minThresh, maxThresh);

    if(showRegOnImagePC)
    {
        Mat ImShow = ShowSolidRegionOnImageInGray(Mask, ShowImage16PseudoColor(InIm,minShow,maxShow), 255);
        imshow("Mask treshold",ImShow);
    }
    if(showGradient)
    {
        Mat ImGradient =  GradientDown(InIm);
        imshow("Gradient",ShowImageF32PseudoColor(ImGradient,minShow,maxShow));
    }
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
    int scaledX = maxX*imageShowScale;
    int scaledY = maxY*imageShowScale;
    if(showGray)
        resizeWindow("Input gray",scaledX,scaledY);
    if(showPseudoColor)
        resizeWindow("Input pseudocolor",scaledX,scaledY);
    if(showRegOnImagePC)
        resizeWindow("Mask treshold",scaledX,scaledY);
    if(showGradient)
        resizeWindow("Gradient",scaledX,scaledY);

    //ProcessFile();
}

void MainWindow::on_checkBoxShowGray_toggled(bool checked)
{
    showGray = checked;
    ProcessFile();
}

void MainWindow::on_checkBoxShowPseudoColor_toggled(bool checked)
{
    showPseudoColor = checked;
    ProcessFile();
}

void MainWindow::on_spinBoxThreshMin_valueChanged(int arg1)
{
    minThresh = arg1;
    ProcessFile();
}

void MainWindow::on_spinBoxThreshMax_valueChanged(int arg1)
{
    maxThresh = arg1;
    ProcessFile();
}

void MainWindow::on_checkBoxRegOnPseudoColor_toggled(bool checked)
{
    showRegOnImagePC = checked;
    ProcessFile();
}

void MainWindow::on_checkBoxShowGradient_toggled(bool checked)
{
    showGradient = checked;
    ProcessFile();
}
