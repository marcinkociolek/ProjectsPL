#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <iostream>
#include <string>
#include <vector>

#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include "gradient.h"
#include "DispLib.h"
#include "RegionU16Lib.h"

using namespace std;
using namespace boost::filesystem;
using namespace boost;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonOpenFile_clicked()
{
    QFileDialog dialog(this, "Open Roi");
    path InputFile;
    InputFile = dialog.getOpenFileName().toStdWString();

    if (!exists(InputFile))
    {
        QMessageBox msgBox;
        msgBox.setText((InputFile.string()+ " not exists ").c_str());
        msgBox.exec();
        return;
    }
    RoiIn = imread(InputFile.string().c_str(),CV_LOAD_IMAGE_ANYDEPTH);
    maxX = RoiIn.cols;
    maxY = RoiIn.rows;
    RoiOut = Mat::zeros(maxY,maxX,CV_16U);
    imshow("Reg In", ShowRegion(RoiIn));
    int countOfROI = 15;
    unsigned short originalReg = 2;
    int startAngle = 0;
    int sweepAngle = 12;

    int centerX, centerY;
    CenterOfMas(RoiIn, &centerX, &centerY);
    for(int i = 0; i < countOfROI; i++)
    {
        int direction = startAngle + i * sweepAngle * 2;
        PartOfRegionAtTheAngle(RoiIn, RoiOut,
                                 centerX, centerY, direction,
                                 sweepAngle , originalReg,i+1);
    }
    imshow("Reg Out", ShowRegion(RoiOut));
    imwrite("D:/Testimages/DirectionalityPaper02/Cell02.tif",RoiOut);

}
