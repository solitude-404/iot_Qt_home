/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>
#include <gaugewidget.h>
#include "QtCharts/QChartView.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QFrame *leftPanel;
    QLabel *lblLogo_2;
    QLabel *lblTimeDisplay;
    QLabel *label_2;
    QPushButton *btnComfortMode;
    QPushButton *btnSleepMode;
    QPushButton *btnCinemaMode;
    QPushButton *btnManualMode;
    QPushButton *btnNavLogs;
    QPushButton *btnNavTempHumi;
    QPushButton *btnNavControl;
    QFrame *rightPanel;
    QStackedWidget *stackedWidget;
    QWidget *homepage;
    QWidget *temp;
    QChartView *chartViewWidget;
    QLabel *lblStatusLed;
    QPushButton *btnConnect;
    QComboBox *cmbSerialPorts;
    QFrame *cardFan;
    QLabel *lblFanIcon;
    QCheckBox *chkFanSwitch;
    QFrame *cardHeat;
    QLabel *lblFireIcon;
    QCheckBox *chkHeatSwitch;
    QLabel *lblTempMin;
    QLabel *label;
    QLabel *label_3;
    QLabel *lblTempMax;
    QFrame *cardThreshold;
    QGridLayout *gridLayout;
    QPushButton *btnTempMaxDown;
    QPushButton *btnTempMinDown;
    QPushButton *btnTempMinUp;
    QPushButton *btnTempMaxUp;
    QPushButton *btnToRealtimeData;
    QWidget *history;
    QPushButton *btnBackToEnv;
    QTableWidget *tableHistory;
    QWidget *tempHistory;
    QPushButton *btnBackFromTempHistory;
    QTableWidget *tableTempHistory;
    QLabel *lbTempStart;
    QDateEdit *deTempStart;
    QLabel *lbTempEnd;
    QDateEdit *deTempEnd;
    QPushButton *btnTempFilter;
    QLabel *lbTempPage;
    QPushButton *btnTempPrev;
    QPushButton *btnTempNext;
    QWidget *camera;
    QWidget *operationLogs;
    QPushButton *btnBackFromLogs;
    QTableWidget *tableOperationLogs;
    QLineEdit *leOpLogDevice;
    QLabel *lbOpLogStart;
    QDateEdit *deOpLogStart;
    QLabel *lbOpLogEnd;
    QDateEdit *deOpLogEnd;
    QPushButton *btnOpLogFilter;
    QLabel *lbOpLogPage;
    QPushButton *btnOpLogPrev;
    QPushButton *btnOpLogNext;
    QFrame *lastPanel;
    QFrame *humiCard;
    QLabel *lblCurrentHumi;
    GaugeWidget *gaugeHumi;
    QFrame *tempCard;
    QLabel *lblCurrentTemp;
    QLabel *lblTempTargetRange;
    GaugeWidget *gaugeTemp;
    QCheckBox *switchAlarm;
    QPushButton *btnHistory;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1024, 600);
        MainWindow->setMinimumSize(QSize(1024, 600));
        MainWindow->setMaximumSize(QSize(1024, 600));
        MainWindow->setStyleSheet(QString::fromUtf8("QMainWindow {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #EEF5FF, stop:0.5 #F0F8FF, stop:1 #E8F4FF);\n"
"}\n"
"\n"
"/* ------------------------------\n"
"   \345\267\246\344\276\247\351\235\242\346\235\277 - \346\267\261\350\223\235\350\211\262\n"
"------------------------------ */\n"
"QFrame#leftPanel {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1E3A5F, stop:0.5 #1A3050, stop:1 #162845);\n"
"    border-radius: 0;\n"
"}\n"
"\n"
"/* ------------------------------\n"
"   \346\257\233\347\216\273\347\222\203\345\215\241\347\211\207\346\240\267\345\274\217\n"
"------------------------------ */\n"
"QFrame#cardSystem, \n"
"QFrame#cardTime, \n"
"QFrame#cardConnection,\n"
"QFrame#cardFan, \n"
"QFrame#cardHeat, \n"
"QFrame#cardThreshold {\n"
"    background-color: rgba(255, 255, 255, 0.75);\n"
"    border-radius: 16px;\n"
"    border: 1px solid rgba(200, 220, 255, 0.5);\n"
"}\n"
"\n"
"QFrame#tempCard, \n"
"QFrame#humiCard {\n"
"    background-color: rgba(255, 255, 255, 0"
                        ".75);\n"
"    border-radius: 16px;\n"
"    border: 1px solid rgba(200, 220, 255, 0.5);\n"
"}\n"
"\n"
"/* \345\215\241\347\211\207\346\202\254\346\265\256\346\225\210\346\236\234 */\n"
"QFrame#cardSystem:hover, \n"
"QFrame#cardTime:hover, \n"
"QFrame#cardConnection:hover,\n"
"QFrame#cardFan:hover, \n"
"QFrame#cardHeat:hover, \n"
"QFrame#cardThreshold:hover,\n"
"QFrame#tempCard:hover, \n"
"QFrame#humiCard:hover {\n"
"    background-color: rgba(255, 255, 255, 0.9);\n"
"    border: 1px solid rgba(100, 150, 220, 0.6);\n"
"}\n"
"\n"
"/* ------------------------------\n"
"   CheckBox \345\274\200\345\205\263\n"
"------------------------------ */\n"
"QCheckBox {\n"
"    spacing: 0;\n"
"}\n"
"QCheckBox::indicator {\n"
"    width: 52px;\n"
"    height: 26px;\n"
"}\n"
"QCheckBox::indicator:unchecked {\n"
"    image: url(:/images/off.svg);\n"
"}\n"
"QCheckBox::indicator:checked {\n"
"    image: url(:/images/on.svg);\n"
"}\n"
"\n"
"/* ------------------------------\n"
"   \346\214\211\351\222\256\347\276\216\345\214\226 - "
                        "\347\273\237\344\270\200\350\223\235\350\211\262\347\263\273\n"
"------------------------------ */\n"
"QPushButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #4A90D9, stop:1 #357ABD);\n"
"    color: white;\n"
"    border-radius: 8px;\n"
"    padding: 6px 16px;\n"
"    font-weight: 600;\n"
"    border: none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #5BA0E9, stop:1 #4A90D9);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #357ABD, stop:1 #2868A8);\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"    background-color: #C5D5E8;\n"
"    color: #8BA4C0;\n"
"}\n"
"\n"
"/* \345\257\274\350\210\252\346\214\211\351\222\256\346\240\267\345\274\217 */\n"
"QPushButton#btnNavHome,\n"
"QPushButton#btnNavControl,\n"
"QPushButton#btnNavLogs,\n"
"QPushButton#btnNavTempHumi {\n"
"    background-color: transparent;\n"
"    color: #A8C8E8;\n"
"    border-radius: 8px;\n"
"    padding: 10px 16px;\n"
""
                        "    font-weight: 500;\n"
"    border: none;\n"
"    text-align: left;\n"
"}\n"
"\n"
"QPushButton#btnNavHome:hover,\n"
"QPushButton#btnNavControl:hover,\n"
"QPushButton#btnNavLogs:hover,\n"
"QPushButton#btnNavTempHumi:hover {\n"
"    background-color: rgba(255, 255, 255, 0.15);\n"
"    color: #FFFFFF;\n"
"}\n"
"\n"
"QPushButton#btnNavHome:pressed,\n"
"QPushButton#btnNavControl:pressed,\n"
"QPushButton#btnNavLogs:pressed,\n"
"QPushButton#btnNavTempHumi:pressed {\n"
"    background-color: rgba(255, 255, 255, 0.25);\n"
"}\n"
"\n"
"/* ------------------------------\n"
"   \346\250\241\345\274\217\346\214\211\351\222\256\346\240\267\345\274\217 - \347\273\237\344\270\200\350\223\235\350\211\262\347\263\273\n"
"------------------------------ */\n"
"QPushButton#btnComfortMode {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #4A90D9, stop:1 #357ABD);\n"
"    color: white;\n"
"}\n"
"QPushButton#btnComfortMode:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #5BA0E9, stop:1 #"
                        "4A90D9);\n"
"}\n"
"\n"
"QPushButton#btnSleepMode {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #4A90D9, stop:1 #357ABD);\n"
"    color: white;\n"
"}\n"
"QPushButton#btnSleepMode:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #5BA0E9, stop:1 #4A90D9);\n"
"}\n"
"\n"
"QPushButton#btnCinemaMode {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #4A90D9, stop:1 #357ABD);\n"
"    color: white;\n"
"}\n"
"QPushButton#btnCinemaMode:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #5BA0E9, stop:1 #4A90D9);\n"
"}\n"
"\n"
"QPushButton#btnManualMode {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #4A90D9, stop:1 #357ABD);\n"
"    color: white;\n"
"}\n"
"QPushButton#btnManualMode:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #5BA0E9, stop:1 #4A90D9);\n"
"}\n"
"\n"
"/* ------------------------------\n"
"   \346\240\207\347\255\276\346\226\207\345\255\227\347\276\216\345\214\226 - \350"
                        "\223\235\350\211\262\347\263\273\n"
"------------------------------ */\n"
"QLabel {\n"
"    color: #1A3050;\n"
"}\n"
"\n"
"QLabel#labelTitle {\n"
"    font-size: 14px;\n"
"    color: #4A6080;\n"
"}\n"
"\n"
"QLabel#labelValue {\n"
"    font-size: 24px;\n"
"    font-weight: bold;\n"
"    color: #1E3A5F;\n"
"}\n"
"\n"
"QLabel#lblLogo_2 {\n"
"    color: #E8F4FF;\n"
"}\n"
"\n"
"QLabel#lblTimeDisplay {\n"
"    color: #A8C8E8;\n"
"}\n"
"\n"
"QLabel#lblStatusLed {\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QLabel#label_2 {\n"
"    color: #FFFFFF;\n"
"    font-weight: 500;\n"
"}\n"
"\n"
"/* ------------------------------\n"
"   \350\241\250\346\240\274\347\276\216\345\214\226 - \350\223\235\350\211\262\347\263\273\n"
"------------------------------ */\n"
"QTableWidget {\n"
"    background-color: rgba(255, 255, 255, 0.8);\n"
"    border-radius: 8px;\n"
"    border: 1px solid rgba(200, 220, 255, 0.5);\n"
"}\n"
"\n"
"QTableWidget::item {\n"
"    padding: 6px;\n"
"    border-bottom: 1px solid rgba(200, 220, 255, 0.3);\n"
"}"
                        "\n"
"\n"
"QTableWidget::item:hover {\n"
"    background-color: rgba(74, 144, 217, 0.15);\n"
"}\n"
"\n"
"QTableWidget::item:selected {\n"
"    background-color: rgba(74, 144, 217, 0.25);\n"
"    color: #1E3A5F;\n"
"}\n"
"\n"
"QTableWidget::horizontalHeader {\n"
"    background-color: rgba(232, 244, 255, 0.9);\n"
"    border-radius: 8px;\n"
"}\n"
"\n"
"QTableWidget::horizontalHeader::section {\n"
"    background-color: rgba(232, 244, 255, 0.9);\n"
"    color: #1A3050;\n"
"    padding: 8px;\n"
"    font-weight: 600;\n"
"    border: none;\n"
"    border-right: 1px solid rgba(200, 220, 255, 0.4);\n"
"}\n"
"\n"
"QTableWidget::horizontalHeader::section:last-child {\n"
"    border-right: none;\n"
"}\n"
"\n"
"QTableWidget::verticalHeader {\n"
"    background-color: rgba(232, 244, 255, 0.6);\n"
"}\n"
"\n"
"QTableWidget::verticalHeader::section {\n"
"    background-color: rgba(232, 244, 255, 0.6);\n"
"    color: #4A6080;\n"
"    padding: 6px;\n"
"    border: none;\n"
"    border-bottom: 1px solid rgba(200, 220, 255, 0.3)"
                        ";\n"
"}\n"
"\n"
"/* ------------------------------\n"
"   \344\270\213\346\213\211\346\241\206\347\276\216\345\214\226\n"
"------------------------------ */\n"
"QComboBox {\n"
"    background-color: rgba(255, 255, 255, 0.8);\n"
"    border: 1px solid rgba(200, 220, 255, 0.5);\n"
"    border-radius: 8px;\n"
"    padding: 6px 12px;\n"
"    color: #1A3050;\n"
"}\n"
"\n"
"QComboBox:hover {\n"
"    border-color: #4A90D9;\n"
"}\n"
"\n"
"QComboBox::drop-down {\n"
"    border: none;\n"
"}\n"
"\n"
"QComboBox::down-arrow {\n"
"    image: url(:/images/arrow-down.svg);\n"
"    width: 16px;\n"
"    height: 16px;\n"
"}\n"
"\n"
"/* ------------------------------\n"
"   \347\212\266\346\200\201\346\240\217\n"
"------------------------------ */\n"
"QStatusBar {\n"
"    background-color: rgba(232, 244, 255, 0.7);\n"
"    border-top: 1px solid rgba(200, 220, 255, 0.3);\n"
"    color: #1A3050;\n"
"}\n"
"\n"
"/* ------------------------------\n"
"   \346\273\232\345\212\250\346\235\241\347\276\216\345\214\226 - \350\223\235\350\211"
                        "\262\347\263\273\n"
"------------------------------ */\n"
"QScrollBar:vertical {\n"
"    background-color: rgba(200, 220, 255, 0.2);\n"
"    width: 8px;\n"
"    border-radius: 4px;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical {\n"
"    background-color: rgba(74, 144, 217, 0.4);\n"
"    border-radius: 4px;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical:hover {\n"
"    background-color: rgba(74, 144, 217, 0.6);\n"
"}\n"
"\n"
"QScrollBar:horizontal {\n"
"    background-color: rgba(200, 220, 255, 0.2);\n"
"    height: 8px;\n"
"    border-radius: 4px;\n"
"}\n"
"\n"
"QScrollBar::handle:horizontal {\n"
"    background-color: rgba(74, 144, 217, 0.4);\n"
"    border-radius: 4px;\n"
"}\n"
"\n"
"QScrollBar::handle:horizontal:hover {\n"
"    background-color: rgba(74, 144, 217, 0.6);\n"
"}\n"
"\n"
"/* ------------------------------\n"
"   \344\273\252\350\241\250\347\233\230\346\226\207\345\255\227\n"
"------------------------------ */\n"
"QLabel#gaugeLabelTemp,\n"
"QLabel#gaugeLabelHumi {\n"
"    font-size: 12px;\n"
"    color"
                        ": #4A6080;\n"
"}\n"
"\n"
"QLabel#gaugeValueTemp,\n"
"QLabel#gaugeValueHumi {\n"
"    font-size: 20px;\n"
"    font-weight: bold;\n"
"    color: #1E3A5F;\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        leftPanel = new QFrame(centralwidget);
        leftPanel->setObjectName(QString::fromUtf8("leftPanel"));
        leftPanel->setGeometry(QRect(0, 0, 240, 600));
        leftPanel->setFrameShape(QFrame::StyledPanel);
        leftPanel->setFrameShadow(QFrame::Raised);
        lblLogo_2 = new QLabel(leftPanel);
        lblLogo_2->setObjectName(QString::fromUtf8("lblLogo_2"));
        lblLogo_2->setGeometry(QRect(50, 0, 171, 51));
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        lblLogo_2->setFont(font);
        lblTimeDisplay = new QLabel(leftPanel);
        lblTimeDisplay->setObjectName(QString::fromUtf8("lblTimeDisplay"));
        lblTimeDisplay->setGeometry(QRect(50, 50, 121, 19));
        QFont font1;
        font1.setPointSize(11);
        lblTimeDisplay->setFont(font1);
        label_2 = new QLabel(leftPanel);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(60, 260, 121, 20));
        label_2->setFont(font1);
        btnComfortMode = new QPushButton(leftPanel);
        btnComfortMode->setObjectName(QString::fromUtf8("btnComfortMode"));
        btnComfortMode->setGeometry(QRect(40, 310, 161, 41));
        btnSleepMode = new QPushButton(leftPanel);
        btnSleepMode->setObjectName(QString::fromUtf8("btnSleepMode"));
        btnSleepMode->setGeometry(QRect(40, 370, 161, 41));
        btnCinemaMode = new QPushButton(leftPanel);
        btnCinemaMode->setObjectName(QString::fromUtf8("btnCinemaMode"));
        btnCinemaMode->setGeometry(QRect(40, 430, 161, 41));
        btnManualMode = new QPushButton(leftPanel);
        btnManualMode->setObjectName(QString::fromUtf8("btnManualMode"));
        btnManualMode->setGeometry(QRect(40, 490, 161, 41));
        btnNavLogs = new QPushButton(leftPanel);
        btnNavLogs->setObjectName(QString::fromUtf8("btnNavLogs"));
        btnNavLogs->setGeometry(QRect(40, 83, 161, 41));
        btnNavTempHumi = new QPushButton(leftPanel);
        btnNavTempHumi->setObjectName(QString::fromUtf8("btnNavTempHumi"));
        btnNavTempHumi->setGeometry(QRect(40, 140, 161, 41));
        btnNavControl = new QPushButton(leftPanel);
        btnNavControl->setObjectName(QString::fromUtf8("btnNavControl"));
        btnNavControl->setGeometry(QRect(40, 197, 161, 41));
        rightPanel = new QFrame(centralwidget);
        rightPanel->setObjectName(QString::fromUtf8("rightPanel"));
        rightPanel->setGeometry(QRect(240, 0, 781, 600));
        rightPanel->setFrameShape(QFrame::StyledPanel);
        rightPanel->setFrameShadow(QFrame::Raised);
        stackedWidget = new QStackedWidget(rightPanel);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(0, 0, 520, 600));
        homepage = new QWidget();
        homepage->setObjectName(QString::fromUtf8("homepage"));
        stackedWidget->addWidget(homepage);
        temp = new QWidget();
        temp->setObjectName(QString::fromUtf8("temp"));
        chartViewWidget = new QChartView(temp);
        chartViewWidget->setObjectName(QString::fromUtf8("chartViewWidget"));
        chartViewWidget->setGeometry(QRect(40, 270, 471, 221));
        lblStatusLed = new QLabel(temp);
        lblStatusLed->setObjectName(QString::fromUtf8("lblStatusLed"));
        lblStatusLed->setGeometry(QRect(310, 30, 91, 19));
        btnConnect = new QPushButton(temp);
        btnConnect->setObjectName(QString::fromUtf8("btnConnect"));
        btnConnect->setGeometry(QRect(190, 20, 93, 28));
        cmbSerialPorts = new QComboBox(temp);
        cmbSerialPorts->setObjectName(QString::fromUtf8("cmbSerialPorts"));
        cmbSerialPorts->setGeometry(QRect(40, 20, 121, 25));
        cardFan = new QFrame(temp);
        cardFan->setObjectName(QString::fromUtf8("cardFan"));
        cardFan->setGeometry(QRect(40, 100, 130, 130));
        cardFan->setFrameShape(QFrame::StyledPanel);
        cardFan->setFrameShadow(QFrame::Raised);
        lblFanIcon = new QLabel(cardFan);
        lblFanIcon->setObjectName(QString::fromUtf8("lblFanIcon"));
        lblFanIcon->setGeometry(QRect(40, 30, 60, 60));
        QFont font2;
        font2.setPointSize(9);
        lblFanIcon->setFont(font2);
        chkFanSwitch = new QCheckBox(cardFan);
        chkFanSwitch->setObjectName(QString::fromUtf8("chkFanSwitch"));
        chkFanSwitch->setEnabled(false);
        chkFanSwitch->setGeometry(QRect(40, 100, 51, 23));
        cardHeat = new QFrame(temp);
        cardHeat->setObjectName(QString::fromUtf8("cardHeat"));
        cardHeat->setEnabled(false);
        cardHeat->setGeometry(QRect(190, 100, 130, 130));
        cardHeat->setFrameShape(QFrame::StyledPanel);
        cardHeat->setFrameShadow(QFrame::Raised);
        lblFireIcon = new QLabel(cardHeat);
        lblFireIcon->setObjectName(QString::fromUtf8("lblFireIcon"));
        lblFireIcon->setGeometry(QRect(40, 30, 60, 60));
        chkHeatSwitch = new QCheckBox(cardHeat);
        chkHeatSwitch->setObjectName(QString::fromUtf8("chkHeatSwitch"));
        chkHeatSwitch->setGeometry(QRect(40, 100, 51, 23));
        lblTempMin = new QLabel(temp);
        lblTempMin->setObjectName(QString::fromUtf8("lblTempMin"));
        lblTempMin->setGeometry(QRect(470, 70, 31, 19));
        label = new QLabel(temp);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(440, 70, 21, 19));
        label_3 = new QLabel(temp);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(330, 70, 69, 19));
        lblTempMax = new QLabel(temp);
        lblTempMax->setObjectName(QString::fromUtf8("lblTempMax"));
        lblTempMax->setGeometry(QRect(400, 70, 31, 19));
        cardThreshold = new QFrame(temp);
        cardThreshold->setObjectName(QString::fromUtf8("cardThreshold"));
        cardThreshold->setGeometry(QRect(340, 100, 171, 131));
        cardThreshold->setFrameShape(QFrame::StyledPanel);
        cardThreshold->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(cardThreshold);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        btnTempMaxDown = new QPushButton(cardThreshold);
        btnTempMaxDown->setObjectName(QString::fromUtf8("btnTempMaxDown"));

        gridLayout->addWidget(btnTempMaxDown, 1, 1, 1, 1);

        btnTempMinDown = new QPushButton(cardThreshold);
        btnTempMinDown->setObjectName(QString::fromUtf8("btnTempMinDown"));

        gridLayout->addWidget(btnTempMinDown, 2, 1, 1, 1);

        btnTempMinUp = new QPushButton(cardThreshold);
        btnTempMinUp->setObjectName(QString::fromUtf8("btnTempMinUp"));

        gridLayout->addWidget(btnTempMinUp, 2, 0, 1, 1);

        btnTempMaxUp = new QPushButton(cardThreshold);
        btnTempMaxUp->setObjectName(QString::fromUtf8("btnTempMaxUp"));

        gridLayout->addWidget(btnTempMaxUp, 1, 0, 1, 1);

        btnToRealtimeData = new QPushButton(temp);
        btnToRealtimeData->setObjectName(QString::fromUtf8("btnToRealtimeData"));
        btnToRealtimeData->setGeometry(QRect(40, 500, 121, 31));
        stackedWidget->addWidget(temp);
        history = new QWidget();
        history->setObjectName(QString::fromUtf8("history"));
        btnBackToEnv = new QPushButton(history);
        btnBackToEnv->setObjectName(QString::fromUtf8("btnBackToEnv"));
        btnBackToEnv->setGeometry(QRect(370, 450, 121, 41));
        tableHistory = new QTableWidget(history);
        tableHistory->setObjectName(QString::fromUtf8("tableHistory"));
        tableHistory->setGeometry(QRect(30, 80, 461, 351));
        stackedWidget->addWidget(history);
        tempHistory = new QWidget();
        tempHistory->setObjectName(QString::fromUtf8("tempHistory"));
        btnBackFromTempHistory = new QPushButton(tempHistory);
        btnBackFromTempHistory->setObjectName(QString::fromUtf8("btnBackFromTempHistory"));
        btnBackFromTempHistory->setGeometry(QRect(390, 450, 121, 41));
        tableTempHistory = new QTableWidget(tempHistory);
        tableTempHistory->setObjectName(QString::fromUtf8("tableTempHistory"));
        tableTempHistory->setGeometry(QRect(50, 80, 461, 351));
        lbTempStart = new QLabel(tempHistory);
        lbTempStart->setObjectName(QString::fromUtf8("lbTempStart"));
        lbTempStart->setGeometry(QRect(110, 30, 30, 19));
        deTempStart = new QDateEdit(tempHistory);
        deTempStart->setObjectName(QString::fromUtf8("deTempStart"));
        deTempStart->setGeometry(QRect(140, 27, 100, 24));
        deTempStart->setCalendarPopup(true);
        lbTempEnd = new QLabel(tempHistory);
        lbTempEnd->setObjectName(QString::fromUtf8("lbTempEnd"));
        lbTempEnd->setGeometry(QRect(250, 30, 30, 19));
        deTempEnd = new QDateEdit(tempHistory);
        deTempEnd->setObjectName(QString::fromUtf8("deTempEnd"));
        deTempEnd->setGeometry(QRect(280, 27, 100, 24));
        deTempEnd->setCalendarPopup(true);
        btnTempFilter = new QPushButton(tempHistory);
        btnTempFilter->setObjectName(QString::fromUtf8("btnTempFilter"));
        btnTempFilter->setGeometry(QRect(390, 25, 60, 28));
        lbTempPage = new QLabel(tempHistory);
        lbTempPage->setObjectName(QString::fromUtf8("lbTempPage"));
        lbTempPage->setGeometry(QRect(120, 463, 80, 19));
        btnTempPrev = new QPushButton(tempHistory);
        btnTempPrev->setObjectName(QString::fromUtf8("btnTempPrev"));
        btnTempPrev->setEnabled(false);
        btnTempPrev->setGeometry(QRect(210, 460, 70, 24));
        btnTempNext = new QPushButton(tempHistory);
        btnTempNext->setObjectName(QString::fromUtf8("btnTempNext"));
        btnTempNext->setGeometry(QRect(290, 460, 70, 24));
        stackedWidget->addWidget(tempHistory);
        camera = new QWidget();
        camera->setObjectName(QString::fromUtf8("camera"));
        stackedWidget->addWidget(camera);
        operationLogs = new QWidget();
        operationLogs->setObjectName(QString::fromUtf8("operationLogs"));
        btnBackFromLogs = new QPushButton(operationLogs);
        btnBackFromLogs->setObjectName(QString::fromUtf8("btnBackFromLogs"));
        btnBackFromLogs->setGeometry(QRect(390, 460, 121, 41));
        tableOperationLogs = new QTableWidget(operationLogs);
        tableOperationLogs->setObjectName(QString::fromUtf8("tableOperationLogs"));
        tableOperationLogs->setGeometry(QRect(30, 90, 481, 351));
        leOpLogDevice = new QLineEdit(operationLogs);
        leOpLogDevice->setObjectName(QString::fromUtf8("leOpLogDevice"));
        leOpLogDevice->setGeometry(QRect(40, 28, 90, 24));
        lbOpLogStart = new QLabel(operationLogs);
        lbOpLogStart->setObjectName(QString::fromUtf8("lbOpLogStart"));
        lbOpLogStart->setGeometry(QRect(140, 31, 30, 19));
        deOpLogStart = new QDateEdit(operationLogs);
        deOpLogStart->setObjectName(QString::fromUtf8("deOpLogStart"));
        deOpLogStart->setGeometry(QRect(170, 28, 100, 24));
        deOpLogStart->setCalendarPopup(true);
        lbOpLogEnd = new QLabel(operationLogs);
        lbOpLogEnd->setObjectName(QString::fromUtf8("lbOpLogEnd"));
        lbOpLogEnd->setGeometry(QRect(280, 31, 30, 19));
        deOpLogEnd = new QDateEdit(operationLogs);
        deOpLogEnd->setObjectName(QString::fromUtf8("deOpLogEnd"));
        deOpLogEnd->setGeometry(QRect(310, 28, 100, 24));
        deOpLogEnd->setCalendarPopup(true);
        btnOpLogFilter = new QPushButton(operationLogs);
        btnOpLogFilter->setObjectName(QString::fromUtf8("btnOpLogFilter"));
        btnOpLogFilter->setGeometry(QRect(420, 26, 60, 28));
        lbOpLogPage = new QLabel(operationLogs);
        lbOpLogPage->setObjectName(QString::fromUtf8("lbOpLogPage"));
        lbOpLogPage->setGeometry(QRect(120, 470, 80, 19));
        btnOpLogPrev = new QPushButton(operationLogs);
        btnOpLogPrev->setObjectName(QString::fromUtf8("btnOpLogPrev"));
        btnOpLogPrev->setEnabled(false);
        btnOpLogPrev->setGeometry(QRect(210, 467, 70, 24));
        btnOpLogNext = new QPushButton(operationLogs);
        btnOpLogNext->setObjectName(QString::fromUtf8("btnOpLogNext"));
        btnOpLogNext->setGeometry(QRect(290, 467, 70, 24));
        stackedWidget->addWidget(operationLogs);
        lastPanel = new QFrame(rightPanel);
        lastPanel->setObjectName(QString::fromUtf8("lastPanel"));
        lastPanel->setGeometry(QRect(520, 0, 284, 600));
        lastPanel->setFrameShape(QFrame::StyledPanel);
        lastPanel->setFrameShadow(QFrame::Raised);
        humiCard = new QFrame(lastPanel);
        humiCard->setObjectName(QString::fromUtf8("humiCard"));
        humiCard->setGeometry(QRect(40, 300, 200, 200));
        humiCard->setFrameShape(QFrame::StyledPanel);
        humiCard->setFrameShadow(QFrame::Raised);
        lblCurrentHumi = new QLabel(humiCard);
        lblCurrentHumi->setObjectName(QString::fromUtf8("lblCurrentHumi"));
        lblCurrentHumi->setGeometry(QRect(70, 0, 69, 61));
        QFont font3;
        font3.setPointSize(19);
        font3.setBold(true);
        lblCurrentHumi->setFont(font3);
        gaugeHumi = new GaugeWidget(humiCard);
        gaugeHumi->setObjectName(QString::fromUtf8("gaugeHumi"));
        gaugeHumi->setGeometry(QRect(40, 50, 120, 80));
        tempCard = new QFrame(lastPanel);
        tempCard->setObjectName(QString::fromUtf8("tempCard"));
        tempCard->setGeometry(QRect(40, 70, 200, 200));
        tempCard->setFrameShape(QFrame::StyledPanel);
        tempCard->setFrameShadow(QFrame::Raised);
        lblCurrentTemp = new QLabel(tempCard);
        lblCurrentTemp->setObjectName(QString::fromUtf8("lblCurrentTemp"));
        lblCurrentTemp->setGeometry(QRect(70, 0, 101, 61));
        lblCurrentTemp->setFont(font3);
        lblTempTargetRange = new QLabel(tempCard);
        lblTempTargetRange->setObjectName(QString::fromUtf8("lblTempTargetRange"));
        lblTempTargetRange->setGeometry(QRect(60, 160, 81, 19));
        gaugeTemp = new GaugeWidget(tempCard);
        gaugeTemp->setObjectName(QString::fromUtf8("gaugeTemp"));
        gaugeTemp->setGeometry(QRect(40, 60, 120, 80));
        switchAlarm = new QCheckBox(lastPanel);
        switchAlarm->setObjectName(QString::fromUtf8("switchAlarm"));
        switchAlarm->setGeometry(QRect(40, 30, 201, 23));
        switchAlarm->setFont(font1);
        btnHistory = new QPushButton(lastPanel);
        btnHistory->setObjectName(QString::fromUtf8("btnHistory"));
        btnHistory->setGeometry(QRect(90, 510, 121, 31));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1024, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        lblLogo_2->setText(QCoreApplication::translate("MainWindow", "SMART HOME", nullptr));
        lblTimeDisplay->setText(QCoreApplication::translate("MainWindow", "2026-05-19", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\344\270\200\351\224\256\345\210\207\346\215\242\346\250\241\345\274\217", nullptr));
        btnComfortMode->setText(QCoreApplication::translate("MainWindow", "\350\210\222\351\200\202\346\250\241\345\274\217", nullptr));
        btnSleepMode->setText(QCoreApplication::translate("MainWindow", "\347\235\241\347\234\240\346\250\241\345\274\217", nullptr));
        btnCinemaMode->setText(QCoreApplication::translate("MainWindow", "\350\247\202\345\275\261\346\250\241\345\274\217", nullptr));
        btnManualMode->setText(QCoreApplication::translate("MainWindow", "\346\211\213\345\212\250\346\250\241\345\274\217", nullptr));
        btnNavLogs->setText(QCoreApplication::translate("MainWindow", "\346\223\215\344\275\234\346\227\245\345\277\227", nullptr));
        btnNavTempHumi->setText(QCoreApplication::translate("MainWindow", "\346\270\251\346\271\277\345\272\246\346\243\200\346\265\213", nullptr));
        btnNavControl->setText(QCoreApplication::translate("MainWindow", "\345\205\250\345\261\213\346\216\247\345\210\266", nullptr));
        lblStatusLed->setText(QCoreApplication::translate("MainWindow", "\342\227\213 \347\241\254\344\273\266\346\234\252\350\277\236\346\216\245", nullptr));
        btnConnect->setText(QCoreApplication::translate("MainWindow", "\350\277\236\346\216\245\347\241\254\344\273\266", nullptr));
        lblFanIcon->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        chkFanSwitch->setText(QString());
        lblFireIcon->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        chkHeatSwitch->setText(QString());
        lblTempMin->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "~", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\346\270\251\345\272\246\351\230\210\345\200\274\357\274\232", nullptr));
        lblTempMax->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        btnTempMaxDown->setText(QCoreApplication::translate("MainWindow", "MAX-", nullptr));
        btnTempMinDown->setText(QCoreApplication::translate("MainWindow", "MIN-", nullptr));
        btnTempMinUp->setText(QCoreApplication::translate("MainWindow", "MIN+", nullptr));
        btnTempMaxUp->setText(QCoreApplication::translate("MainWindow", "MAX+", nullptr));
        btnToRealtimeData->setText(QCoreApplication::translate("MainWindow", "\345\256\236\346\227\266\346\225\260\346\215\256\350\241\250\346\240\274", nullptr));
        btnBackToEnv->setText(QCoreApplication::translate("MainWindow", "\350\277\224\345\233\236\346\270\251\346\271\277\345\272\246\351\241\265", nullptr));
        btnBackFromTempHistory->setText(QCoreApplication::translate("MainWindow", "\350\277\224\345\233\236\346\270\251\346\271\277\345\272\246\351\241\265", nullptr));
        lbTempStart->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213:", nullptr));
        deTempStart->setDisplayFormat(QCoreApplication::translate("MainWindow", "yyyy-MM-dd", nullptr));
        lbTempEnd->setText(QCoreApplication::translate("MainWindow", "\347\273\223\346\235\237:", nullptr));
        deTempEnd->setDisplayFormat(QCoreApplication::translate("MainWindow", "yyyy-MM-dd", nullptr));
        btnTempFilter->setText(QCoreApplication::translate("MainWindow", "\347\255\233\351\200\211", nullptr));
        lbTempPage->setText(QCoreApplication::translate("MainWindow", "\347\254\254 1/1 \351\241\265", nullptr));
        btnTempPrev->setText(QCoreApplication::translate("MainWindow", "\344\270\212\344\270\200\351\241\265", nullptr));
        btnTempNext->setText(QCoreApplication::translate("MainWindow", "\344\270\213\344\270\200\351\241\265", nullptr));
        btnBackFromLogs->setText(QCoreApplication::translate("MainWindow", "\350\277\224\345\233\236\351\246\226\351\241\265", nullptr));
        leOpLogDevice->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\256\276\345\244\207\345\220\215\347\247\260", nullptr));
        lbOpLogStart->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213:", nullptr));
        deOpLogStart->setDisplayFormat(QCoreApplication::translate("MainWindow", "yyyy-MM-dd", nullptr));
        lbOpLogEnd->setText(QCoreApplication::translate("MainWindow", "\347\273\223\346\235\237:", nullptr));
        deOpLogEnd->setDisplayFormat(QCoreApplication::translate("MainWindow", "yyyy-MM-dd", nullptr));
        btnOpLogFilter->setText(QCoreApplication::translate("MainWindow", "\347\255\233\351\200\211", nullptr));
        lbOpLogPage->setText(QCoreApplication::translate("MainWindow", "\347\254\254 1/1 \351\241\265", nullptr));
        btnOpLogPrev->setText(QCoreApplication::translate("MainWindow", "\344\270\212\344\270\200\351\241\265", nullptr));
        btnOpLogNext->setText(QCoreApplication::translate("MainWindow", "\344\270\213\344\270\200\351\241\265", nullptr));
        lblCurrentHumi->setText(QCoreApplication::translate("MainWindow", "50.0", nullptr));
        lblCurrentTemp->setText(QCoreApplication::translate("MainWindow", "25.0", nullptr));
        lblTempTargetRange->setText(QCoreApplication::translate("MainWindow", " 20\302\260C - 30\302\260C", nullptr));
        switchAlarm->setText(QCoreApplication::translate("MainWindow", "\350\234\202\351\270\243\345\231\250\350\255\246\346\212\245", nullptr));
        btnHistory->setText(QCoreApplication::translate("MainWindow", "\346\270\251\346\271\277\345\272\246\345\216\206\345\217\262", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
