#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>
#include <QtCharts>
#include <QPropertyAnimation>
#include <QGraphicsRotation>
#include "homepage.h"
#include "camerawidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateCurrentTime();                // 更新系统时间
    void onConnectBtnClicked();              // 串口连接/断开按钮
    void readSerialData();                   // 读取串口数据

    // 温度阈值按钮
    void on_btnTempMaxUp_clicked();
    void on_btnTempMaxDown_clicked();
    void on_btnTempMinUp_clicked();
    void on_btnTempMinDown_clicked();
    void on_switchAlarm_toggled(bool checked);
    void initHistoryTable();
    void addHistory(double temp, double humi);
    void loadOperationLogs();
    void loadOperationLogs(const QString& deviceName, const QString& startTime, const QString& endTime);
    void loadTempHistory();
    void loadTempHistory(const QString& startTime, const QString& endTime);
    // 分页相关
    void on_opLogPrevPage();
    void on_opLogNextPage();
    void on_opLogFilter();
    void on_tempHistoryPrevPage();
    void on_tempHistoryNextPage();
    void on_tempHistoryFilter();
    // 模式切换（就加这4行）
    void on_btnComfortMode_clicked();
    void on_btnSleepMode_clicked();
    void on_btnCinemaMode_clicked();
    void on_btnManualMode_clicked();
private:
    Ui::MainWindow *ui;

    QSerialPort *serial;                     // 串口对象
    QTimer *timeTimer;                       // 定时器（刷新时间）
    int dataPointCount;                      // 波形图数据点计数

    double tempMax;                          // 温度上限
    double tempMin;                          // 温度下限

    QLineSeries *tempSeries;                 // 温度曲线
    QLineSeries *humiSeries;                 // 湿度曲线
    QChart *chart;                           // 图表对象
    QValueAxis *axisX, *axisY;                // 坐标轴

    QGraphicsRotation *fanRotation;          // 风扇旋转
    QPropertyAnimation *fanAnimation;        // 风扇动画
    bool heaterOn = false;  // 加热继电器状态
    bool coolerOn = false;  // 制冷继电器状态
    QWidget *gaugeTemp;
    QWidget *gaugeHumi;
    HomePage *homePage;
    CameraWidget *cameraPage;
    QDateTime lastHistoryTime;                // 上次记录历史数据的时间
    QString serialBuffer;                      // 串口数据缓冲区

    // 分页相关
    int opLogCurrentPage = 0;
    int opLogTotalPages = 0;
    int tempHistoryCurrentPage = 0;
    int tempHistoryTotalPages = 0;
    const int pageSize = 10;

    // 筛选条件
    QString opLogDeviceFilter;
    QString opLogStartFilter;
    QString opLogEndFilter;
    QString tempStartFilter;
    QString tempEndFilter;

    void drawGauge(QWidget *widget, double value, double min, double max, QString unit, QColor color);

    void initChart();                        // 初始化图表
    void initAnimation();                    // 初始化风扇动画
    void updateThresholdUI();                // 更新阈值显示
    void updateRelayUI();
};

#endif // MAINWINDOW_H