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

using namespace std;
using namespace boost::filesystem;
using namespace boost;
using namespace cv;

//--------------------------------------------------------------------------------------------------
//          My functions
//--------------------------------------------------------------------------------------------------
void MainWindow::ProcessImage(void)
{
    if(ImIn.empty())
        return;

    cvtColor(ImIn,ImGray,CV_BGR2GRAY);

    threshold(ImGray, ImMask, thesholdVal ,1,THRESH_BINARY);
    ImMask.convertTo(ImMask, CV_16U);
    //ImShowPseudoColor = ShowImage16PseudoColor(InIm,minShowPseudoColor,maxShowPseudoColor);
    //ImShowGradient = ShowImageF32PseudoColor(ImGradient,minShowGradient,maxShowGradient);

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

    minShowGray = ui->spinBoxMinShowGray->value();
    maxShowGray = ui->spinBoxMaxShowGray->value();
    minShowPseudocolor = ui->spinBoxMinShowPseudoColor->value();
    maxShowPseudocolor = ui->spinBoxMaxShowPseudoColor->value();
    minShowGradient = ui->spinBoxMinShowGradient->value();
    maxShowGradient = ui->spinBoxMaxShowGradient->value();

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

        regex FilePattern("crop.+");
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
    ProcessImage();
}



void MainWindow::on_spinBoxMinShowGray_valueChanged(int arg1)
{
    minShowGray = arg1;
    ProcessImage();
}

void MainWindow::on_spinBoxMaxShowGray_valueChanged(int arg1)
{
    maxShowGray = arg1;
    ProcessImage();
}

void MainWindow::on_CheckBoxShowInputImageGray_toggled(bool checked)
{
    showInputGray = checked;
    ProcessImage();
}

void MainWindow::on_CheckBoxShowInputImagePC_toggled(bool checked)
{
    showInputPseudocolor = checked;
    ProcessImage();
}

void MainWindow::on_spinBoxMinShowPseudoColor_valueChanged(int arg1)
{
    minShowPseudocolor = arg1;
    ProcessImage();
}

void MainWindow::on_spinBoxMaxShowPseudoColor_valueChanged(int arg1)
{
    maxShowPseudocolor = arg1;
    ProcessImage();
}

void MainWindow::on_spinBoxMinShowGradient_valueChanged(int arg1)
{
    minShowGradient = arg1;
    ProcessImage();
}

void MainWindow::on_spinBoxMaxShowGradient_valueChanged(int arg1)
{
    maxShowGradient = arg1;
    ProcessImage();
}

void MainWindow::on_CheckBoxShowGradient_toggled(bool checked)
{
    showGradient = checked;
    ProcessImage();
}
