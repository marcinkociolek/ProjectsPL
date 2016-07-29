#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <boost/filesystem.hpp>

#include <opencv2/core/core.hpp>

#include <QMainWindow>

using namespace boost::filesystem;
using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    path FileToOpen;
    path CurrentDir;

    Mat ImShow;
    Mat ImIn;

    Mat ImMask;

//  Mat Contour2;

    int maxX;
    int maxY;

    path InputDirectory;
    // display options
    int displayFlag;


    void ProcessImage(void);

private slots:
    void on_pushButtonOpenFile_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
