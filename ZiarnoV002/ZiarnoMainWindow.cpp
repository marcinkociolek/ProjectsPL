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
    fitEllipseToReg     = ui->FitEllipseCheckBox->checkState();
    rotateImage         = ui->RotateCheckBox->checkState();
    croppImage          = ui->CroppCheckBox->checkState();

    // display options
    showInput           = ui->ShowInputImageCheckBox->checkState();
    showThesholded      = ui->ShowThresholdedImgeCheckBox->checkState();
    show1stMorphology   = ui->ShowAfter1MorphologyCheckBox->checkState();
    showHolesRemoved    = ui->ShowWithoutHolesCheckBox->checkState();
    showMask            = ui->ShowMaskCheckBox->checkState();
    showContourOnInput  = ui->ShowContourOnImageCheckBox->checkState();
    showFitted          = ui->ShowFitedElipseCheckBox->checkState();
    showRotated         = ui->ShowRotatedCheckBox->checkState();
    showCropped         = ui->ShowCroppedCheckBox->checkState();

    if(ui->AllowResizeCheckBox->checkState())
        displayFlag = WINDOW_NORMAL;
    else
        displayFlag = WINDOW_AUTOSIZE;


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QFileDialog dialog(this, "Open Folder");
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setDirectory("E:/Ziarno/3.11.2015_uszkodzone/Oble");

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

    RotatedRect fittedRect;
    fittedRect.angle = 0.0;
    fittedRect.center = Point2f(ImIn1.cols/2,ImIn1.rows/2);
    fittedRect.size = Size2f(100.0,100.0);


    if(fitEllipseToReg)
    {
        Mat ImTemp;
        ImMask1.convertTo(ImTemp,CV_8U);

        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        findContours(ImTemp,contours,hierarchy,CV_RETR_LIST,CHAIN_APPROX_NONE);

        Mat pointsF;
        Mat(contours[0]).convertTo(pointsF, CV_32F);
        fittedRect = fitEllipse(pointsF);
    }

    if(showFitted)
    {
        Point2f vertices[4];
        fittedRect.points(vertices);
        Mat ImToShow;
        ImIn1.copyTo(ImToShow);
        for (int i = 0; i < 4; i++)
            line(ImToShow, vertices[i], vertices[(i+1)%4], Scalar(0,255,0));

        imshow("Fitted", ImToShow);
    }


    Mat ImRotated;
    if(rotateImage)
    {
        Mat RotationMatrix = getRotationMatrix2D(fittedRect.center,fittedRect.angle,1.0);
        Size smallImageSize;
        smallImageSize.height = (int)(fittedRect.size.height * 1.2);
        smallImageSize.width = (int)(fittedRect.size.width * 1.2);

        //Mat ImTemp
        warpAffine(ImIn1,ImRotated,RotationMatrix,Size(ImIn1.cols,ImIn1.rows));

    }
    else
        ImRotated = Mat::zeros(ImIn1.rows,ImIn1.cols,CV_8UC3);

    RotatedRect alignedRect = fittedRect;
    alignedRect.angle = 0.0;
    if(showRotated)
    {
        Point2f vertices[4];
        alignedRect.points(vertices);
        Mat ImToShow;
        ImRotated.copyTo(ImToShow);
        for (int i = 0; i < 4; i++)
            line(ImToShow, vertices[i], vertices[(i+1)%4], Scalar(0,255,0));
        imshow("Rotated", ImToShow);
    }

    Mat ImCropped;
    if(croppImage)
    {
        Size croppedImageSize;
        int croppWidth = (((int)(alignedRect.size.width * 1.2))/8)*8;
        int croppHeight = (((int)(alignedRect.size.height * 1.2))/8)*8;
        int croppX = (int)alignedRect.center.x - croppWidth/2;
        if(croppX < 0)
            croppX = 0;
        int croppY = (int)alignedRect.center.y - croppHeight/2;
        if(croppY < 0)
            croppY = 0;

        ImRotated(Rect(croppX,croppY,croppWidth,croppHeight)).copyTo(ImCropped);
    }
    else
        ImCropped = ImIn1;

    if(showCropped)
    {
        /*
        Point2f vertices[4];
        alignedRect.points(vertices);
        Mat ImToShow;
        ImRotated.copyTo(ImToShow);
        for (int i = 0; i < 4; i++)
            line(ImToShow, vertices[i], vertices[(i+1)%4], Scalar(0,255,0));
        */

        imshow("Cropped", ImCropped);
    }


    steady_clock::time_point t2 = steady_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    ui->DurationLineEdit->setText(  QString::number(time_span.count()));


}
//--------------------------------------------------------------------------------------------------
void MainWindow::OnOffImageWindow(void)
{
    destroyAllWindows();

    if(showInput)
        namedWindow("Input image", displayFlag);

    if(showThesholded)
        namedWindow("Thresholded", displayFlag);

    if(show1stMorphology)
        namedWindow("Morphology1", displayFlag);

    if(showHolesRemoved)
        namedWindow("WithoutHoles", displayFlag);

    if(showMask)
        namedWindow("Mask", displayFlag);

    if(showContourOnInput)
        namedWindow("Superimposed", displayFlag);

    if(showFitted)
        namedWindow("Fitted", displayFlag);

    if(showRotated)
        namedWindow("Rotated", displayFlag);

    if(showCropped)
        namedWindow("Cropped", displayFlag);

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
    if(showInput)
        namedWindow("Input image", displayFlag);
    else
        destroyWindow("Input image");
    ProcessImage();
}

void MainWindow::on_ShowThresholdedImgeCheckBox_toggled(bool checked)
{
    showThesholded = checked;
    if(showThesholded)
        namedWindow("Thresholded", displayFlag);
    else
        destroyWindow("Thresholded");
    ProcessImage();
}

void MainWindow::on_ShowAfter1MorphologyCheckBox_toggled(bool checked)
{
    show1stMorphology = checked;
    if(show1stMorphology)
        namedWindow("Morphology1", displayFlag);
    else
        destroyWindow("Morphology1");
    ProcessImage();
}

void MainWindow::on_ShowWithoutHolesCheckBox_toggled(bool checked)
{
    showHolesRemoved = checked;
    if(showHolesRemoved)
        namedWindow("WithoutHoles", displayFlag);
    else
        destroyWindow("WithoutHoles");
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
    if(showMask)
        namedWindow("Mask", displayFlag);
    else
        destroyWindow("Mask");

    ProcessImage();
}

void MainWindow::on_ShowContourOnImageCheckBox_toggled(bool checked)
{
    showContourOnInput = checked;

    if(showContourOnInput)
        namedWindow("Superimposed", displayFlag);
    else
        destroyWindow("Superimposed");
    ProcessImage();
}

void MainWindow::on_AllowResizeCheckBox_toggled(bool checked)
{
    if(checked)
        displayFlag = WINDOW_NORMAL;
    else
        displayFlag = WINDOW_AUTOSIZE;

    OnOffImageWindow();
    ProcessImage();
}

void MainWindow::on_RemoveBorderRegionCheckBox_toggled(bool checked)
{
    removeBorderRegion = checked;
    ProcessImage();
}

void MainWindow::on_FitEllipseCheckBox_toggled(bool checked)
{
    fitEllipseToReg = checked;
    ProcessImage();
}

void MainWindow::on_RotateCheckBox_toggled(bool checked)
{
    rotateImage = checked;
    ProcessImage();
}

void MainWindow::on_ShowFitedElipseCheckBox_toggled(bool checked)
{
    showFitted = checked;
    if(showFitted)
        namedWindow("Fitted", displayFlag);
    else
        destroyWindow("Fitted");

    ProcessImage();
}

void MainWindow::on_ShowRotatedCheckBox_toggled(bool checked)
{
    showRotated = checked;
    if(showRotated)
        namedWindow("Rotated", displayFlag);
    else
        destroyWindow("Rotated");

    ProcessImage();
}

void MainWindow::on_CroppCheckBox_toggled(bool checked)
{
    croppImage = checked;
    ProcessImage();
}

void MainWindow::on_ShowCroppedCheckBox_toggled(bool checked)
{
    showCropped = checked;
    if(showCropped)
        namedWindow("Cropped", displayFlag);
    else
        destroyWindow("Cropped");

    ProcessImage();
}
