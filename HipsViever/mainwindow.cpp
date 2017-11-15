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
void MainWindow::CalculateTransform(void)
{
    if(ImIn.empty())
        return;
    //unsigned short *wImIn = (unsigned short*)ImIn.data;
    //float *wImGradient = (float*)ImGradient.data;

    int maxX = ImIn.cols;
    int maxY = ImIn.rows;
    int maxXY = maxX * maxY;

    int diameter = 100;
    Mat RoiSmall = CreateRoi16(2, diameter, diameter);

    ImConv2 = Mat::zeros(maxY, maxX, CV_16U);
    unsigned short *wImConv2 = (unsigned short*)ImConv2.data;
    unsigned short *wImIn = (unsigned short*)ImIn.data;

    int xLimMin = diameter/2 +1;
    int xLimMax = maxX - diameter/2;
    int yLimMin = diameter/2 + 1;
    int yLimMax = maxY - diameter/2;
    int maxXYSmall = diameter *diameter;


    for(int y = 0; y < maxY; y++)
    {
        for(int x = 0; x < maxX; x++)
        {
            if(x > xLimMin && x < xLimMax && y > yLimMin && y < yLimMax )
            {
                Mat ImSmall;
                unsigned short pixVal = *wImIn;
                int sum = 0;
                ImIn(Rect(x - diameter/2, y - diameter/2, diameter,diameter)).copyTo(ImSmall);
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
                *wImConv2 = sum;
            }

            wImIn++;
            wImConv2++;
        }
    }


}
//--------------------------------------------------------------------------------------------------
void MainWindow::MaskImage(void)
{
    if(ImIn.empty())
        return;
    int maxX = ImIn.cols;
    int maxY = ImIn.rows;
    int maxXY = maxX * maxY;


    Mat MaskForFlood = Mat::zeros(maxY +2, maxX +2, CV_8U);

    Point seedPoint;
    seedPoint.x = 240;
    seedPoint.y = 340;
    Mat ImInF;
    ImIn.convertTo(ImInF, CV_32F);
    floodFill(ImInF,MaskForFlood,seedPoint, 255, 0, Scalar(0),Scalar(256),4 + (255 << 8) + FLOODFILL_MASK_ONLY) ;
    imshow ("flood", MaskForFlood);
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

    if(ImConv2.empty())
        return;

    unsigned short *wImConv = (unsigned short*)ImConv2.data;

    Mask = Mat::zeros(maxY, maxX, CV_16U);
    unsigned short *wMask = (unsigned short*)Mask.data;

    for(int i = 0; i < maxXY; i++)
    {
        //if(*wImIn > thresholdImOrg)
        //    *wMask = 1;
        //wImIn++;

        //if(*wImGradient > thresholdImOrg)
        //    *wMask = 1;
        //wImGradient++;

        if(*wImConv > thresholdImOrg)
            *wMask = 1;
        wImConv++;
        wMask++;
    }




    ShowImages();
}

//--------------------------------------------------------------------------------------------------
void MainWindow::ShowImages(void)
{
    if(ImIn.empty())
        return;
    Mat ImShowGray;
    if(showInputGray)
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
        Mat ImShowGradient = ShowImageF32PseudoColor(ImGradient,minShowGradient,maxShowGradient);
        imshow("Gradient", ImShowGradient);
    }
    if(showMask)
    {
        Mat ImShowMask = ShowSolidRegionOnImageInBlack(Mask, ImShowPseudocolor);
        imshow("Mask", ImShowMask);
    }
    if(showConv && !ImConv2.empty())
    {
        Mat ImShowConverted = ShowImage16PseudoColor(ImConv2,minShowConv,maxShowConv);
        imshow("Converted 2", ImShowConverted);
    }
    if(showConv && !ImConv.empty())
    {
        Mat ImShowConverted = ShowImage16PseudoColor(ImConv,minShowConv,maxShowConv);
        imshow("Converted", ImShowConverted);
    }
}

//--------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    showInputPseudocolor = ui->CheckBoxShowInputImagePC->checkState();
    showInputGray = ui->CheckBoxShowInputImageGray->checkState();
    showGradient = ui->CheckBoxShowGradient->checkState();
    showMask = ui->CheckBoxShowMask->checkState();
    showConv = ui->CheckBoxShowConv->checkState();

    minShowGray = ui->spinBoxMinShowGray->value();
    maxShowGray = ui->spinBoxMaxShowGray->value();
    minShowPseudocolor = ui->spinBoxMinShowPseudoColor->value();
    maxShowPseudocolor = ui->spinBoxMaxShowPseudoColor->value();
    minShowGradient = ui->spinBoxMinShowGradient->value();
    maxShowGradient = ui->spinBoxMaxShowGradient->value();

    minShowConv = ui->spinBoxMinShowConv->value();
    maxShowConv = ui->spinBoxMaxShowConv->value();

    thresholdImOrg = ui->spinBoxThresholdOryginalImage->value();
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
    ImGradient = GradientDown(ImIn);

    FileName = regex_replace(FileName,regex("_crop"),"_SDA");
    path FileToOpen2 = InputDirectory;
    FileToOpen2.append(FileName);
    ImConv = imread(FileToOpen2.string().c_str(),CV_LOAD_IMAGE_ANYDEPTH);

    //CalculateTransform();
    MaskImage();
    ProcessImage();

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
    ProcessImage();
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
    minShowConv = arg1;
    ShowImages();
}

void MainWindow::on_spinBoxMaxShowConv_valueChanged(int arg1)
{
    maxShowConv = arg1;
    ShowImages();
}
