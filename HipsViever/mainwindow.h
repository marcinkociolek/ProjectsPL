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
    cv::Mat ImConv2;
    cv::Mat ImGradient;
    cv::Mat Mask;

    int minShowGray;
    int maxShowGray;

    int minShowPseudocolor;
    int maxShowPseudocolor;
    int minShowGradient;
    int maxShowGradient;

    int minShowConv;
    int maxShowConv;

    unsigned short thresholdImOrg;

    bool showInputGray;
    bool showInputPseudocolor;

    bool showGradient;
    bool showConv;
    bool showMask;

    void MainWindow::CalculateTransform(void);
    void MainWindow::MaskImage(void);
    void MainWindow::ProcessImage(void);
    void MainWindow::ShowImages(void);

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
