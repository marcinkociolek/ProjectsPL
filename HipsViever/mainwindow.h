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
    cv::Mat ImGradient;
    cv::Mat Mask;
    cv::Mat MaskSDA;
    cv::Mat ImOut;

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
    bool showConv;
    bool showMask;

    bool showContour;

    bool fillHoles;
    bool divideSeparateRegions;

    int minRegionSize;

    bool expandMask;
    int expansionSize;

    bool croppMask;
    int croppSize;

    bool calculateSDA;
    int kernelSizeSDA;

    bool thresholdSDA;
    int thresholdImSDA;

    bool showOutput;
    int transparency;

    bool displayFlag;

    void MainWindow::CalculateSDA(void);
    void MainWindow::MaskImage(void);
    void MainWindow::ProcessImage(void);
    void MainWindow::ShowImages(void);
    void MainWindow::OnOffImageWindow(void);

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
