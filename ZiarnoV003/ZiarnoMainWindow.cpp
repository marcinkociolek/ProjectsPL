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

#include "tiffio.h"

//#include "processimage.h"
#include "gradient.h"
#include "DispLib.h"
#include "SegmentGrainImage.h"

#include "mazdaroi.h"
#include "mazdaroiio.h"

typedef MazdaRoi<unsigned int, 2> MR2DType;

//QFileSystemModel *model;
QAbstractItemModel *model;


using namespace std;
using namespace boost::filesystem;
using namespace boost;
using namespace cv;

using namespace std::chrono;

// global variables
QStringList Files;

#define TERAZ_DEBUG

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

    if(ui->ShowComboBox->currentIndex())
        showContour = true;
    else
        showContour = false;

    ui->ShowComboBox->addItem("Region");
    ui->ShowComboBox->addItem("Contour");
    ui->ShowComboBox->setCurrentIndex(1);

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

    ui->InputTypecomboBox->addItem("Horizontal");
    ui->InputTypecomboBox->addItem("Vertical");
    ui->InputTypecomboBox->addItem("3 separated");

    ui->InputTypecomboBox->setCurrentIndex(2);

    // analisis options
    inputType = ui->InputTypecomboBox->currentIndex();
    verticalInputImages = ui->VerticalDivisionCheckBox->checkState();
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
    alignGrains         = ui->AlignCheckBox->checkState();
    findValey           = ui->FindValeyCheckBox->checkState();


    fitEllipseToReg     = ui->FitEllipseCheckBox->checkState();
    rotateImage         = ui->RotateCheckBox->checkState();
    croppImage          = ui->CroppCheckBox->checkState();
    subsegment          = ui->SubsegmentCheckBox->checkState();

    saveResult          =ui->SaveResultCheckBox->checkState();

    // display options
    showInput           = ui->ShowInputImageCheckBox->checkState();
    showThesholded      = ui->ShowThresholdedImgeCheckBox->checkState();
    show1stMorphology   = ui->ShowAfter1MorphologyCheckBox->checkState();
    showHolesRemoved    = ui->ShowWithoutHolesCheckBox->checkState();
    showMask            = ui->ShowMaskCheckBox->checkState();
    showAreaForAlign    = ui->ShowAreaForAlignCheckBox->checkState();
    showAligned         = ui->ShowAlignedCheckBox->checkState();
    showGradient        = ui->ShowGradientCheckBox->checkState();
    showOutput          = ui->ShowOutputCheckBox->checkState();
    showOutput2         = ui->ShowOutput2CheckBox->checkState();

    showContourOnInput  = ui->ShowContourOnImageCheckBox->checkState();

    showFitted          = ui->ShowFitedElipseCheckBox->checkState();
    showRotated         = ui->ShowRotatedCheckBox->checkState();
    showAreaForAlign    = ui->ShowAreaForAlignCheckBox->checkState();
    showAligned         = ui->ShowAlignedCheckBox->checkState();
    if(ui->AllowResizeCheckBox->checkState())
        displayFlag = WINDOW_NORMAL;
    else
        displayFlag = WINDOW_AUTOSIZE;


    InputDirectory = "C:\\Data\\Ziarno\\14.04.2016 Rozne klasy\\Dobre\\";
    OutputDirectory = "c:\\";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
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
    ui->DirectoryLineEdit->setText(QString::fromWCharArray(InputDirectory.wstring().c_str()));
    ui->FileListWidget->clear();
    for (directory_entry& FileToProcess : directory_iterator(InputDirectory))
    {
        if (FileToProcess.path().extension() != ".bmp" && FileToProcess.path().extension() != ".png" )
            continue;
        path PathLocal = FileToProcess.path();

        regex FilePattern(".+_top_.+");
        if ((inputType == 2) && (!regex_match(FileToProcess.path().filename().string().c_str(), FilePattern )))
            continue;

        if (!exists(PathLocal))
        {
            //Files << PathLocal.filename().string() << " File not exists" << "\n";
            QMessageBox msgBox;
            msgBox.setText((PathLocal.filename().string() + " File not exists" ).c_str());
            msgBox.exec();
            break;
        }
        ui->FileListWidget->addItem(PathLocal.filename().string().c_str());
    }
}
void MainWindow::on_spinBox_valueChanged(int arg1)
{
    threshVal = arg1;
    ProcessImage();
}

void MainWindow::on_FileListWidget_currentTextChanged(const QString &currentText)
{
    switch (inputType)
    {
    case 2:
        {
            string FileNameTop(currentText.toStdString());
            string FileNameBottom = regex_replace(FileNameTop,regex("_top_"),"_bot_");
            string FileNameSide = regex_replace(FileNameTop,regex("_top_"),"_side_");

            FileToOpen = InputDirectory;
            FileToOpen.append(FileNameTop);
            ImIn1 = imread(FileToOpen.string().c_str());

            FileToOpen = InputDirectory;
            FileToOpen.append(FileNameBottom);
            ImIn2 = imread(FileToOpen.string().c_str());
        }
        break;
    default:
        FileToOpen = InputDirectory;
        FileToOpen.append(currentText.toStdWString());
        if (!exists(FileToOpen))
            return;
        ImIn = imread(FileToOpen.string().c_str());

        if (ImIn.empty())
            return;
        break;
    }

    threshVal = ui->spinBox->value();

    ProcessImage();
}


//--------------------------------------------------------------------------------------------------
//          My functions
//--------------------------------------------------------------------------------------------------
void MainWindow::ProcessImage(void)
{
    string OutText = "";

    switch(inputType)
    {
    case 1:
        if (ImIn.empty())
            return;
        maxX = ImIn.cols;
        maxY = ImIn.rows/2;

        ImIn(Rect(0, 0, maxX, maxY)).copyTo(ImIn1);
        ImIn(Rect(0, maxY, maxX, maxY)).copyTo(ImIn2);
        break;
    case 2:
        if (ImIn1.empty())
            return;
        if (ImIn2.empty())
            return;
        maxX = ImIn1.cols;
        maxY = ImIn1.rows;
        break;
    default:
        if (ImIn.empty())
            return;
        maxX = ImIn.cols/2;
        maxY = ImIn.rows;

        ImIn(Rect(0, 0, maxX, maxY)).copyTo(ImIn1);
        ImIn(Rect(maxX, 0, maxX, maxY)).copyTo(ImIn2);
    }

    SegmentParams segParams;

    segParams.threshVal = threshVal;
    segParams.removeSmallReg = removeSmallReg;
    segParams.rawMorphErosion1 = rawMorphErosion1;
    segParams.rawMorphDilation2 = rawMorphDilation2;
    segParams.rawMorphErosion3 = rawMorphErosion3;
    segParams.fillHoles = fillHoles;
    segParams.divideSeparateReg = divideSeparateReg;
    segParams.MinRegSize = MinRegSize;
    segParams.regMorphErosion1 = regMorphErosion1;
    segParams.regMorphDilation2 = regMorphDilation2;
    segParams.regMorphErosion3 = regMorphErosion3;
    segParams.removeBorderRegion = removeBorderRegion;
    segParams.fitEllipseToReg = fitEllipseToReg;
    segParams.rotateImage = rotateImage;
    segParams.croppImage = croppImage;
    segParams.alignGrains = alignGrains;
    if(inputType == 0)
        segParams.addBlurToSecondImage = 1;
    else
        segParams.addBlurToSecondImage = 0;
    //segParams.addBlurToSecondImage = !verticalInputImages;
    segParams.findValey = findValey;
    segParams.subsegment = subsegment;

    segParams.showContour = showContour;
    segParams.showInput = showInput;
    segParams.showThesholded = showThesholded;
    segParams.show1stMorphology = show1stMorphology;
    segParams.showHolesRemoved = showHolesRemoved;
    segParams.showMask = showMask;
    segParams.showContourOnInput = showContourOnInput;
    segParams.showFitted = showFitted;
    segParams.showRotated = showRotated;
    segParams.showCropped = showCropped;
    segParams.showAreaForAlign = showAreaForAlign;
    segParams.showAligned = showAligned;
    segParams.showGradient = showGradient;
    segParams.showOutput = showOutput;
    segParams.showOutput2 = showOutput2;

    vector<Mat*> ImInVect;
    ImInVect.push_back(&ImIn1);
    ImInVect.push_back(&ImIn2);

    vector<Mat*> ImOutVect;

    vector<TransformacjaZiarna> TransfVect;
    vector <MR2DType*> RoiVect;



    steady_clock::time_point t1 = steady_clock::now();



    if(SegmentGrainImg(&ImInVect, &ImOutVect, &RoiVect, &TransfVect, &segParams))
    {
        path FileToSave = OutputDirectory;
        FileToSave.append(FileToOpen.stem().string());

        if (saveResult)
        {
            MazdaRoiIO<MR2DType>::Write((FileToSave.string() +"GrzbietROI.tif").c_str(), &RoiVect, NULL);
            imwrite((FileToSave.string() + "Grzbiet.png").c_str(),ImIn2);
        }

    }

    while(ImOutVect.size() > 0)
    {
        ImOutVect.back()->release();
        //delete ImOutVect.back();
        ImOutVect.pop_back();
    }
    while(RoiVect.size() > 0)
    {
        delete RoiVect.back();
        RoiVect.pop_back();
    }
    while(ImInVect.size() > 0)
    {
        ImInVect.back()->release();

        ImInVect.pop_back();
    }

    steady_clock::time_point t2 = steady_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    ui->DurationLineEdit->setText(  QString::number(time_span.count()));
    //ImIn.release();
    ImIn1.release();
    ImIn2.release();

}
//--------------------------------------------------------------------------------------------------
void MainWindow::ShowImageCombination(bool show, string WinName, Mat Im1, Mat Im2)
{
    if(!show)
        return;
    int imMaxX, imMaxY;
    imMaxX = Im1.cols;
    imMaxY = Im1.rows;
    Mat Im = Mat::zeros(Size(imMaxX *2,imMaxY) ,Im1.type());
    Im1.copyTo(Im(Rect(0, 0, imMaxX, imMaxY)));
    Im2.copyTo(Im(Rect(imMaxX, 0, imMaxX, imMaxY)));
    imshow(WinName.c_str(), Im);

}
//--------------------------------------------------------------------------------------------------
void MainWindow::ShowImageRegionCombination(bool show, bool showContour, string WinName, Mat Im1, Mat Im2, Mat Mask1, Mat Mask2)
{
    if(!show)
        return;
    int imMaxX, imMaxY;
    imMaxX = Im1.cols;
    if (imMaxX < Im2.cols)
        imMaxX = Im2.cols;
    imMaxY = Im1.rows;
    if (imMaxY < Im2.rows)
        imMaxY = Im2.rows;
    Mat Im = Mat::zeros(Size(imMaxX *2,imMaxY) ,Im1.type());
    Im1.copyTo(Im(Rect(0, 0, Im1.cols , Im1.rows)));
    Im2.copyTo(Im(Rect(imMaxX, 0, Im2.cols , Im2.rows)));

    Mat Mask = Mat::zeros(Size(imMaxX *2,imMaxY) ,Mask1.type());
    Mask1.copyTo(Mask(Rect(0, 0, Im1.cols , Im1.rows)));
    Mask2.copyTo(Mask(Rect(imMaxX, 0, Im2.cols , Im2.rows)));

    if(showContour)
    {
        Mask = GetContour5(Mask);
    }
    imshow(WinName.c_str(), ShowSolidRegionOnImage(Mask,Im));

}
//--------------------------------------------------------------------------------------------------
void MainWindow::ShowImageRegionCombination(bool show, bool showContour, string WinName, Mat Im, Mat Mask1, Mat Mask2)
{
    if(!show)
        return;
    int imMaxX, imMaxY;
    imMaxX = Mask1.cols;
    imMaxY = Mask1.rows;

    Mat Mask = Mat::zeros(Size(imMaxX *2,imMaxY) ,Mask1.type());
    Mask1.copyTo(Mask(Rect(0, 0, imMaxX, imMaxY)));
    Mask2.copyTo(Mask(Rect(imMaxX, 0, imMaxX, imMaxY)));

    if(showContour)
    {
        Mask = GetContour5(Mask);
    }
    imshow(WinName.c_str(), ShowSolidRegionOnImage(Mask,Im));

}
//--------------------------------------------------------------------------------------------------
void MainWindow::ShowHLinesOnImage(bool show, string WinName, Mat Im, int lineU, int lineCU, int lineCL, int lineL)
{
    if(!show)
        return;


    Mat ImShow;
    Im.copyTo(ImShow);
    line(ImShow,Point(0,lineU),Point(ImShow.cols,lineU),CV_RGB(255,0,0),1);
    line(ImShow,Point(0,lineCU),Point(ImShow.cols,lineCU),CV_RGB(255,0,0),1);
    line(ImShow,Point(0,lineCL),Point(ImShow.cols,lineCL),CV_RGB(0,255,0),1);
    line(ImShow,Point(0,lineL),Point(ImShow.cols,lineL),CV_RGB(0,255,0),1);


    imshow(WinName.c_str(), ImShow);

}
//--------------------------------------------------------------------------------------------------
void MainWindow::ShowHLinesOnImage(bool show, string WinName, Mat Im1, Mat Im2, int lineU, int lineCU, int lineCL, int lineL)
{
    if(!show)
        return;
    int imMaxX = Im1.cols;
    if(imMaxX < Im2.cols)
        imMaxX = Im2.cols;
    int imMaxY = Im1.rows;
    if(imMaxY < Im2.rows)
        imMaxY = Im2.rows;

    Mat ImShow = Mat::zeros(Size(imMaxX *2,imMaxY) ,Im1.type());
    Im1.copyTo(ImShow(Rect(0, 0, Im1.cols, Im1.rows)));
    Im2.copyTo(ImShow(Rect(imMaxX, 0, Im2.cols, Im2.rows)));

    //Mat ImShow;
    //Im.copyTo(ImShow);
    line(ImShow,Point(0,lineU),Point(ImShow.cols,lineU),CV_RGB(255,0,0),1);
    line(ImShow,Point(0,lineCU),Point(ImShow.cols,lineCU),CV_RGB(255,0,0),1);
    line(ImShow,Point(0,lineCL),Point(ImShow.cols,lineCL),CV_RGB(0,255,0),1);
    line(ImShow,Point(0,lineL),Point(ImShow.cols,lineL),CV_RGB(0,255,0),1);


    imshow(WinName.c_str(), ImShow);

}
//--------------------------------------------------------------------------------------------------
Mat MainWindow::Combine2Images( Mat Im1, Mat Im2)
{
    int imMaxX, imMaxY;
    imMaxX = Im1.cols;
    if (imMaxX < Im2.cols)
        imMaxX = Im2.cols;
    imMaxY = Im1.rows;
    if(imMaxY < Im2.rows)
        imMaxY = Im2.rows;
    Mat Im = Mat::zeros(Size(imMaxX *2,imMaxY) ,Im1.type());
    Im1.copyTo(Im(Rect(0, 0, Im1.cols, Im1.rows)));
    Im2.copyTo(Im(Rect(imMaxX, 0, Im2.cols, Im2.rows)));

    return Im;
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

    if(showAreaForAlign)
        namedWindow("AreaForAlign", displayFlag);

    if(showGradient)
        namedWindow("Gradient", displayFlag);

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


void MainWindow::on_ShowComboBox_currentIndexChanged(int index)
{
    if(index)
        showContour = true;
    else
        showContour = false;

    ProcessImage();
}

void MainWindow::on_AlignCheckBox_toggled(bool checked)
{
    alignGrains = checked;
    ProcessImage();
}

void MainWindow::on_ShowAreaForAlignCheckBox_toggled(bool checked)
{
    showAreaForAlign = checked;
    if(showAreaForAlign)
        namedWindow("AreaForAlign", displayFlag);
    else
        destroyWindow("AreaForAlign");

    ProcessImage();
}

void MainWindow::on_ShowAlignedCheckBox_toggled(bool checked)
{
    showAligned = checked;
    if(showAligned)
        namedWindow("Aligned", displayFlag);
    else
        destroyWindow("Aligned");

    ProcessImage();
}


void MainWindow::on_FindValeyCheckBox_toggled(bool checked)
{
    findValey = checked;
    ProcessImage();
}

void MainWindow::on_ShowGradientCheckBox_toggled(bool checked)
{
    showGradient = checked;
    if(showGradient)
        namedWindow("Gradient", displayFlag);
    else
        destroyWindow("Gradient");

    ProcessImage();
}

void MainWindow::on_ShowOutputCheckBox_toggled(bool checked)
{
    showOutput = checked;
    if(showOutput)
        namedWindow("Output", displayFlag);
    else
        destroyWindow("Output");

    ProcessImage();
}

void MainWindow::on_SelOutFolderPushButton_clicked()
{
    QFileDialog dialog(this, "Open Folder");
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setDirectory(InputDirectory.string().c_str());

    //QStringList FileList= dialog.e
    if(dialog.exec())
    {
        OutputDirectory = dialog.directory().path().toStdWString();
    }
    else
         return;

    //InputDirectory = dialog.getExistingDirectory().toStdWString();//  toStdString());
    if (!exists(OutputDirectory))
    {
        QMessageBox msgBox;
        msgBox.setText((OutputDirectory.string()+ " not exists ").c_str());
        msgBox.exec();
        OutputDirectory = "c:\\";
    }
    if (!is_directory(OutputDirectory))
    {
        QMessageBox msgBox;
        msgBox.setText((OutputDirectory.string()+ " This is not a directory path ").c_str());
        msgBox.exec();
        OutputDirectory = "c:\\";
    }
    //Files.clear();
    ui->OutDirectoryLineEdit->setText(QString::fromWCharArray(OutputDirectory.wstring().c_str()));
}

void MainWindow::on_pushButtonConvertAll_clicked()
{
    for (directory_entry& FileToProcess : directory_iterator(InputDirectory))
    {
        //path InPath = FileToProcess.path();

        // check if the filename follows the input regular expression
//		if (!regex_match(InPath.filename().string().c_str(), FilePattern))
//			continue;



        //Mat ImIn = imread(InPath.string(), CV_LOAD_IMAGE_ANYDEPTH);
        //ImIn.release();
        //ImIn = imread(InPath.string(), CV_LOAD_IMAGE_ANYDEPTH);
        // check if it is an image file



        FileToOpen = FileToProcess.path();

        if (!exists(FileToOpen))
            continue;
        ImIn = imread(FileToOpen.string().c_str());
        if (ImIn.empty())
            continue;
        maxX = ImIn.cols/2;
        maxY = ImIn.rows;
        ImIn(Rect(0, 0, maxX, maxY)).copyTo(ImIn1);
        ImIn(Rect(maxX, 0, maxX, maxY)).copyTo(ImIn2);

        threshVal = ui->spinBox->value();

        ProcessImage();
    }
}

void MainWindow::on_ShowCroppedCheckBox_2_toggled(bool checked)
{
    showCropped = checked;
    if(showCropped)
        namedWindow("Cropped", displayFlag);
    else
        destroyWindow("Cropped");

    ProcessImage();
}

void MainWindow::on_VerticalDivisionCheckBox_toggled(bool checked)
{
    verticalInputImages = checked;
    ProcessImage();
}

void MainWindow::on_SaveResultCheckBox_toggled(bool checked)
{
    saveResult = checked;
    ProcessImage();
}

void MainWindow::on_ShowOutput2CheckBox_toggled(bool checked)
{

    showOutput2 = checked;
    if(showOutput2)
        namedWindow("Output2", displayFlag);
    else
        destroyWindow("Output2");

    ProcessImage();

}

void MainWindow::on_SubsegmentCheckBox_toggled(bool checked)
{
    subsegment = checked;
    ProcessImage();
}

void MainWindow::on_InputTypecomboBox_currentIndexChanged(int index)
{
    inputType = index;
    ProcessImage();
}
