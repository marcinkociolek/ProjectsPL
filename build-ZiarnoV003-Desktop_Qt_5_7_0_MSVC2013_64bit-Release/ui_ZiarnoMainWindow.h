/********************************************************************************
** Form generated from reading UI file 'ZiarnoMainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ZIARNOMAINWINDOW_H
#define UI_ZIARNOMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QSpinBox *spinBox;
    QListWidget *FileListWidget;
    QLineEdit *DirectoryLineEdit;
    QComboBox *SegmentationComboBox;
    QCheckBox *RemoveSmalRegionscheckBox;
    QSpinBox *MinRegionSizeSpinBox;
    QComboBox *RawRegionMorfology1ComboBox;
    QComboBox *RawRegionMorfology2ComboBox;
    QComboBox *RawRegionMorfology3ComboBox;
    QComboBox *RegionMorfology2ComboBox;
    QComboBox *RegionMorfology1ComboBox;
    QComboBox *RegionMorfology3ComboBox;
    QCheckBox *DivideRegionsCheckBox;
    QCheckBox *ShowInputImageCheckBox;
    QCheckBox *ShowThresholdedImgeCheckBox;
    QCheckBox *ShowWithoutHolesCheckBox;
    QCheckBox *ShowAfter1MorphologyCheckBox;
    QCheckBox *FillHolesCheckBox;
    QLineEdit *DurationLineEdit;
    QLineEdit *RegexLineEdit;
    QListWidget *RegexListWidget;
    QPushButton *pushButtonRegex;
    QCheckBox *ShowMaskCheckBox;
    QCheckBox *ShowContourOnImageCheckBox;
    QCheckBox *AllowResizeCheckBox;
    QCheckBox *RemoveBorderRegionCheckBox;
    QCheckBox *FitEllipseCheckBox;
    QCheckBox *RotateCheckBox;
    QCheckBox *ShowFitedElipseCheckBox;
    QCheckBox *ShowRotatedCheckBox;
    QCheckBox *CroppCheckBox;
    QLabel *label;
    QLabel *label_2;
    QComboBox *ShowComboBox;
    QTextEdit *MesageTextEdit;
    QCheckBox *ShowCroppedGradientCheckBox;
    QCheckBox *AlignCheckBox;
    QCheckBox *ShowAreaForAlignCheckBox;
    QCheckBox *ShowAlignedCheckBox;
    QCheckBox *ShowGradientCheckBox;
    QCheckBox *FindValeyCheckBox;
    QCheckBox *ShowOutputCheckBox;
    QLineEdit *OutDirectoryLineEdit;
    QPushButton *SelOutFolderPushButton;
    QPushButton *pushButtonConvertAll;
    QCheckBox *VerticalDivisionCheckBox;
    QCheckBox *ShowCroppedCheckBox_2;
    QCheckBox *SaveResultCheckBox;
    QCheckBox *ShowOutput2CheckBox;
    QCheckBox *SubsegmentCheckBox;
    QComboBox *InputTypecomboBox;
    QLabel *label_3;
    QCheckBox *ShowThirdCheckBox;
    QSpinBox *spinBoxThreshVal3;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(776, 589);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 0, 91, 23));
        spinBox = new QSpinBox(centralWidget);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(480, 0, 42, 22));
        spinBox->setMaximum(255);
        spinBox->setValue(16);
        spinBox->setDisplayIntegerBase(10);
        FileListWidget = new QListWidget(centralWidget);
        FileListWidget->setObjectName(QStringLiteral("FileListWidget"));
        FileListWidget->setGeometry(QRect(10, 60, 371, 251));
        DirectoryLineEdit = new QLineEdit(centralWidget);
        DirectoryLineEdit->setObjectName(QStringLiteral("DirectoryLineEdit"));
        DirectoryLineEdit->setEnabled(false);
        DirectoryLineEdit->setGeometry(QRect(110, 0, 271, 20));
        SegmentationComboBox = new QComboBox(centralWidget);
        SegmentationComboBox->setObjectName(QStringLiteral("SegmentationComboBox"));
        SegmentationComboBox->setGeometry(QRect(400, 50, 151, 22));
        RemoveSmalRegionscheckBox = new QCheckBox(centralWidget);
        RemoveSmalRegionscheckBox->setObjectName(QStringLiteral("RemoveSmalRegionscheckBox"));
        RemoveSmalRegionscheckBox->setGeometry(QRect(400, 80, 151, 17));
        RemoveSmalRegionscheckBox->setChecked(true);
        MinRegionSizeSpinBox = new QSpinBox(centralWidget);
        MinRegionSizeSpinBox->setObjectName(QStringLiteral("MinRegionSizeSpinBox"));
        MinRegionSizeSpinBox->setGeometry(QRect(400, 240, 71, 22));
        MinRegionSizeSpinBox->setMaximum(1000000);
        MinRegionSizeSpinBox->setSingleStep(50);
        MinRegionSizeSpinBox->setValue(10000);
        RawRegionMorfology1ComboBox = new QComboBox(centralWidget);
        RawRegionMorfology1ComboBox->setObjectName(QStringLiteral("RawRegionMorfology1ComboBox"));
        RawRegionMorfology1ComboBox->setGeometry(QRect(400, 100, 151, 22));
        RawRegionMorfology2ComboBox = new QComboBox(centralWidget);
        RawRegionMorfology2ComboBox->setObjectName(QStringLiteral("RawRegionMorfology2ComboBox"));
        RawRegionMorfology2ComboBox->setGeometry(QRect(400, 130, 151, 22));
        RawRegionMorfology3ComboBox = new QComboBox(centralWidget);
        RawRegionMorfology3ComboBox->setObjectName(QStringLiteral("RawRegionMorfology3ComboBox"));
        RawRegionMorfology3ComboBox->setGeometry(QRect(400, 160, 151, 22));
        RegionMorfology2ComboBox = new QComboBox(centralWidget);
        RegionMorfology2ComboBox->setObjectName(QStringLiteral("RegionMorfology2ComboBox"));
        RegionMorfology2ComboBox->setGeometry(QRect(400, 300, 151, 22));
        RegionMorfology1ComboBox = new QComboBox(centralWidget);
        RegionMorfology1ComboBox->setObjectName(QStringLiteral("RegionMorfology1ComboBox"));
        RegionMorfology1ComboBox->setGeometry(QRect(400, 270, 151, 22));
        RegionMorfology3ComboBox = new QComboBox(centralWidget);
        RegionMorfology3ComboBox->setObjectName(QStringLiteral("RegionMorfology3ComboBox"));
        RegionMorfology3ComboBox->setGeometry(QRect(400, 330, 151, 22));
        DivideRegionsCheckBox = new QCheckBox(centralWidget);
        DivideRegionsCheckBox->setObjectName(QStringLiteral("DivideRegionsCheckBox"));
        DivideRegionsCheckBox->setGeometry(QRect(400, 210, 151, 17));
        DivideRegionsCheckBox->setChecked(true);
        ShowInputImageCheckBox = new QCheckBox(centralWidget);
        ShowInputImageCheckBox->setObjectName(QStringLiteral("ShowInputImageCheckBox"));
        ShowInputImageCheckBox->setGeometry(QRect(560, 100, 151, 17));
        ShowThresholdedImgeCheckBox = new QCheckBox(centralWidget);
        ShowThresholdedImgeCheckBox->setObjectName(QStringLiteral("ShowThresholdedImgeCheckBox"));
        ShowThresholdedImgeCheckBox->setGeometry(QRect(560, 120, 151, 17));
        ShowThresholdedImgeCheckBox->setChecked(true);
        ShowWithoutHolesCheckBox = new QCheckBox(centralWidget);
        ShowWithoutHolesCheckBox->setObjectName(QStringLiteral("ShowWithoutHolesCheckBox"));
        ShowWithoutHolesCheckBox->setGeometry(QRect(560, 160, 151, 17));
        ShowAfter1MorphologyCheckBox = new QCheckBox(centralWidget);
        ShowAfter1MorphologyCheckBox->setObjectName(QStringLiteral("ShowAfter1MorphologyCheckBox"));
        ShowAfter1MorphologyCheckBox->setGeometry(QRect(560, 140, 151, 17));
        FillHolesCheckBox = new QCheckBox(centralWidget);
        FillHolesCheckBox->setObjectName(QStringLiteral("FillHolesCheckBox"));
        FillHolesCheckBox->setGeometry(QRect(400, 190, 151, 17));
        FillHolesCheckBox->setChecked(true);
        DurationLineEdit = new QLineEdit(centralWidget);
        DurationLineEdit->setObjectName(QStringLiteral("DurationLineEdit"));
        DurationLineEdit->setEnabled(false);
        DurationLineEdit->setGeometry(QRect(10, 360, 111, 20));
        RegexLineEdit = new QLineEdit(centralWidget);
        RegexLineEdit->setObjectName(QStringLiteral("RegexLineEdit"));
        RegexLineEdit->setGeometry(QRect(10, 330, 113, 20));
        RegexListWidget = new QListWidget(centralWidget);
        RegexListWidget->setObjectName(QStringLiteral("RegexListWidget"));
        RegexListWidget->setGeometry(QRect(560, 360, 211, 161));
        pushButtonRegex = new QPushButton(centralWidget);
        pushButtonRegex->setObjectName(QStringLiteral("pushButtonRegex"));
        pushButtonRegex->setGeometry(QRect(130, 330, 75, 23));
        ShowMaskCheckBox = new QCheckBox(centralWidget);
        ShowMaskCheckBox->setObjectName(QStringLiteral("ShowMaskCheckBox"));
        ShowMaskCheckBox->setGeometry(QRect(560, 180, 151, 17));
        ShowContourOnImageCheckBox = new QCheckBox(centralWidget);
        ShowContourOnImageCheckBox->setObjectName(QStringLiteral("ShowContourOnImageCheckBox"));
        ShowContourOnImageCheckBox->setGeometry(QRect(240, 340, 151, 17));
        AllowResizeCheckBox = new QCheckBox(centralWidget);
        AllowResizeCheckBox->setObjectName(QStringLiteral("AllowResizeCheckBox"));
        AllowResizeCheckBox->setGeometry(QRect(560, 0, 151, 17));
        RemoveBorderRegionCheckBox = new QCheckBox(centralWidget);
        RemoveBorderRegionCheckBox->setObjectName(QStringLiteral("RemoveBorderRegionCheckBox"));
        RemoveBorderRegionCheckBox->setGeometry(QRect(400, 360, 151, 17));
        FitEllipseCheckBox = new QCheckBox(centralWidget);
        FitEllipseCheckBox->setObjectName(QStringLiteral("FitEllipseCheckBox"));
        FitEllipseCheckBox->setGeometry(QRect(400, 380, 151, 17));
        FitEllipseCheckBox->setChecked(true);
        RotateCheckBox = new QCheckBox(centralWidget);
        RotateCheckBox->setObjectName(QStringLiteral("RotateCheckBox"));
        RotateCheckBox->setGeometry(QRect(400, 450, 151, 17));
        RotateCheckBox->setChecked(true);
        ShowFitedElipseCheckBox = new QCheckBox(centralWidget);
        ShowFitedElipseCheckBox->setObjectName(QStringLiteral("ShowFitedElipseCheckBox"));
        ShowFitedElipseCheckBox->setGeometry(QRect(560, 200, 151, 17));
        ShowRotatedCheckBox = new QCheckBox(centralWidget);
        ShowRotatedCheckBox->setObjectName(QStringLiteral("ShowRotatedCheckBox"));
        ShowRotatedCheckBox->setGeometry(QRect(560, 220, 151, 17));
        CroppCheckBox = new QCheckBox(centralWidget);
        CroppCheckBox->setObjectName(QStringLiteral("CroppCheckBox"));
        CroppCheckBox->setGeometry(QRect(400, 470, 151, 17));
        CroppCheckBox->setChecked(true);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(130, 360, 71, 16));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(480, 240, 61, 16));
        ShowComboBox = new QComboBox(centralWidget);
        ShowComboBox->setObjectName(QStringLiteral("ShowComboBox"));
        ShowComboBox->setGeometry(QRect(560, 50, 151, 22));
        MesageTextEdit = new QTextEdit(centralWidget);
        MesageTextEdit->setObjectName(QStringLiteral("MesageTextEdit"));
        MesageTextEdit->setGeometry(QRect(10, 390, 371, 111));
        ShowCroppedGradientCheckBox = new QCheckBox(centralWidget);
        ShowCroppedGradientCheckBox->setObjectName(QStringLiteral("ShowCroppedGradientCheckBox"));
        ShowCroppedGradientCheckBox->setGeometry(QRect(240, 360, 151, 17));
        AlignCheckBox = new QCheckBox(centralWidget);
        AlignCheckBox->setObjectName(QStringLiteral("AlignCheckBox"));
        AlignCheckBox->setGeometry(QRect(400, 400, 151, 17));
        AlignCheckBox->setChecked(true);
        ShowAreaForAlignCheckBox = new QCheckBox(centralWidget);
        ShowAreaForAlignCheckBox->setObjectName(QStringLiteral("ShowAreaForAlignCheckBox"));
        ShowAreaForAlignCheckBox->setGeometry(QRect(560, 260, 151, 17));
        ShowAlignedCheckBox = new QCheckBox(centralWidget);
        ShowAlignedCheckBox->setObjectName(QStringLiteral("ShowAlignedCheckBox"));
        ShowAlignedCheckBox->setGeometry(QRect(560, 280, 151, 17));
        ShowGradientCheckBox = new QCheckBox(centralWidget);
        ShowGradientCheckBox->setObjectName(QStringLiteral("ShowGradientCheckBox"));
        ShowGradientCheckBox->setGeometry(QRect(560, 300, 151, 17));
        FindValeyCheckBox = new QCheckBox(centralWidget);
        FindValeyCheckBox->setObjectName(QStringLiteral("FindValeyCheckBox"));
        FindValeyCheckBox->setGeometry(QRect(400, 420, 151, 17));
        FindValeyCheckBox->setChecked(true);
        ShowOutputCheckBox = new QCheckBox(centralWidget);
        ShowOutputCheckBox->setObjectName(QStringLiteral("ShowOutputCheckBox"));
        ShowOutputCheckBox->setGeometry(QRect(560, 320, 151, 17));
        ShowOutputCheckBox->setChecked(false);
        OutDirectoryLineEdit = new QLineEdit(centralWidget);
        OutDirectoryLineEdit->setObjectName(QStringLiteral("OutDirectoryLineEdit"));
        OutDirectoryLineEdit->setEnabled(false);
        OutDirectoryLineEdit->setGeometry(QRect(110, 31, 271, 20));
        SelOutFolderPushButton = new QPushButton(centralWidget);
        SelOutFolderPushButton->setObjectName(QStringLiteral("SelOutFolderPushButton"));
        SelOutFolderPushButton->setGeometry(QRect(10, 30, 91, 23));
        pushButtonConvertAll = new QPushButton(centralWidget);
        pushButtonConvertAll->setObjectName(QStringLiteral("pushButtonConvertAll"));
        pushButtonConvertAll->setGeometry(QRect(400, 0, 80, 22));
        VerticalDivisionCheckBox = new QCheckBox(centralWidget);
        VerticalDivisionCheckBox->setObjectName(QStringLiteral("VerticalDivisionCheckBox"));
        VerticalDivisionCheckBox->setGeometry(QRect(400, 30, 151, 17));
        VerticalDivisionCheckBox->setChecked(false);
        ShowCroppedCheckBox_2 = new QCheckBox(centralWidget);
        ShowCroppedCheckBox_2->setObjectName(QStringLiteral("ShowCroppedCheckBox_2"));
        ShowCroppedCheckBox_2->setGeometry(QRect(560, 240, 151, 17));
        SaveResultCheckBox = new QCheckBox(centralWidget);
        SaveResultCheckBox->setObjectName(QStringLiteral("SaveResultCheckBox"));
        SaveResultCheckBox->setEnabled(true);
        SaveResultCheckBox->setGeometry(QRect(400, 510, 151, 17));
        SaveResultCheckBox->setChecked(false);
        ShowOutput2CheckBox = new QCheckBox(centralWidget);
        ShowOutput2CheckBox->setObjectName(QStringLiteral("ShowOutput2CheckBox"));
        ShowOutput2CheckBox->setGeometry(QRect(560, 340, 151, 17));
        ShowOutput2CheckBox->setChecked(false);
        SubsegmentCheckBox = new QCheckBox(centralWidget);
        SubsegmentCheckBox->setObjectName(QStringLiteral("SubsegmentCheckBox"));
        SubsegmentCheckBox->setGeometry(QRect(400, 490, 151, 17));
        SubsegmentCheckBox->setChecked(true);
        InputTypecomboBox = new QComboBox(centralWidget);
        InputTypecomboBox->setObjectName(QStringLiteral("InputTypecomboBox"));
        InputTypecomboBox->setGeometry(QRect(560, 20, 91, 22));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(660, 20, 61, 16));
        ShowThirdCheckBox = new QCheckBox(centralWidget);
        ShowThirdCheckBox->setObjectName(QStringLiteral("ShowThirdCheckBox"));
        ShowThirdCheckBox->setGeometry(QRect(560, 80, 151, 17));
        ShowThirdCheckBox->setChecked(true);
        spinBoxThreshVal3 = new QSpinBox(centralWidget);
        spinBoxThreshVal3->setObjectName(QStringLiteral("spinBoxThreshVal3"));
        spinBoxThreshVal3->setGeometry(QRect(520, 0, 42, 22));
        spinBoxThreshVal3->setMaximum(255);
        spinBoxThreshVal3->setValue(100);
        spinBoxThreshVal3->setDisplayIntegerBase(10);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 776, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        ShowComboBox->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Select folder In", 0));
        RemoveSmalRegionscheckBox->setText(QApplication::translate("MainWindow", "Remove small regions", 0));
        DivideRegionsCheckBox->setText(QApplication::translate("MainWindow", "Divide regions", 0));
        ShowInputImageCheckBox->setText(QApplication::translate("MainWindow", "Show input image", 0));
        ShowThresholdedImgeCheckBox->setText(QApplication::translate("MainWindow", "Show Thresholded image", 0));
        ShowWithoutHolesCheckBox->setText(QApplication::translate("MainWindow", "Show after hole removal", 0));
        ShowAfter1MorphologyCheckBox->setText(QApplication::translate("MainWindow", "Show after first morphology", 0));
        FillHolesCheckBox->setText(QApplication::translate("MainWindow", "FillHoles", 0));
        pushButtonRegex->setText(QApplication::translate("MainWindow", "Subselection", 0));
        ShowMaskCheckBox->setText(QApplication::translate("MainWindow", "Show mask", 0));
        ShowContourOnImageCheckBox->setText(QApplication::translate("MainWindow", "Show Contour on image", 0));
        AllowResizeCheckBox->setText(QApplication::translate("MainWindow", "Allow Image Resize", 0));
        RemoveBorderRegionCheckBox->setText(QApplication::translate("MainWindow", "Remove regions touching border", 0));
        FitEllipseCheckBox->setText(QApplication::translate("MainWindow", "Fit Ellipse", 0));
        RotateCheckBox->setText(QApplication::translate("MainWindow", "Rotate", 0));
        ShowFitedElipseCheckBox->setText(QApplication::translate("MainWindow", "Show Fited Rectangle", 0));
        ShowRotatedCheckBox->setText(QApplication::translate("MainWindow", "Show Rotated", 0));
        CroppCheckBox->setText(QApplication::translate("MainWindow", "Cropp", 0));
        label->setText(QApplication::translate("MainWindow", "Comp. time [s]", 0));
        label_2->setText(QApplication::translate("MainWindow", "Min reg size", 0));
        ShowCroppedGradientCheckBox->setText(QApplication::translate("MainWindow", "Show Cropped Gradient", 0));
        AlignCheckBox->setText(QApplication::translate("MainWindow", "Align", 0));
        ShowAreaForAlignCheckBox->setText(QApplication::translate("MainWindow", "Show align area", 0));
        ShowAlignedCheckBox->setText(QApplication::translate("MainWindow", "Show aligned", 0));
        ShowGradientCheckBox->setText(QApplication::translate("MainWindow", "Show gradient", 0));
        FindValeyCheckBox->setText(QApplication::translate("MainWindow", "FindValey", 0));
        ShowOutputCheckBox->setText(QApplication::translate("MainWindow", "Show output", 0));
        SelOutFolderPushButton->setText(QApplication::translate("MainWindow", "Select folder Out", 0));
        pushButtonConvertAll->setText(QApplication::translate("MainWindow", "ConvertAll", 0));
        VerticalDivisionCheckBox->setText(QApplication::translate("MainWindow", "Vertical Input Images", 0));
        ShowCroppedCheckBox_2->setText(QApplication::translate("MainWindow", "Show cropped", 0));
        SaveResultCheckBox->setText(QApplication::translate("MainWindow", "Save Result", 0));
        ShowOutput2CheckBox->setText(QApplication::translate("MainWindow", "Show output 2", 0));
        SubsegmentCheckBox->setText(QApplication::translate("MainWindow", "Subsegment", 0));
        label_3->setText(QApplication::translate("MainWindow", "Input type", 0));
        ShowThirdCheckBox->setText(QApplication::translate("MainWindow", "Show Third", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ZIARNOMAINWINDOW_H
