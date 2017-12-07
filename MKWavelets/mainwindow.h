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

    cv::Mat ImIn;
    cv::Mat ImSmall;
    cv::Mat ImNormalised;
    cv::Mat ImWaveletLL[8];
    cv::Mat ImWaveletLH[8];
    cv::Mat ImWaveletHL[8];
    cv::Mat ImWaveletHH[8];
    cv::Mat ImScaledDown[8];

    int roiOffsetX;
    int roiOffsetY;
    int roiSizeX;
    int roiSizeY;
    int qLevels;
    int imShowScale;
    int maxWavScale;

    int MaxScale;

    int minNorm;
    int maxNorm;

    boost::filesystem::path InputDirectory;

    void ProcessImage();

private slots:
    void on_pushButtonFileOpen_clicked();

    void on_listWidgetFiles_currentTextChanged(const QString &currentText);

    void on_spinBoxOffsetX_valueChanged(int arg1);

    void on_spinBoxOffsetY_valueChanged(int arg1);

    void on_spinBoxSizeX_valueChanged(int arg1);

    void on_spinBoxSizeY_valueChanged(int arg1);

    void on_spinBoxQLevels_valueChanged(int arg1);

    void on_spinBoxImShowScale_valueChanged(int arg1);

    void on_spinBoxMaxWavScale_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
