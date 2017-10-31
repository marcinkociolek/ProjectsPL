#ifndef ZIARNOMAINWINDOW_H
#define ZIARNOMAINWINDOW_H

#include <boost/filesystem.hpp>

#include <opencv2/core/core.hpp>

#include <QMainWindow>


using namespace boost::filesystem;
//using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    std::string CurrentFileName;
    path FileToOpen;
    path CurrentDir;

    cv::Mat ImShow;
    cv::Mat ImIn;
    //IplImage ImIn;
    cv::Mat ImIn1;
    cv::Mat ImIn2;
    cv::Mat ImIn3;

    cv::Mat ImMask1;
    cv::Mat ImMask2;

    cv::Mat Contour1;
    cv::Mat Contour2;

    int maxX;
    int maxY;

    path InputDirectory;
    path OutputDirectory;


    // analisis options
    bool verticalInputImages;
    int inputType;
    int segmentType;
    float threshVal;
    int threshVal3;
    bool zeroOutsideEllipse;
    bool removeSmallReg;
    int rawMorphErosion1;
    int rawMorphDilation2;
    int rawMorphErosion3;
    bool fillHoles;
    bool divideSeparateReg;
    int MinRegSize;
    int regMorphErosion1;
    int regMorphDilation2;
    int regMorphErosion3;
    bool removeBorderRegion;
    bool alignGrains;
    bool findValey;

    bool fitEllipseToReg;
    bool rotateImage;
    bool croppImage;
    bool subsegment;

    bool saveResult;

    bool temp;

    // display options
    int displayFlag;

    bool showContour;

    bool showThirdImage;

    bool showInput;
    bool showThesholded;
    bool show1stMorphology;
    bool showHolesRemoved;
    bool showMask;
    bool showContourOnInput;
    bool showFitted;
    bool showRotated;
    bool showCropped;
    bool showAreaForAlign;
    bool showAligned;
    bool showGradient;
    bool showOutput;
    bool showOutput2;



    void ProcessImage(void);
    void ShowImageCombination(bool show, std::string WinName, cv::Mat Im1, cv::Mat Im2);

    cv::Mat  Combine2Images(cv::Mat Im1, cv::Mat Im2);
    void ShowImageRegionCombination(bool show, bool contour, std::string WinName, cv::Mat Im1, cv::Mat Im2, cv::Mat Mask1, cv::Mat Mask2);
    void ShowImageRegionCombination(bool show, bool showContour, std::string WinName, cv::Mat Im, cv::Mat Mask1, cv::Mat Mask2);
    void ShowHLinesOnImage(bool show, std::string WinName, cv::Mat Im, int lineU, int lineCU, int lineCL, int lineL);
    void ShowHLinesOnImage(bool show, std::string WinName, cv::Mat Im1, cv::Mat Im2, int lineU, int lineCU, int lineCL, int lineL);
    void OnOffImageWindow(void);

private slots:
    void on_pushButton_clicked();

    //void on_FileTreeView_clicked(const QModelIndex &index);





    void on_spinBox_valueChanged(int arg1);

    void on_FileListWidget_currentTextChanged(const QString &currentText);

    void on_RemoveSmalRegionscheckBox_stateChanged(int arg1);

    void on_MinRegionSizeSpinBox_valueChanged(int arg1);

    void on_SegmentationComboBox_currentIndexChanged(int index);

    //void on_RemoveSmalRegionscheckBox_toggled(bool checked);

    void on_RawRegionMorfology1ComboBox_currentIndexChanged(int index);

    void on_RawRegionMorfology2ComboBox_currentIndexChanged(int index);

    void on_RawRegionMorfology3ComboBox_currentIndexChanged(int index);

    void on_FillHolesCheckBox_toggled(bool checked);

    void on_DivideRegionsCheckBox_toggled(bool checked);

    void on_RegionMorfology1ComboBox_currentIndexChanged(int index);

    void on_RegionMorfology2ComboBox_currentIndexChanged(int index);

    void on_RegionMorfology3ComboBox_currentIndexChanged(int index);

    void on_ShowInputImageCheckBox_toggled(bool checked);

    void on_ShowThresholdedImgeCheckBox_toggled(bool checked);

    void on_ShowAfter1MorphologyCheckBox_toggled(bool checked);

    void on_ShowWithoutHolesCheckBox_toggled(bool checked);


    void on_pushButtonRegex_clicked();

    void on_ShowMaskCheckBox_toggled(bool checked);

    void on_ShowContourOnImageCheckBox_toggled(bool checked);

    void on_AllowResizeCheckBox_toggled(bool checked);

    void on_RemoveBorderRegionCheckBox_toggled(bool checked);

    void on_FitEllipseCheckBox_toggled(bool checked);

    void on_RotateCheckBox_toggled(bool checked);

    void on_ShowFitedElipseCheckBox_toggled(bool checked);

    void on_ShowRotatedCheckBox_toggled(bool checked);

    void on_CroppCheckBox_toggled(bool checked);

    void on_ShowComboBox_currentIndexChanged(int index);

    void on_AlignCheckBox_toggled(bool checked);

    void on_ShowAreaForAlignCheckBox_toggled(bool checked);

    void on_ShowAlignedCheckBox_toggled(bool checked);

    void on_FindValeyCheckBox_toggled(bool checked);

    void on_ShowGradientCheckBox_toggled(bool checked);

    void on_ShowOutputCheckBox_toggled(bool checked);

    void on_SelOutFolderPushButton_clicked();

    void on_pushButtonConvertAll_clicked();

    void on_ShowCroppedCheckBox_2_toggled(bool checked);

    void on_VerticalDivisionCheckBox_toggled(bool checked);

    void on_SaveResultCheckBox_toggled(bool checked);

    void on_ShowOutput2CheckBox_toggled(bool checked);

    void on_SubsegmentCheckBox_toggled(bool checked);

    void on_InputTypecomboBox_currentIndexChanged(int index);

    void on_ShowThirdCheckBox_toggled(bool checked);

    void on_spinBoxThreshVal3_valueChanged(int arg1);

    void on_ZeroMaskOutsideEllipseRegionscheckBox_toggled(bool checked);

    void on_TempCheckBox_toggled(bool checked);

    void on_pushButtonGetBackground_clicked();

private:
    Ui::MainWindow *ui;
};

//--------------------------------------------------------------------------------------------------
//          My functions
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
#endif // ZIARNOMAINWINDOW_H
