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
    cv::Mat ImGradient;

    int minShowGray;
    int maxShowGray;

    int minShowPseudocolor;
    int maxShowPseudocolor;
    int minShowGradient;
    int maxShowGradient;

    bool showInputGray;
    bool showInputPseudocolor;

    bool showGradient;

    bool showMask;

    void MainWindow::ProcessImage(void);

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
