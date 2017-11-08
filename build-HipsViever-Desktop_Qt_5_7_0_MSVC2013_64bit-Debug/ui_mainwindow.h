/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

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
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLineEdit *LineEditDirectory;
    QPushButton *pushButtonSelectInFolder;
    QListWidget *ListWidgetFiles;
    QCheckBox *CheckBoxShowInputImageGray;
    QComboBox *comboBoxShowModeGray;
    QSpinBox *spinBoxMaxShowGray;
    QSpinBox *spinBoxMinShowGray;
    QLabel *labelMaxDispGray;
    QLabel *labelMinDispGray;
    QCheckBox *CheckBoxShowInputImagePC;
    QLabel *labelMaxDispPseudoColor;
    QLabel *labelMinDispPseudoColor;
    QSpinBox *spinBoxMaxShowPseudoColor;
    QSpinBox *spinBoxMinShowPseudoColor;
    QLabel *labelMaxDispGradient;
    QSpinBox *spinBoxMinShowGradient;
    QLabel *labelMinDispGradient;
    QSpinBox *spinBoxMaxShowGradient;
    QCheckBox *CheckBoxShowGradient;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(945, 512);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        LineEditDirectory = new QLineEdit(centralWidget);
        LineEditDirectory->setObjectName(QStringLiteral("LineEditDirectory"));
        LineEditDirectory->setEnabled(false);
        LineEditDirectory->setGeometry(QRect(110, 10, 271, 20));
        pushButtonSelectInFolder = new QPushButton(centralWidget);
        pushButtonSelectInFolder->setObjectName(QStringLiteral("pushButtonSelectInFolder"));
        pushButtonSelectInFolder->setGeometry(QRect(10, 10, 91, 23));
        ListWidgetFiles = new QListWidget(centralWidget);
        ListWidgetFiles->setObjectName(QStringLiteral("ListWidgetFiles"));
        ListWidgetFiles->setGeometry(QRect(10, 40, 371, 251));
        CheckBoxShowInputImageGray = new QCheckBox(centralWidget);
        CheckBoxShowInputImageGray->setObjectName(QStringLiteral("CheckBoxShowInputImageGray"));
        CheckBoxShowInputImageGray->setGeometry(QRect(400, 40, 151, 17));
        CheckBoxShowInputImageGray->setChecked(true);
        comboBoxShowModeGray = new QComboBox(centralWidget);
        comboBoxShowModeGray->setObjectName(QStringLiteral("comboBoxShowModeGray"));
        comboBoxShowModeGray->setGeometry(QRect(400, 100, 140, 30));
        QFont font;
        font.setPointSize(11);
        comboBoxShowModeGray->setFont(font);
        spinBoxMaxShowGray = new QSpinBox(centralWidget);
        spinBoxMaxShowGray->setObjectName(QStringLiteral("spinBoxMaxShowGray"));
        spinBoxMaxShowGray->setGeometry(QRect(470, 170, 71, 24));
        spinBoxMaxShowGray->setFont(font);
        spinBoxMaxShowGray->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBoxMaxShowGray->setMaximum(65535);
        spinBoxMaxShowGray->setSingleStep(32);
        spinBoxMaxShowGray->setValue(4095);
        spinBoxMinShowGray = new QSpinBox(centralWidget);
        spinBoxMinShowGray->setObjectName(QStringLiteral("spinBoxMinShowGray"));
        spinBoxMinShowGray->setGeometry(QRect(470, 140, 71, 24));
        spinBoxMinShowGray->setFont(font);
        spinBoxMinShowGray->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBoxMinShowGray->setMinimum(-65536);
        spinBoxMinShowGray->setMaximum(65535);
        spinBoxMinShowGray->setSingleStep(2);
        labelMaxDispGray = new QLabel(centralWidget);
        labelMaxDispGray->setObjectName(QStringLiteral("labelMaxDispGray"));
        labelMaxDispGray->setGeometry(QRect(400, 170, 70, 30));
        labelMaxDispGray->setFont(font);
        labelMinDispGray = new QLabel(centralWidget);
        labelMinDispGray->setObjectName(QStringLiteral("labelMinDispGray"));
        labelMinDispGray->setGeometry(QRect(400, 140, 60, 24));
        labelMinDispGray->setFont(font);
        labelMinDispGray->setMouseTracking(true);
        CheckBoxShowInputImagePC = new QCheckBox(centralWidget);
        CheckBoxShowInputImagePC->setObjectName(QStringLiteral("CheckBoxShowInputImagePC"));
        CheckBoxShowInputImagePC->setGeometry(QRect(400, 60, 151, 17));
        CheckBoxShowInputImagePC->setChecked(true);
        labelMaxDispPseudoColor = new QLabel(centralWidget);
        labelMaxDispPseudoColor->setObjectName(QStringLiteral("labelMaxDispPseudoColor"));
        labelMaxDispPseudoColor->setGeometry(QRect(400, 260, 60, 24));
        labelMaxDispPseudoColor->setFont(font);
        labelMinDispPseudoColor = new QLabel(centralWidget);
        labelMinDispPseudoColor->setObjectName(QStringLiteral("labelMinDispPseudoColor"));
        labelMinDispPseudoColor->setGeometry(QRect(400, 230, 60, 24));
        labelMinDispPseudoColor->setFont(font);
        labelMinDispPseudoColor->setMouseTracking(true);
        spinBoxMaxShowPseudoColor = new QSpinBox(centralWidget);
        spinBoxMaxShowPseudoColor->setObjectName(QStringLiteral("spinBoxMaxShowPseudoColor"));
        spinBoxMaxShowPseudoColor->setGeometry(QRect(470, 260, 71, 24));
        spinBoxMaxShowPseudoColor->setFont(font);
        spinBoxMaxShowPseudoColor->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBoxMaxShowPseudoColor->setMaximum(65535);
        spinBoxMaxShowPseudoColor->setSingleStep(32);
        spinBoxMaxShowPseudoColor->setValue(4095);
        spinBoxMinShowPseudoColor = new QSpinBox(centralWidget);
        spinBoxMinShowPseudoColor->setObjectName(QStringLiteral("spinBoxMinShowPseudoColor"));
        spinBoxMinShowPseudoColor->setGeometry(QRect(470, 230, 71, 24));
        spinBoxMinShowPseudoColor->setFont(font);
        spinBoxMinShowPseudoColor->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBoxMinShowPseudoColor->setMinimum(-65536);
        spinBoxMinShowPseudoColor->setMaximum(65535);
        spinBoxMinShowPseudoColor->setSingleStep(2);
        labelMaxDispGradient = new QLabel(centralWidget);
        labelMaxDispGradient->setObjectName(QStringLiteral("labelMaxDispGradient"));
        labelMaxDispGradient->setGeometry(QRect(400, 320, 60, 24));
        labelMaxDispGradient->setFont(font);
        spinBoxMinShowGradient = new QSpinBox(centralWidget);
        spinBoxMinShowGradient->setObjectName(QStringLiteral("spinBoxMinShowGradient"));
        spinBoxMinShowGradient->setGeometry(QRect(469, 290, 71, 24));
        spinBoxMinShowGradient->setFont(font);
        spinBoxMinShowGradient->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBoxMinShowGradient->setMinimum(-65536);
        spinBoxMinShowGradient->setMaximum(65535);
        spinBoxMinShowGradient->setSingleStep(2);
        labelMinDispGradient = new QLabel(centralWidget);
        labelMinDispGradient->setObjectName(QStringLiteral("labelMinDispGradient"));
        labelMinDispGradient->setGeometry(QRect(400, 290, 60, 24));
        labelMinDispGradient->setFont(font);
        labelMinDispGradient->setMouseTracking(true);
        spinBoxMaxShowGradient = new QSpinBox(centralWidget);
        spinBoxMaxShowGradient->setObjectName(QStringLiteral("spinBoxMaxShowGradient"));
        spinBoxMaxShowGradient->setGeometry(QRect(470, 320, 71, 24));
        spinBoxMaxShowGradient->setFont(font);
        spinBoxMaxShowGradient->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBoxMaxShowGradient->setMaximum(65535);
        spinBoxMaxShowGradient->setSingleStep(2);
        spinBoxMaxShowGradient->setValue(128);
        CheckBoxShowGradient = new QCheckBox(centralWidget);
        CheckBoxShowGradient->setObjectName(QStringLiteral("CheckBoxShowGradient"));
        CheckBoxShowGradient->setGeometry(QRect(400, 80, 151, 17));
        CheckBoxShowGradient->setChecked(true);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 945, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        pushButtonSelectInFolder->setText(QApplication::translate("MainWindow", "Select folder In", 0));
        CheckBoxShowInputImageGray->setText(QApplication::translate("MainWindow", "Show input image Gray", 0));
        labelMaxDispGray->setText(QApplication::translate("MainWindow", "Max disp", 0));
        labelMinDispGray->setText(QApplication::translate("MainWindow", "<html><head/><body><p>Min disp</p></body></html>", 0));
        CheckBoxShowInputImagePC->setText(QApplication::translate("MainWindow", "Show input image PC", 0));
        labelMaxDispPseudoColor->setText(QApplication::translate("MainWindow", "Max disp", 0));
        labelMinDispPseudoColor->setText(QApplication::translate("MainWindow", "<html><head/><body><p>Min disp</p></body></html>", 0));
        labelMaxDispGradient->setText(QApplication::translate("MainWindow", "Max disp", 0));
        labelMinDispGradient->setText(QApplication::translate("MainWindow", "<html><head/><body><p>Min disp</p></body></html>", 0));
        CheckBoxShowGradient->setText(QApplication::translate("MainWindow", "Show Gradient", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
