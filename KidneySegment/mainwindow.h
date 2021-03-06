#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <opencv2/core/core.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
// my definitions--------------------------------------------------------------------------------------------
    boost::filesystem::path InputDirectory;
    boost::filesystem::path ExportDirectory;

    boost::filesystem::path ROIFile;
    int frameNr;
    std::string CurrentFrame00FileName;
    cv::Mat InIm;
    cv::Mat ImGradient;
    cv::Mat ImShowGray;
    cv::Mat ImShowPseudoColor;
    cv::Mat ImShowGradient;

    cv::Mat MaskKidney1;
    cv::Mat MaskCortex1;
    cv::Mat MaskPelvis1;
    cv::Mat MaskMedula1;
    cv::Mat MaskKidney2;
    cv::Mat MaskCortex2;
    cv::Mat MaskPelvis2;
    cv::Mat MaskMedula2;

    int prevPosX;
    int prevPosY;

    int maxX;
    int maxY;

    int minShow;
    int maxShow;

    bool showGray;
    int minShowGray;
    int maxShowGray;
    int showModeGray;
    //bool showRegionOnGray;
    //bool showContourOnGray;

    bool showPseudoColor;
    int minShowPseudoColor;
    int maxShowPseudoColor;
    int showModePseudoColor;
    //bool showRegionOnPseudoColor;
    //bool showContourOnPseudoColor;

    bool showGradient;
    int minShowGradient;
    int maxShowGradient;
    int showModeGradient;
    //bool showRegionOnGradient;
    //bool showContourOnGradient;

    int imageShowScale;

    bool showPelvis;
    bool showCortex;
    bool showMedula;

    int penSize;

    bool displayFlag;
    int transparency;

    int minThresh;
    int maxThresh;


    cv::Mat Mask;
    cv::Mat Mask2;
    int regionIndex;


    bool moveAll;
    bool moveAllL;
    bool moveAllR;

// my functions
    void ProcessFile();
    void PrepareShowImages();
    void ShowImages();
    void ScaleImages();
    void PlaceShapeOnImage(cv::Mat Im, int x, int y, int size, unsigned short val);
    void PlaceLineOnImage(cv::Mat Im, int x, int y, int size,unsigned short val);

    void OnOffImageWindow(void);
    cv::Mat SegmetU16BetweentwoThresholds(cv::Mat Im, unsigned short threshMin, unsigned short threshMax);

    void LoadROI(boost::filesystem::path InputFile);
    void SaveROI(boost::filesystem::path OutputFile);

//SLOTS------------------------------------------------------------------------------------------------------
private slots:
    void on_widgetImage_mousePressed(QPoint point);

    void on_widgetImage_mouseMoved(QPoint point, int butPressed);

    void on_widgetImage_KeyPressed(int key);

    void on_OpenFolderPushButton_clicked();

    void on_InFileListWidget_currentTextChanged(const QString &currentText);

    void on_spinBoxFrameNr_valueChanged(int arg1);

    void on_spinBoxMinShow_valueChanged(int arg1);

    void on_spinBoxMaxShow_valueChanged(int arg1);

    void on_spinBoxImageScale_valueChanged(int arg1);

    void on_checkBoxShowGray_toggled(bool checked);

    void on_checkBoxShowPseudoColor_toggled(bool checked);

    void on_checkBoxShowGradient_toggled(bool checked);

    void on_pushButtonGetPixelValues_clicked();

    void on_comboBoxRegioNr_currentIndexChanged(int index);



    void on_spinBoxMinShowGray_valueChanged(int arg1);

    void on_spinBoxMaxShowGray_valueChanged(int arg1);

    void on_spinBoxMinShowPseudoColor_valueChanged(int arg1);

    void on_spinBoxMaxShowPseudoColor_valueChanged(int arg1);

    void on_spinBoxMinShowGradient_valueChanged(int arg1);

    void on_spinBoxMaxShowGradient_valueChanged(int arg1);

    void on_comboBoxShowModeGray_currentIndexChanged(int index);

    void on_comboBoxShowModePseudoColor_currentIndexChanged(int index);

    void on_comboBoxShowModeGradient_currentIndexChanged(int index);

    void on_pushButtonFillHoles_pressed();

    void on_checkBoxShowPelvis_toggled(bool checked);

    void on_checkBoxShowCortex_toggled(bool checked);

    void on_checkBoxShowMedula_toggled(bool checked);

    void on_pushButtonUp_clicked();

    void on_pushButtonDown_clicked();

    void on_pushButtonRight_clicked();

    void on_pushButtonLeft_clicked();

    void on_spinBoxPenSize_valueChanged(int arg1);

    void on_spinBoxTransparency_valueChanged(int arg1);

    void on_pushButtonSaveRoiCommon_clicked();

    void on_pushButtonLoadRoiCommon_clicked();

    void on_pushButtonDeleteReg_clicked();

    void on_pushButtonLoadCommonP_clicked();

    void on_pushButtonLoadCommonM_clicked(bool checked);

    void on_pushButtonDeleteAll_clicked();

    void on_pushButtonSaveRoi_clicked();

    void on_checkBoxMoveAll_toggled(bool checked);

    void on_checkBoxMoveL_toggled(bool checked);

    void on_checkBoxMoveP_toggled(bool checked);

    void on_pushButtonSaveOutBMP_clicked();


    void on_checkBoxShowLeft_toggled(bool checked);

    void on_checkBoxShowRight_toggled(bool checked);

    void on_pushButtonOpenExportFolder_clicked();

    void on_pushButtonExport1_clicked();

    void on_pushButtonExport2_clicked();

    void on_pushButtonExport3_clicked();

private:
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
