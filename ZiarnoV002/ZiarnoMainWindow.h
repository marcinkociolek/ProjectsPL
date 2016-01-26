#ifndef ZIARNOMAINWINDOW_H
#define ZIARNOMAINWINDOW_H

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
    Mat ImIn;
    Mat ImIn1;
    Mat ImIn2;

    Mat ImMask1;
    Mat ImMask2;

    Mat Contour1;
    Mat Contour2;

    int maxX;
    int maxY;

    path InputDirectory;

    // analisis options
    int segmentType;
    float threshVal;
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
    bool fitEllipseToReg;

    // display options
    int displayFlag;

    bool showInput;
    bool showThesholded;
    bool show1stMorphology;
    bool showHolesRemoved;
    bool showMask;
    bool showContourOnInput;



    void ProcessImage(void);
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

private:
    Ui::MainWindow *ui;
};

//--------------------------------------------------------------------------------------------------
//          My functions
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
#endif // ZIARNOMAINWINDOW_H
