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
    int frameNr;
    std::string CurrentFrame00FileName;
    cv::Mat InIm;
    int maxX;
    int maxY;

    int minShow;
    int maxShow;

    int imageShowScale;

    bool showGray;
    bool showPseudoColor;
    bool showRegOnImagePC;
    bool showGradient;

    bool displayFlag;


    int minThresh;
    int maxThresh;

    cv::Mat Mask;



// my functions
    void MainWindow::ProcessFile();
    void MainWindow::ScaleImages();
    void MainWindow::OnOffImageWindow(void);
    cv::Mat MainWindow::SegmetU16BetweentwoThresholds(cv::Mat Im, unsigned short threshMin, unsigned short threshMax);

//SLOTS------------------------------------------------------------------------------------------------------
private slots:
    void on_OpenFolderPushButton_clicked();

    void on_InFileListWidget_currentTextChanged(const QString &currentText);

    void on_spinBoxFrameNr_valueChanged(int arg1);

    void on_spinBoxMinShow_valueChanged(int arg1);

    void on_spinBoxMaxShow_valueChanged(int arg1);

    void on_spinBoxImageScale_valueChanged(int arg1);

    void on_checkBoxShowGray_toggled(bool checked);

    void on_checkBoxShowPseudoColor_toggled(bool checked);

    void on_spinBoxThreshMin_valueChanged(int arg1);

    void on_spinBoxThreshMax_valueChanged(int arg1);

    void on_checkBoxRegOnPseudoColor_toggled(bool checked);

    void on_checkBoxShowGradient_toggled(bool checked);

    void on_pushButtonGetPixelValues_clicked();

private:
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
