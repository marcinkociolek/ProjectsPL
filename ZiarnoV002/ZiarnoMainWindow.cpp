#include "ZiarnoMainWindow.h"
#include "ui_ZiarnoMainWindow.h"
#include <QFileDialog>
#include <QMessageBox>

#include <QFileSystemModel>
#include <QStringListModel>
#include <QAbstractItemModel>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <iostream>
#include <string>
#include <vector>

#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "processimage.h"
#include "gradient.h"
#include "DispLib.h"


//QFileSystemModel *model;
QAbstractItemModel *model;


using namespace std;
using namespace boost::filesystem;
using namespace boost;
using namespace cv;

using namespace std::chrono;

// global variables
QStringList Files;



//---------------------------------------------------------------------------------------------------
// main program
//---------------------------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    ui->SegmentationComboBox->addItem("Gray Threshold Manual");
    ui->SegmentationComboBox->addItem("Gradient Threshold manual");

    threshVal = ui->spinBox->value();
    MinRegSize = ui->MinRegionSizeSpinBox->value();

    ui->RawRegionMorfology1ComboBox->addItem("none");
    ui->RawRegionMorfology1ComboBox->addItem("erode5");
    ui->RawRegionMorfology1ComboBox->addItem("erode9");
    ui->RawRegionMorfology1ComboBox->addItem("erode13");


    ui->RawRegionMorfology2ComboBox->addItem("none");
    ui->RawRegionMorfology2ComboBox->addItem("dilate5");
    ui->RawRegionMorfology2ComboBox->addItem("dilate9");
    ui->RawRegionMorfology2ComboBox->addItem("dilate13");

    ui->RawRegionMorfology3ComboBox->addItem("none");
    ui->RawRegionMorfology3ComboBox->addItem("erode5");
    ui->RawRegionMorfology3ComboBox->addItem("erode9");
    ui->RawRegionMorfology3ComboBox->addItem("erode13");


    ui->RegionMorfology1ComboBox->addItem("none");
    ui->RegionMorfology1ComboBox->addItem("erode5");
    ui->RegionMorfology1ComboBox->addItem("erode9");
    ui->RegionMorfology1ComboBox->addItem("erode13");

    ui->RegionMorfology2ComboBox->addItem("none");
    ui->RegionMorfology2ComboBox->addItem("dilate5");
    ui->RegionMorfology2ComboBox->addItem("dilate9");
    ui->RegionMorfology2ComboBox->addItem("dilate13");

    ui->RegionMorfology3ComboBox->addItem("none");
    ui->RegionMorfology3ComboBox->addItem("erode5");
    ui->RegionMorfology3ComboBox->addItem("erode9");
    ui->RegionMorfology3ComboBox->addItem("erode13");


    // analisis options
    segmentType         = ui->SegmentationComboBox->currentIndex();
    threshVal           = ui->spinBox->value();
    removeSmallReg      = ui->RemoveSmalRegionscheckBox->checkState();
    rawMorphErosion1    = ui->RawRegionMorfology1ComboBox->currentIndex();
    rawMorphDilation2   = ui->RawRegionMorfology2ComboBox->currentIndex();
    rawMorphErosion3    = ui->RawRegionMorfology2ComboBox->currentIndex();
    fillHoles           = ui->FillHolesCheckBox->checkState();
    divideSeparateReg   = ui->DivideRegionsCheckBox->checkState();
    MinRegSize          = ui->MinRegionSizeSpinBox->value();
    regMorphErosion1    = ui->RegionMorfology1ComboBox->currentIndex();
    regMorphDilation2   = ui->RegionMorfology2ComboBox->currentIndex();
    regMorphErosion3    = ui->RegionMorfology3ComboBox->currentIndex();
    removeBorderRegion  = ui->RemoveBorderRegionCheckBox->checkState();
    fitEllipseToReg     = ui->FitEllipseCheckBox->checkState();;

    // display options
    showInput           = ui->ShowInputImageCheckBox->checkState();
    showThesholded      = ui->ShowThresholdedImgeCheckBox->checkState();
    show1stMorphology   = ui->ShowAfter1MorphologyCheckBox->checkState();
    showHolesRemoved    = ui->ShowWithoutHolesCheckBox->checkState();
    showMask            = ui->ShowMaskCheckBox->checkState();
    showContourOnInput  = ui->ShowContourOnImageCheckBox->checkState();



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QFileDialog dialog(this, "Open Folder");
    dialog.setFileMode(QFileDialog::Directory);
    //dialog.setDirectory("E:/Ziarno");

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
        InputDirectory = "d:\\";
    }
    if (!is_directory(InputDirectory))
    {
        QMessageBox msgBox;
        msgBox.setText((InputDirectory.string()+ " This is not a directory path ").c_str());
        msgBox.exec();
        InputDirectory = "d:\\";
    }
    //Files.clear();
    ui->DirectoryLineEdit->setText(QString::fromWCharArray(InputDirectory.wstring().c_str()));
    ui->FileListWidget->clear();
    for (directory_entry& FileToProcess : directory_iterator(InputDirectory))
    {
        if (FileToProcess.path().extension() != ".bmp")
            continue;

        path PathLocal = FileToProcess.path();
        if (!exists(PathLocal))
        {
            //Files << PathLocal.filename().string() << " File not exists" << "\n";
            QMessageBox msgBox;
            msgBox.setText((PathLocal.filename().string() + " File not exists" ).c_str());
            msgBox.exec();
            break;
        }
        //Files << PathLocal.filename().string().c_str();
        ui->FileListWidget->addItem(PathLocal.filename().string().c_str());
    }
   // model = new QStringListModel(Files);
    //ui->FileListView->setModel(model);
//    model->setRootPath("D:\\");  //(PathLocal.filename().string().c_str());
//    ui->FileListView->setModel(model);
//    QModelIndex index = model->index(dialog.getExistingDirectory()) ;
//    ui->FileListView->setRootIndex(index);
//    ui->FileTreeView->setRootIndex(index);
   // ui->FileListWidget->setCurrentRow(0);

}






void MainWindow::on_spinBox_valueChanged(int arg1)
{
    threshVal = arg1;
    ProcessImage();
}

void MainWindow::on_FileListWidget_currentTextChanged(const QString &currentText)
{
    FileToOpen = InputDirectory;
    FileToOpen.append(currentText.toStdWString());
    if (!exists(FileToOpen))
        return;
    ImIn = imread(FileToOpen.string().c_str());
    if (ImIn.empty())
        return;
    maxX = ImIn.cols;
    maxY = ImIn.rows/2;
    ImIn(Rect(0, 0, maxX, maxY)).copyTo(ImIn1);
    ImIn(Rect(0, maxY, maxX, maxY)).copyTo(ImIn2);

    if(ui->ShowInputImageCheckBox->checkState())
        imshow("Input image", ImIn1);

    threshVal = ui->spinBox->value();

    ProcessImage();
}


//--------------------------------------------------------------------------------------------------
//          My functions
//--------------------------------------------------------------------------------------------------
void MainWindow::ProcessImage(void)
{
    steady_clock::time_point t1 = steady_clock::now();

    if (ImIn.empty())
        return;
    Mat MaskTresh, MaskMorf1;
    switch (segmentType)
    {
    case 1:
        MaskTresh = FindMaskFromGradient(ImIn1,threshVal);
        break;
    default:
        MaskTresh = FindMaskFromGray(ImIn1,threshVal);
        break;
    }

    if(showThesholded)
    {
        imshow("Thresholded", ShowRegion(MaskTresh));
    }

    if(removeSmallReg)
        MaskMorf1 = RemovingTinyReg9(MaskTresh);
    else
        MaskTresh.copyTo(MaskMorf1);



    switch (rawMorphErosion1)
    {
    case 1:
        RegionErosion5(MaskMorf1);
        break;
    case 2:
        RegionErosion9(MaskMorf1);
        break;
    case 3:
        RegionErosion13(MaskMorf1);
        break;

    default:
        break;
    }

    switch (rawMorphDilation2)
    {
    case 1:
        RegionDilation5(MaskMorf1);
        break;
    case 2:
        RegionDilation9(MaskMorf1);
        break;
    case 3:
        RegionDilation13(MaskMorf1);
        break;

    default:
        break;
    }

    switch (rawMorphErosion3)
    {
    case 1:
        RegionErosion5(MaskMorf1);
        break;
    case 2:
        RegionErosion9(MaskMorf1);
        break;
    case 3:
        RegionErosion13(MaskMorf1);
        break;

    default:
        break;
    }

    if(show1stMorphology)
        imshow("Morphology1", ShowRegion(MaskMorf1));

    Mat MaskWHoles;
    MaskMorf1.copyTo(MaskWHoles);

    if(fillHoles)
    {
        FillBorderWithValue(MaskWHoles, 0xFFFF);
        OneRegionFill5Fast1(MaskWHoles,  0xFFFF);
        FillHoles(MaskWHoles);
        DeleteRegionFromImage(MaskWHoles, 0xFFFF);
    }
    if(showHolesRemoved)
        imshow("WithoutHoles", ShowRegion(MaskWHoles));

    MaskWHoles.copyTo(ImMask1);

    if(divideSeparateReg)
        DivideSeparateRegions(ImMask1, MinRegSize);

    switch (regMorphErosion1)
    {
    case 1:
        RegionErosion5(ImMask1);
        break;
    case 2:
        RegionErosion9(ImMask1);
        break;
    case 3:
        RegionErosion13(ImMask1);
        break;

    default:
        break;
    }

    switch (regMorphDilation2)
    {
    case 1:
        RegionDilation5(ImMask1);
        break;
    case 2:
        RegionDilation9(ImMask1);
        break;
    case 3:
        RegionDilation13(ImMask1);
        break;

    default:
        break;
    }

    switch (regMorphErosion3)
    {
    case 1:
        RegionErosion5(ImMask1);
        break;
    case 2:
        RegionErosion9(ImMask1);
        break;
    case 3:
        RegionErosion13(ImMask1);
        break;

    default:
        break;
    }
    if(removeBorderRegion)
        DeleteRegTouchingBorder(ImMask1);

    if(showMask)
        imshow("Mask", ShowRegion(ImMask1));
 //   return;

    //ImMask1.copyTo(Contour1);
    if(showContourOnInput)
    {
        Contour1 = GetContour5(ImMask1);



        Mat ImToShow = ShowSolidRegionOnImage(Contour1,ImIn1);



        //RotatedRect fittedRect = fitEllipse(ImMask1);
        //Point2f vertices[4];
        //fittedRect.points(vertices);
        //for (int i = 0; i < 4; i++)
        //    line(ImToShow, vertices[i], vertices[(i+1)%4], Scalar(0,255,0));

        imshow("Superimposed", ImToShow);
    }






    steady_clock::time_point t2 = steady_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    ui->DurationLineEdit->setText(  QString::number(time_span.count()));


}
//--------------------------------------------------------------------------------------------------
void MainWindow::OnOffImageWindow(void)
{
    int parametr;
    if(ui->AllowResizeCheckBox->checkState())
        parametr = WINDOW_NORMAL;
    else
        parametr = WINDOW_AUTOSIZE;


    destroyAllWindows();

    if(showInput)
        namedWindow("Input image", parametr);
//    else
//        destroyWindow("Input image");

    if(showThesholded)
        namedWindow("Thresholded", parametr);
//    else
//        destroyWindow("Thresholded");

    if(show1stMorphology)
        namedWindow("Morphology1", parametr);
 //   else
 //       destroyWindow("Morphology1");

    if(showHolesRemoved)
        namedWindow("WithoutHoles", parametr);
//    else
//        destroyWindow("WithoutHoles");

    if(showMask)
        namedWindow("Mask", parametr);
//    else
//        destroyWindow("Mask");

    if(showContourOnInput)
        namedWindow("Superimposed", parametr);
//    else
//        destroyWindow("Superimposed");


    //namedWindow("Histogram", WINDOW_AUTOSIZE);
}


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

void MainWindow::on_RemoveSmalRegionscheckBox_stateChanged(int arg1)
{
    removeSmallReg = arg1;
    ProcessImage();
}

void MainWindow::on_MinRegionSizeSpinBox_valueChanged(int arg1)
{
    MinRegSize = arg1;
    ProcessImage();
}

void MainWindow::on_SegmentationComboBox_currentIndexChanged(int index)
{
    segmentType = index;
    ProcessImage();
}

void MainWindow::on_RawRegionMorfology1ComboBox_currentIndexChanged(int index)
{
    rawMorphErosion1 = index;
    ProcessImage();
}

void MainWindow::on_RawRegionMorfology2ComboBox_currentIndexChanged(int index)
{
    rawMorphDilation2 = index;
    ProcessImage();
}

void MainWindow::on_RawRegionMorfology3ComboBox_currentIndexChanged(int index)
{
    rawMorphErosion3 = index;
    ProcessImage();
}

void MainWindow::on_FillHolesCheckBox_toggled(bool checked)
{
    fillHoles = checked;
    ProcessImage();
}

void MainWindow::on_DivideRegionsCheckBox_toggled(bool checked)
{
    divideSeparateReg = checked;
    ProcessImage();
}

void MainWindow::on_RegionMorfology1ComboBox_currentIndexChanged(int index)
{
    regMorphErosion1 = index;
    ProcessImage();
}

void MainWindow::on_RegionMorfology2ComboBox_currentIndexChanged(int index)
{
    regMorphDilation2 = index;
    ProcessImage();
}

void MainWindow::on_RegionMorfology3ComboBox_currentIndexChanged(int index)
{
    regMorphErosion3 = index;
    ProcessImage();
}

void MainWindow::on_ShowInputImageCheckBox_toggled(bool checked)
{

    showInput = checked;
    OnOffImageWindow();
    ProcessImage();
}

void MainWindow::on_ShowThresholdedImgeCheckBox_toggled(bool checked)
{
    showThesholded = checked;
    OnOffImageWindow();
    ProcessImage();
}

void MainWindow::on_ShowAfter1MorphologyCheckBox_toggled(bool checked)
{
    show1stMorphology = checked;
    OnOffImageWindow();
    ProcessImage();
}

void MainWindow::on_ShowWithoutHolesCheckBox_toggled(bool checked)
{
    showHolesRemoved = checked;
    OnOffImageWindow();
    ProcessImage();
}


void MainWindow::on_pushButtonRegex_clicked()
{
    ui->RegexListWidget->clear();
    regex FilePattern(ui->RegexLineEdit->text().toStdString());

    for (directory_entry& FileToProcess : directory_iterator(InputDirectory))
    {
        if (!regex_match(FileToProcess.path().filename().string().c_str(), FilePattern ))
            continue;

        path PathLocal = FileToProcess.path();
        ui->RegexListWidget->addItem(PathLocal.filename().string().c_str());
    }
}

void MainWindow::on_ShowMaskCheckBox_toggled(bool checked)
{
    showMask = checked;
    OnOffImageWindow();
    ProcessImage();
}

void MainWindow::on_ShowContourOnImageCheckBox_toggled(bool checked)
{
    showContourOnInput = checked;
    OnOffImageWindow();
    ProcessImage();
}

void MainWindow::on_AllowResizeCheckBox_toggled(bool checked)
{
    OnOffImageWindow();
    ProcessImage();
}

void MainWindow::on_RemoveBorderRegionCheckBox_toggled(bool checked)
{
    removeBorderRegion = checked;
    ProcessImage();
}
