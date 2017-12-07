#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

#include <math.h>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "DispLib.h"


using namespace std;
using namespace boost::filesystem;
using namespace boost;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InputDirectory = "C:/Data/Brodatz";//"E:/TestFigs/Brodatz";
    roiOffsetX = ui->spinBoxOffsetX->value();
    roiOffsetY = ui->spinBoxOffsetY->value();
    roiSizeX = ui->spinBoxSizeX->value();
    roiSizeY = ui->spinBoxSizeY->value();
    qLevels = ui->spinBoxQLevels->value();
    imShowScale = ui->spinBoxImShowScale->value();
    maxWavScale = ui->spinBoxMaxWavScale->value();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
void MainWindow::ProcessImage()
{
    Mat ImShow;
    //ImSmall = Mat::zeros(Size(roiSizeX,roiSizeY) ,ImIn.type());
    if(ImIn.empty())
        return;
    ImIn(Rect(roiOffsetX, roiOffsetY, roiSizeX , roiSizeY)).copyTo(ImSmall);//(Rect(roiOffsetX, roiOffsetY, roiSizeX , roiSizeY)));

    //Mat ImSmallF;
    //ImSmall.convertTo(ImSmall,CV_64F);
    cv::resize(ImSmall , ImShow, Size(0,0), (double)imShowScale, (double)imShowScale, INTER_NEAREST ) ;
    imshow("Small Image",ImShow);
    int maxX = ImSmall.cols;
    int maxY = ImSmall.rows;
    int maxXY = maxX*maxY;
    unsigned char *wIm;

    // normalisation
    int sum = 0;
    int count = 0;
    wIm = ( unsigned char *)ImSmall.data;
    for(int i = 0 ; i < maxXY; i++)
    {
        sum+= *wIm;
        count++;
        wIm++;
    }
    double mean = (double)sum/(double)count;

    double sumSqr = 0;
    wIm = ( unsigned char* )ImSmall.data;
    for(int i = 0 ; i < maxXY; i++)
    {
        sumSqr+= ((double)*wIm - mean)*((double)*wIm - mean);
        wIm++;
    }

    if(!count)
        return;

    double std = sqrt(sumSqr/(double)(count - 1));

    minNorm = (int)(mean - 3 * std);
    maxNorm = (int)(mean + 3 * std);
    string outString  = "";
    outString += "#: " + to_string(count);
    outString += " mean: " + to_string(mean);
    outString += " std: " + to_string(std);
    outString += " minN: " + to_string(minNorm);
    outString += " maxN: " + to_string(maxNorm);

    ui->lineEditOutput->setText(outString.c_str());

    double offset = minNorm;
    double mul = (double)qLevels/(maxNorm - minNorm);
    Mat ImNorm = Mat::zeros(maxY,maxX,CV_8U);
    unsigned char* wImNorm;
    wImNorm = ( unsigned char*)ImNorm.data;
    wIm = ( unsigned char* )ImSmall.data;
    for(int i = 0 ; i < maxXY; i++)
    {
        double pixVal = (double)*wIm;
        double newPixVal = (pixVal - offset)*mul;
        *wImNorm = (unsigned char)newPixVal;
        sumSqr+= ((double)*wIm - mean)*((double)*wIm - mean);
        wIm++;
        wImNorm++;
    }

    cv::resize(ImNorm * 255/(qLevels-1), ImShow, Size(0,0), (double)imShowScale, (double)imShowScale, INTER_NEAREST ) ;
    imshow("Small Image normalised",ImShow);
    ImNorm
    for(int scale = 0; scale < maxWavScale; scale++)
    {
        int localMaxX = maxX/(int)pow(2.0,scale);
        int localMaxY = maxY/(int)pow(2.0,scale);
        ImWaveletLL[scale] = Mat::zeros(localMaxY,localMaxX,CV_32F);
        ImWaveletLH[scale] = Mat::zeros(localMaxY,localMaxX,CV_32F);
        ImWaveletHL[scale] = Mat::zeros(localMaxY,localMaxX,CV_32F);
        ImWaveletHH[scale] = Mat::zeros(localMaxY,localMaxX,CV_32F);
        unsigned wInput
        if(!scale)
        {

        }
    }



}

//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------

void MainWindow::on_pushButtonFileOpen_clicked()
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
    ui->lineEditCurrentDirectory->setText(QString::fromWCharArray(InputDirectory.wstring().c_str()));
    ui->listWidgetFiles->clear();
    for (directory_entry& FileToProcess : directory_iterator(InputDirectory))
    {
        if (FileToProcess.path().extension() != ".bmp" && FileToProcess.path().extension() != ".png"
                && FileToProcess.path().extension() != ".tif"& FileToProcess.path().extension() != ".tiff"  )
            continue;
        path PathLocal = FileToProcess.path();

        regex FilePattern(".+");
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
        ui->listWidgetFiles->addItem(PathLocal.filename().string().c_str());
    }
}

void MainWindow::on_listWidgetFiles_currentTextChanged(const QString &currentText)
{
    path CurrentFile =  InputDirectory;
    CurrentFile.append(currentText.toStdString());
    ImIn = imread(CurrentFile.string(),CV_LOAD_IMAGE_ANYDEPTH);
    if(ImIn.empty())
        return;
    imshow("InputImage",ImIn);

    ProcessImage();

}


void MainWindow::on_spinBoxOffsetX_valueChanged(int arg1)
{
    roiOffsetX = arg1;
    ProcessImage();
}

void MainWindow::on_spinBoxOffsetY_valueChanged(int arg1)
{
    roiOffsetY = arg1;
    ProcessImage();
}

void MainWindow::on_spinBoxSizeX_valueChanged(int arg1)
{
    roiSizeX = arg1;
    ProcessImage();
}

void MainWindow::on_spinBoxSizeY_valueChanged(int arg1)
{
    roiSizeX = arg1;
    ProcessImage();
}

void MainWindow::on_spinBoxQLevels_valueChanged(int arg1)
{
    qLevels = arg1;
    ProcessImage();
}

void MainWindow::on_spinBoxImShowScale_valueChanged(int arg1)
{
    imShowScale = arg1;
    ProcessImage();
}

void MainWindow::on_spinBoxMaxWavScale_valueChanged(int arg1)
{
    maxWavScale = arg1;
    ProcessImage();
}
