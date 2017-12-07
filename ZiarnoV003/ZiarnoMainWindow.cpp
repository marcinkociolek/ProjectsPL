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
#include "heightfromsideimage.h"

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

//#define TERAZ_DEBUG

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

    MinRegSize = ui->MinRegionSizeSpinBox->value();

    ui->RawRegionMorfology1ComboBox->addItem("none");
    ui->RawRegionMorfology1ComboBox->addItem("erode5");
    ui->RawRegionMorfology1ComboBox->addItem("erode9");
    ui->RawRegionMorfology1ComboBox->addItem("erode13");
    ui->RawRegionMorfology1ComboBox->setCurrentIndex(2);

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
    inputType =             ui->InputTypecomboBox->currentIndex();
    verticalInputImages = ui->VerticalDivisionCheckBox->checkState();
    segmentType         = ui->SegmentationComboBox->currentIndex();
    threshVal           = ui->spinBox->value();
    threshVal3          = ui->spinBoxThreshVal3->value();

    zeroOutsideEllipse  = ui->ZeroMaskOutsideEllipseRegionscheckBox->checkState();
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

    temp                = ui->TempCheckBox->checkState();

    saveResult          =ui->SaveResultCheckBox->checkState();

    // display options
    showThirdImage      = ui->ShowThirdCheckBox->checkState();

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


    InputDirectory = "C:\\Data\\Ziarno\\2017.08.30\\";
    OutputDirectory = "c:\\";

    ImStack = 0;
    imStackCount = 0;
    channelCount = 0;

    OnOffImageWindow();
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

        regex FilePattern(".+_T.+");
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
            CurrentFileName = currentText.toStdString();
            string FileNameTop(CurrentFileName);
            string FileNameBottom = regex_replace(CurrentFileName,regex("_T"),"_B");
            string FileNameSide = regex_replace(CurrentFileName,regex("_T"),"_S");

            FileToOpen = InputDirectory;
            FileToOpen.append(FileNameTop);
            ImIn1 = imread(FileToOpen.string().c_str());

            FileToOpen = InputDirectory;
            FileToOpen.append(FileNameBottom);
            ImIn2 = imread(FileToOpen.string().c_str());

            FileToOpen = InputDirectory;
            FileToOpen.append(FileNameSide);
            ImIn3 = imread(FileToOpen.string().c_str());
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

    //threshVal = ui->spinBox->value();

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
        if (ImIn1.empty()||ImIn2.empty())
        {
            string FileNameTop = CurrentFileName;
            string FileNameBottom = regex_replace(CurrentFileName,regex("_T"),"_B");
            string FileNameSide = regex_replace(CurrentFileName,regex("_T"),"_S");

            FileToOpen = InputDirectory;
            FileToOpen.append(FileNameTop);
            ImIn1 = imread(FileToOpen.string().c_str());

            FileToOpen = InputDirectory;
            FileToOpen.append(FileNameBottom);
            ImIn2 = imread(FileToOpen.string().c_str());

            FileToOpen = InputDirectory;
            FileToOpen.append(FileNameSide);
            ImIn3 = imread(FileToOpen.string().c_str());

            if (ImIn1.empty()||ImIn2.empty()||ImIn3.empty())
                return;
        }
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

#ifdef TERAZ_DEBUG
    SegmentParams segParams;

    segParams.threshVal = threshVal;
    //segParams.threshVal3 = threshVal3;
    segParams.zeroOutsideEllipse = zeroOutsideEllipse;
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
    //segParams.subsegment = subsegment;

    segParams.temp = temp;

    //segParams.showThird = showThirdImage;

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
    //segParams.showOutput2 = showOutput2;

#endif

    vector<Mat*> ImInVect;
    ImInVect.push_back(&ImIn1);
    ImInVect.push_back(&ImIn2);
    //ImInVect.push_back(&ImIn3);

    vector<Mat*> ImOutVect;

    vector<TransformacjaZiarna> TransfVect;
    vector <MR2DType*> RoiVect;



    //steady_clock::time_point t1 = steady_clock::now();
    //int startTime = getTickCount();

    int orientation;
    bool done;
#ifdef TERAZ_DEBUG

    done = SegmentGrainImg(&ImInVect, &ImOutVect, &RoiVect, &TransfVect, &orientation, &segParams);
#else
    done = SegmentGrainImg(&ImInVect, &ImOutVect, &RoiVect, &TransfVect, &orientation);//, &segParams);
    ShowImageCombination(showInput,"Input image",*ImInVect.at(0), *ImInVect.at(1));
    if(done)
    {

        //ShowImageCombination(showOutput,"Output",*ImOutVect.at(0), *ImOutVect.at(1));
        Mat MaskTemp1 = Mat::zeros((*ImOutVect.at(0)).rows,(*ImOutVect.at(0)).cols,CV_16U);
        MazdaRoiIterator<MR2DType> iterator1(RoiVect.at(0));
        unsigned short *wMaskTemp1 = (unsigned short*)MaskTemp1.data;
        int reg1 = 3;
        if((RoiVect.at(0))->GetName() == "V")
            reg1 = 1;
        if((RoiVect.at(0))->GetName() == "D")
            reg1 = 2;


        while(! iterator1.IsBehind())
        {
            if (iterator1.GetPixel())
                *wMaskTemp1 = reg1;
            ++iterator1;
           wMaskTemp1++;
        }

        Mat MaskTemp2 = Mat::zeros((*ImOutVect.at(1)).rows,(*ImOutVect.at(1)).cols,CV_16U);
        MazdaRoiIterator<MR2DType> iterator2(RoiVect.at(1));
        unsigned short *wMaskTemp2 = (unsigned short*)MaskTemp2.data;
        int reg2 = 3;
        if((RoiVect.at(1))->GetName() == "V")
            reg2 = 1;
        if((RoiVect.at(1))->GetName() == "D")
            reg2 = 2;
        while(! iterator2.IsBehind())
        {
            if (iterator2.GetPixel())
                *wMaskTemp2 = reg2;
            ++iterator2;
           wMaskTemp2++;
        }
        ShowImageRegionCombination(showOutput, showContour, "Output", *ImOutVect.at(0), *ImOutVect.at(1), MaskTemp1, MaskTemp2);

    }
    else
        ui->MesageTextEdit->append((CurrentFileName + "\n").c_str());

#endif
    //steady_clock::time_point t2 = steady_clock::now();
    //duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    //int stopTime = getTickCount();




    //Mask3 = FindMaskFromGray(ImIn3,params->threshVal3);
    //int firstLine = FindGrainHeighOnBRG(ImIn3, threshVal3);

    //int firstLine = FindGrainHeighOnBRG(ImIn3);
    cvtColor(ImIn3,ImIn3, CV_BGR2GRAY);
    int startTime = getTickCount();
    int firstLine = FindGrainHeigh(ImIn3);
    int stopTime = getTickCount();
    ui->DurationLineEdit->setText(  QString::number((double)(stopTime - startTime)/1000.0));


    if(showThirdImage)
    {
        Mat ImShow = ImIn3;
        line(ImShow,Point(0,firstLine),Point(ImShow.cols,firstLine),Scalar(128,0,0));
        imshow("Thresholded 3",ImShow);
    }
    if(done)
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


    //ImIn.release();
    ImIn1.release();
    ImIn2.release();
    ImIn3.release();

}
//--------------------------------------------------------------------------------------------------
void MainWindow::ShowImageCombination(bool show, string WinName, Mat Im1, Mat Im2)
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
    {
        namedWindow("Input image", displayFlag);
        if(showThirdImage)
            namedWindow("Input image 3", displayFlag);
    }
    else
    {
        destroyWindow("Input image");
        destroyWindow("Input image 3");
    }

    ProcessImage();
}

void MainWindow::on_ShowThresholdedImgeCheckBox_toggled(bool checked)
{
    showThesholded = checked;
    if(showThesholded)
    {
        namedWindow("Thresholded", displayFlag);

    }
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
        namedWindow("Output", WINDOW_AUTOSIZE);
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
        ImIn1 = imread(FileToOpen.string().c_str());
        if (ImIn.empty())
            continue;
        maxX = ImIn.cols/2;
        maxY = ImIn.rows;
        ImIn(Rect(0, 0, maxX, maxY)).copyTo(ImIn1);
        ImIn(Rect(maxX, 0, maxX, maxY)).copyTo(ImIn2);

        //threshVal = ui->spinBox->value();

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

void MainWindow::on_ShowThirdCheckBox_toggled(bool checked)
{
    showThirdImage = checked;
    if(showThirdImage)
        namedWindow("Thresholded 3", displayFlag);
    else
        destroyWindow("Thresholded 3");
    ProcessImage();
}

void MainWindow::on_spinBoxThreshVal3_valueChanged(int arg1)
{
    threshVal3 = arg1;
    ProcessImage();
}

void MainWindow::on_ZeroMaskOutsideEllipseRegionscheckBox_toggled(bool checked)
{
    zeroOutsideEllipse = checked;
    ProcessImage();
}

void MainWindow::on_TempCheckBox_toggled(bool checked)
{
    temp = checked;
    ProcessImage();
}

void MainWindow::on_pushButtonGetBackground_clicked()
{
    //for (int i = 0; i< imStackCount; i++)
    //{
    //    delete[] ImStack[i];
    //}
    delete[] ImStack;
    imStackCount = 0;
    channelCount = 3;


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


    vector<Mat> ImTVect;
    vector<Mat> ImBVect;
    vector<Mat> ImSVect;

    for (directory_entry& FileToProcess : directory_iterator(InputDirectory))
    {
        if (FileToProcess.path().extension() != ".bmp" && FileToProcess.path().extension() != ".png" )
            continue;
        regex FilePattern(".+_0.+");
        //regex FilePattern(".+_T.+");
        if ((inputType == 2) && (!regex_match(FileToProcess.path().filename().string().c_str(), FilePattern )))
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

        string FileNameTop = PathLocal.string();
        //string FileNameBottom = regex_replace(FileNameTop,regex("_T"),"_B");
        //string FileNameSide = regex_replace(FileNameTop,regex("_T"),"_S");
        string FileNameBottom = regex_replace(FileNameTop,regex("_0"),"_1");
        string FileNameSide = regex_replace(FileNameTop,regex("_0"),"_2");

        Mat ImT = imread(FileNameTop);

        Mat ImB = imread(FileNameBottom);

        Mat ImS = imread(FileNameSide);

        if (ImT.empty()||ImB.empty()||ImB.empty())
        {
            continue;
        }

        ImTVect.push_back(ImT);
        ImBVect.push_back(ImB);
        ImSVect.push_back(ImS);
        imStackCount++;
    }
    if(!imStackCount)
    {
        return;
    }

    ImStack = new Mat[imStackCount*channelCount];

    //int *Liczby = new int[3];
    //int **Liczby2D = new int*[6];

    for(int i = 0; i < imStackCount; i++)
    {
        //Mat *GrainImStack = new Mat[channelCount];
        if(channelCount)
            ImTVect.at(i).copyTo(ImStack[i*channelCount + 0]);
        if(channelCount>1)
            ImBVect.at(i).copyTo(ImStack[i*channelCount + 1]);
        if(channelCount>2)
            ImSVect.at(i).copyTo(ImStack[i*channelCount + 2]);
        //ImStack[i] = GrainImStack;
    }
    ImTVect.empty();
    ImBVect.empty();
    ImSVect.empty();
    imshow("Stack Image",ImStack[ui->spinBoxStackIndex->value() *channelCount + ui->spinBoxImIndex->value()]);

    int dirtyPixelsPerImage = ComputeBackgroundImages(ImStack, imStackCount, channelCount);
    string Message = "dirty pixels per image = " + to_string(dirtyPixelsPerImage)+ "\n";
    ui->MesageTextEdit->append( Message.c_str());
}

void MainWindow::on_spinBoxStackIndex_valueChanged(int arg1)
{
    if (arg1<imStackCount)
        imshow("Stack Image",ImStack[ui->spinBoxStackIndex->value() * channelCount + ui->spinBoxImIndex->value()]);
}

void MainWindow::on_spinBoxImIndex_valueChanged(int arg1)
{
    if (arg1<3)
        imshow("Stack Image",ImStack[ui->spinBoxStackIndex->value() * channelCount + ui->spinBoxImIndex->value()]);
}
