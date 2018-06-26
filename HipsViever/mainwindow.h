#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <boost/filesystem.hpp>

#include <opencv2/core/core.hpp>


#include <QMainWindow>

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
    boost::filesystem::path FileToOpen;
    boost::filesystem::path CurrentDir;
    boost::filesystem::path InputDirectory;

    //cv::Mat ImShow;
    cv::Mat ImIn;
    cv::Mat ImConv;
    cv::Mat ImSDA;
    cv::Mat ImNormInvSDA;
    cv::Mat ImGradient;
    cv::Mat MaskImplant;
    cv::Mat Mask;

    cv::Mat MaskSDARef;

    cv::Mat MaskSDA;
    cv::Mat ImOut;

    cv::Mat ImShowGray;
    cv::Mat ImShowPseudocolor;
    cv::Mat ImShowSDA;

    cv::Mat ImShowOutput;
    cv::Mat ImShowOutputOnSDA;

    int maxX;
    int maxY;
    int maxXY;

    int minShowGray;
    int maxShowGray;

    int minShowPseudocolor;
    int maxShowPseudocolor;
    int minShowGradient;
    int maxShowGradient;

    int minShowSDA;
    int maxShowSDA;

    int minFlood;
    int maxFlood;

    unsigned short thresholdImOrg;
    unsigned short thresholdGradient;

    bool showInputGray;
    bool showInputPseudocolor;

    bool showGradient;

    bool showMaskImplant;
    bool showSDA;
    bool showSDANorm;
    bool showSDANormPC;

    bool showSDAThresholded;

    bool showConv;
    bool showMask;

    bool showContour;

    bool showRef;

    int closingShape;

    bool fillHoles;
    bool divideSeparateRegions;


    int minRegionSize;

    int erosionShape;

    bool expandMask;
    bool expandMaskY;
    int expansionSize;

    bool croppMask;
    int croppSize;

    int sdaSize;
    bool calculateSDA;
    int kernelSizeSDA;
    int kernelPixelCountSDA;

    bool thresholdSDA;
    int thresholdImSDA;

    int postErosionShape1;
    int postDilationShape2;
    int postErosionShape3;

    bool fillHolesOnOutMask;

    bool divideSeparateRegionsSDA;
    int minRegionSizeSDA;

    bool findRegTouchingImplant;

    bool showOutput;
    bool showOutputOnSDA;

    //int kernelPixelCountSDA;

    int transparency;
    int kernelSizeSDARef;

    bool stopDisplay;

    bool displayFlag;

    double jaccard;

    std::string OutString;
    std::string OutJaccard;
    std::string OutThreshold;
    std::string LocalString;

    std:: string FilePatternStr;

    void MainWindow::CalculateSDA(void);
    //cv::Mat MainWindow::CalculateSDA(cv::Mat imIn, cv::Mat Roi, int radius);
    void MainWindow::OpenImage(void);
    void MainWindow::ShowImages(void);

    void MainWindow::MaskImage(void);
    void MainWindow::ShowMask(void);
    void MainWindow::EstymateSDA(void);
    void MainWindow::ShowSDA(void);
    void MainWindow::PostSDA(void);
    void MainWindow::ShowResults(void);
    void MainWindow::ProcessImage(void);

    void MainWindow::ShowAll(void);
    void MainWindow::OnOffImageWindow(void);

    void MainWindow::RefreshFileList();

    std::string MainWindow::ParamsToString(void);

private slots:
    void on_pushButtonSelectInFolder_clicked();

    void on_ListWidgetFiles_currentTextChanged(const QString &currentText);

    void on_spinBoxMinShowGray_valueChanged(int arg1);

    void on_spinBoxMaxShowGray_valueChanged(int arg1);

    void on_CheckBoxShowInputImageGray_toggled(bool checked);

    void on_CheckBoxShowInputImagePC_toggled(bool checked);

    void on_spinBoxMinShowPseudoColor_valueChanged(int arg1);

    void on_spinBoxMaxShowPseudoColor_valueChanged(int arg1);

    void on_spinBoxMinShowGradient_valueChanged(int arg1);

    void on_spinBoxMaxShowGradient_valueChanged(int arg1);

    void on_CheckBoxShowGradient_toggled(bool checked);

    void on_spinBoxThresholdOryginalImage_valueChanged(int arg1);

    void on_CheckBoxShowMask_toggled(bool checked);

    void on_CheckBoxShowConv_toggled(bool checked);

    void on_spinBoxMinShowConv_valueChanged(int arg1);

    void on_spinBoxMaxShowConv_valueChanged(int arg1);

    void on_spinBoxMinFlood_valueChanged(int arg1);

    void on_spinBoxMaxFlood_valueChanged(int arg1);

    void on_CheckBoxAllowResize_toggled(bool checked);

    void on_spinBoxThresholdGradient_valueChanged(int arg1);

    void on_CheckBoxFillHoles_toggled(bool checked);

    void on_CheckBoxDivideseparateregions_toggled(bool checked);

    void on_spinBoxMinRegionSize_valueChanged(int arg1);

    void on_CheckBoxExpandMask_toggled(bool checked);

    void on_spinBoxExpansionSize_valueChanged(int arg1);

    void on_CheckBoxCroppMask_toggled(bool checked);

    void on_spinBoxCroppSize_valueChanged(int arg1);

    void on_CheckBoxThresholdSDA_toggled(bool checked);

    void on_spinBoxThresholdSDA_valueChanged(int arg1);

    void on_CheckBoxCalculateSDA_toggled(bool checked);

    void on_spinBoxSDAKernelSize_valueChanged(int arg1);

    void on_CheckBoxShowOutput_toggled(bool checked);

    void on_spinBoxTransparency_valueChanged(int arg1);

    void on_pushButtonSaveOut_clicked();

    void on_CheckBoxContour_toggled(bool checked);

    void on_spinBoxClosingShape_valueChanged(int arg1);

    void on_spinBoxErosionShape_valueChanged(int arg1);

    void on_spinBoxErosionShape1_valueChanged(int arg1);

    void on_spinBoxDilationShape2_valueChanged(int arg1);

    void on_spinBoxErosionShape3_valueChanged(int arg1);

    void on_CheckBoxFillHolesOnOut_toggled(bool checked);

    void on_CheckBoxExpandMaskY_toggled(bool checked);

    void on_CheckBoxShowMaskImplant_toggled(bool checked);

    void on_CheckBoxShowSDA_toggled(bool checked);

    void on_CheckBoxShowSDAThresholded_toggled(bool checked);


    void on_CheckBoxDivideseparateregionsSDA_toggled(bool checked);

    void on_spinBoxMinRegionSizeSDA_valueChanged(int arg1);

    void on_comboBoxSDASize_currentIndexChanged(int index);

    void on_CheckBoxShowSDANormPC_toggled(bool checked);

    void on_CheckBoxShowSDANorm_toggled(bool checked);

    void on_pushButtonStoreLocalToOut_clicked();

    void on_pushButtonSaveRef_clicked();

    void on_pushButtonClearOut_clicked();

    void on_pushButtonFindOptimalTheshold_clicked();

    void on_pushButtonDataFor2dPlot_clicked();

    void on_lineEditFilePattern_returnPressed();

    void on_checkBoxFindRegTouchingImplant_toggled(bool checked);

    void on_DistortImIn_clicked();

    void on_pushButtonOpenRefROI_clicked();

    void on_checkBoxShowRefference_toggled(bool checked);

    void on_pushButtonTemp_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
