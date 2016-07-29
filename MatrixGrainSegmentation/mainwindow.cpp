#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

//#include <QFileSystemModel>
//#include <QStringListModel>
//#include <QAbstractItemModel>

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

using namespace std;
using namespace boost::filesystem;
using namespace boost;
using namespace cv;

using namespace std::chrono;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // display options
    //if(ui->AllowResizeCheckBox->checkState())
        displayFlag = WINDOW_NORMAL;
    //else
    //    displayFlag = WINDOW_AUTOSIZE;


    namedWindow("Input image", displayFlag);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonOpenFile_clicked()
{
    QFileDialog dialog(this, "Open Folder");
    if(dialog.exec())
    {
        FileToOpen = dialog.directory().path().toStdWString();
    }
    else
         return;
    if (!exists(FileToOpen))
    {
        QMessageBox msgBox;
        msgBox.setText((FileToOpen.string()+ " not exists ").c_str());
        msgBox.exec();
    }
    ProcessImage();
}
//--------------------------------------------------------------------------------------------------
//          My functions
//--------------------------------------------------------------------------------------------------
void MainWindow::ProcessImage(void)
{
    ImIn = imread(FileToOpen.string().c_str());
    if (ImIn.empty())
        return;
    maxX = ImIn.cols/2;
    maxY = ImIn.rows;

    //if(showInput)
        imshow("Input image",ImIn);
}
