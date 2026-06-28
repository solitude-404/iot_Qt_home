#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gaugewidget.h"
#include "homepage.h"
#include "camerawidget.h"
#include "dbhelper.h"
#include <QMessageBox>
#include <QDateTime>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtCharts>
#include <QList>
#include <QMap>
#include <QToolTip>
#include <QCursor>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化串口
    serial = new QSerialPort(this);
    dataPointCount = 0;

    // 初始阈值（与单片机一致）
    tempMax = 31;
    tempMin = 29;

    // 初始化继电器状态
    heaterOn = false;
    coolerOn = false;

    // 定时器：每秒刷新当前时间
    timeTimer = new QTimer(this);
    connect(timeTimer, &QTimer::timeout, this, &MainWindow::updateCurrentTime);
    timeTimer->start(1000);
    updateCurrentTime();

    // 扫描可用串口
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts())
        ui->cmbSerialPorts->addItem(info.portName());

    // 初始化图表与动画
    initChart();
    initAnimation();

    // 设置图标
    ui->lblFanIcon->setPixmap(
        QPixmap(":/images/fan.svg")
            .scaled(54,54,Qt::KeepAspectRatio,Qt::SmoothTransformation)
        );
    ui->lblFireIcon->setPixmap(
        QPixmap(":/images/fire.svg")
            .scaled(54,54,Qt::KeepAspectRatio,Qt::SmoothTransformation)
        );

    ui->gaugeTemp->setRange(20, 35);
    ui->gaugeTemp->setColor(QColor("#FF7E5F"));
    ui->gaugeTemp->setUnit("℃");
    ui->gaugeTemp->setValue(25.0);

    // 初始化湿度仪表盘（50~100%）
    ui->gaugeHumi->setRange(50, 100);
    ui->gaugeHumi->setColor(QColor("#4FACFE"));
    ui->gaugeHumi->setUnit("%RH");
    ui->gaugeHumi->setValue(60.0);
    // 初始禁用手动开关
    ui->chkFanSwitch->setEnabled(false);
    ui->chkHeatSwitch->setEnabled(false);

    // 按钮绑定
    connect(ui->btnConnect, &QPushButton::clicked, this, &MainWindow::onConnectBtnClicked);
    connect(ui->btnTempMaxUp, &QPushButton::clicked, this, &MainWindow::on_btnTempMaxUp_clicked);
    connect(ui->btnTempMaxDown, &QPushButton::clicked, this, &MainWindow::on_btnTempMaxDown_clicked);
    connect(ui->btnTempMinUp, &QPushButton::clicked, this, &MainWindow::on_btnTempMinUp_clicked);
    connect(ui->btnTempMinDown, &QPushButton::clicked, this, &MainWindow::on_btnTempMinDown_clicked);

    // 串口数据接收
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
    // 初始化历史表格（只加这一行）
    initHistoryTable();
    connect(ui->btnHistory, &QPushButton::clicked, [=](){
        loadTempHistory();
        ui->stackedWidget->setCurrentWidget(ui->tempHistory);
    });
    connect(ui->btnBackToEnv, &QPushButton::clicked, [=](){
        ui->stackedWidget->setCurrentWidget(ui->temp);
    });
    connect(ui->btnBackFromTempHistory, &QPushButton::clicked, [=](){
        ui->stackedWidget->setCurrentWidget(ui->temp);
    });
    connect(ui->btnToRealtimeData, &QPushButton::clicked, [=](){
        ui->stackedWidget->setCurrentWidget(ui->history);
    });
    connect(ui->btnNavTempHumi, &QPushButton::clicked, [=](){
        ui->stackedWidget->setCurrentWidget(ui->temp);
    });
    connect(ui->btnNavLogs, &QPushButton::clicked, [=](){
        loadOperationLogs();
        ui->stackedWidget->setCurrentWidget(ui->operationLogs);
    });


    // ======================
    // 创建首页
    // ======================
    homePage = new HomePage(this);
    ui->stackedWidget->addWidget(homePage);

    connect(ui->btnNavControl, &QPushButton::clicked, [=](){
        ui->stackedWidget->setCurrentWidget(homePage);
    });
    connect(ui->btnBackFromLogs, &QPushButton::clicked, [=](){
        ui->stackedWidget->setCurrentWidget(homePage);
    });

    // ======================
    // 首页跳转绑定
    // ======================
    connect(homePage, &HomePage::goToTempPage, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->temp);
    });

    connect(homePage, &HomePage::goToCameraPage, this, [=](){
        ui->stackedWidget->setCurrentWidget(cameraPage);
    });
    ui->stackedWidget->setCurrentWidget(homePage);

    // 构造函数里加
    cameraPage = new CameraWidget(this);
    ui->stackedWidget->addWidget(cameraPage);

    // 返回主页
    connect(cameraPage, &CameraWidget::backToHome, [=](){
        ui->stackedWidget->setCurrentWidget(homePage);
    });


    // 刷新界面显示
    updateThresholdUI();
    updateRelayUI();
}

MainWindow::~MainWindow()
{
    if (serial->isOpen())
        serial->close();
    delete ui;
}

// 更新当前时间显示
void MainWindow::updateCurrentTime()
{
    ui->lblTimeDisplay->setText(
        QDateTime::currentDateTime().toString("HH:mm:ss\nyyyy/MM/dd")
        );
}

// 初始化波形图表
void MainWindow::initChart()
{
    tempSeries = new QLineSeries();
    humiSeries = new QLineSeries();

    tempSeries->setName("温度 (°C)");
    humiSeries->setName("湿度 (%RH)");

    // 温度：橙色粗线
    tempSeries->setPen(QPen(QColor("#FF7E5F"), 3));
    // 湿度：蓝色更粗 + 更显眼，曲线更明显
    humiSeries->setPen(QPen(QColor("#4FACFE"), 4));

    chart = new QChart();
    chart->addSeries(tempSeries);
    chart->addSeries(humiSeries);
    chart->legend()->setAlignment(Qt::AlignTop);
    chart->setMargins(QMargins(10, 10, 10, 10));

    // 浅色主题（你要的）
    chart->setTheme(QChart::ChartThemeLight);

    // X 轴（时间轴）
    axisX = new QValueAxis();
    axisX->setRange(0, 30);
    chart->addAxis(axisX, Qt::AlignBottom);

    // ======================
    // 温度 Y 轴：20~35℃（波动放大）
    // ======================
    QValueAxis *axisY_temp = new QValueAxis();
    axisY_temp->setRange(20, 35);
    axisY_temp->setLabelsColor(QColor("#FF7E5F"));
    chart->addAxis(axisY_temp, Qt::AlignRight);

    // ======================
    // 湿度 Y 轴：50~100%（范围更紧凑 → 曲线更明显）
    // ======================
    QValueAxis *axisY_humi = new QValueAxis();
    axisY_humi->setRange(50, 100);
    axisY_humi->setLabelsColor(QColor("#4FACFE"));
    chart->addAxis(axisY_humi, Qt::AlignLeft);

    // 绑定轴
    tempSeries->attachAxis(axisX);
    tempSeries->attachAxis(axisY_temp);

    humiSeries->attachAxis(axisX);
    humiSeries->attachAxis(axisY_humi);

    ui->chartViewWidget->setChart(chart);
}

// 初始化风扇旋转动画
void MainWindow::initAnimation()
{
    fanRotation = new QGraphicsRotation(this);
    fanRotation->setOrigin(QVector3D(27, 27, 0));

    fanAnimation = new QPropertyAnimation(fanRotation, "angle");
    fanAnimation->setDuration(1000);
    fanAnimation->setStartValue(0);
    fanAnimation->setEndValue(360);
    fanAnimation->setLoopCount(-1);

    connect(fanAnimation, &QPropertyAnimation::valueChanged, this, [=](){
        ui->lblFanIcon->setStyleSheet(
            QString("QLabel{transform:rotate(%1deg);}")
                .arg(fanRotation->angle())
            );
    });
}

// 连接/断开串口
void MainWindow::onConnectBtnClicked()
{
    if (!serial->isOpen()) {
        serial->setPortName(ui->cmbSerialPorts->currentText());
        serial->setBaudRate(QSerialPort::Baud9600);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);

        if (serial->open(QIODevice::ReadWrite)) {
            ui->btnConnect->setText("断开硬件");
            ui->lblStatusLed->setText("● 已连接51系统");
            ui->lblStatusLed->setStyleSheet("color:#2ECC71;font-weight:bold;");

            QString savedMax = DBHelper::getConfig("tempMax", "");
            QString savedMin = DBHelper::getConfig("tempMin", "");

            if (!savedMax.isEmpty() && !savedMin.isEmpty()) {
                double targetMax = savedMax.toDouble();
                double targetMin = savedMin.toDouble();

                for (int i = 0; i < targetMax - 30; i++) {
                    serial->write("1");
                    QThread::msleep(50);
                }
                for (int i = 0; i < 30 - targetMax; i++) {
                    serial->write("2");
                    QThread::msleep(50);
                }
                for (int i = 0; i < targetMin - 20; i++) {
                    serial->write("a");
                    QThread::msleep(50);
                }
                for (int i = 0; i < 20 - targetMin; i++) {
                    serial->write("b");
                    QThread::msleep(50);
                }
            }
        } else {
            QMessageBox::critical(this, "错误", "无法打开串口");
        }
    } else {
        serial->close();
        fanAnimation->stop();
        ui->btnConnect->setText("连接硬件");
        ui->lblStatusLed->setText("○ 硬件未连接");
        ui->lblStatusLed->setStyleSheet("color:#95A5A6;");
    }
}

// ================================
// 核心：读取并解析单片机数据
// ================================
void MainWindow::readSerialData()
{
    QByteArray raw = serial->readAll();
    QString data = QString::fromUtf8(raw).trimmed();
    if (data.isEmpty()) return;

    qDebug() << "[单片机]" << data;

    // 解析实时温湿度
    int tStart = data.indexOf("T:");
    int tEnd   = data.indexOf("C", tStart);
    if (tStart != -1 && tEnd != -1) {
        QString temp = data.mid(tStart + 2, tEnd - tStart - 2);
        ui->lblCurrentTemp->setText(temp);
        tempSeries->append(dataPointCount, temp.toDouble());
    }

    int hStart = data.indexOf("H:");
    int hEnd   = data.indexOf("%", hStart);
    if (hStart != -1 && hEnd != -1) {
        QString humi = data.mid(hStart + 2, hEnd - hStart - 2);
        ui->lblCurrentHumi->setText(humi);
        humiSeries->append(dataPointCount, humi.toDouble());
    }

    // ========== 解析温度阈值：TH:xx,TL:xx ==========
    int thIdx = data.indexOf("TH:");
    int tlIdx = data.indexOf("TL:");

    if (thIdx >= 0 && tlIdx >= 0) {
        int comma = data.indexOf(',', thIdx);

        // 提取温度上限
        QString thVal = data.mid(thIdx + 3, comma - thIdx - 3);
        tempMax = thVal.toDouble();

        // 提取温度下限
        QString tlVal = data.mid(tlIdx + 3);
        tempMin = tlVal.toDouble();

        // 更新界面
        updateThresholdUI();

        // 保存到数据库
        DBHelper::setConfig("tempMax", QString::number(tempMax));
        DBHelper::setConfig("tempMin", QString::number(tempMin));
    }

    // ================================
    // 🔥 继电器状态解析（完美版）
    // ================================
    if (data.contains("Heater ON")) {
        heaterOn = true;
        coolerOn = false;
    }
    else if (data.contains("Cooler ON")) {
        coolerOn = true;
        heaterOn = false;
    }
    // 温度正常 → 全部关闭
    else if (data.contains("Temperature normal") || data.contains("Relays OFF")) {
        heaterOn = false;
        coolerOn = false;
    }
    // 更新仪表盘
    double temp = ui->lblCurrentTemp->text().toDouble();
    double humi = ui->lblCurrentHumi->text().toDouble();
    // 示例：你解析出温度 temp，湿度 humi 后

    // 实时表格每次都更新
    QString time = QDateTime::currentDateTime().toString("HH:mm:ss");
    ui->tableHistory->insertRow(0);
    ui->tableHistory->setItem(0, 0, new QTableWidgetItem(QString::number(1)));
    ui->tableHistory->setItem(0, 1, new QTableWidgetItem(QString::number(temp, 'f', 1)));
    ui->tableHistory->setItem(0, 2, new QTableWidgetItem(QString::number(humi, 'f', 1)));
    ui->tableHistory->setItem(0, 3, new QTableWidgetItem(time));
    for(int i=0; i<ui->tableHistory->rowCount(); i++){
        ui->tableHistory->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
    }
    if(ui->tableHistory->rowCount() > 50){
        ui->tableHistory->removeRow(ui->tableHistory->rowCount()-1);
    }

    // 数据库历史记录每1分钟写入一次
    QDateTime now = QDateTime::currentDateTime();
    if (lastHistoryTime.isNull() || lastHistoryTime.secsTo(now) >= 60) {
        DBHelper::addTempHistory(temp, humi);
        lastHistoryTime = now;
    }

    ui->gaugeTemp->setValue(temp);
    ui->gaugeHumi->setValue(humi);

    // ------------------------------
    // 警报状态自动同步胶囊开关
    // ------------------------------
    if (data.contains("Alarm ON")) {
        ui->switchAlarm->setChecked(true);
        ui->switchAlarm->setText("蜂鸣器警报：已开启");
    }
    else if (data.contains("Alarm OFF")) {
        ui->switchAlarm->setChecked(false);
        ui->switchAlarm->setText("蜂鸣器警报：已关闭");
    }

    // 同步图标和开关
    updateRelayUI();

    // 波形图滚动
    if (++dataPointCount > 30)
        axisX->setRange(dataPointCount - 30, dataPointCount);
}

// 更新阈值显示
void MainWindow::updateThresholdUI()
{
    ui->lblTempMax->setText(QString::number(tempMax, 'f', 1));
    ui->lblTempMin->setText(QString::number(tempMin, 'f', 1));
    ui->lblTempTargetRange->setText(
        QString("%1 ~ %2 ").arg(tempMin, 0, 'f', 1).arg(tempMax, 0, 'f', 1)
        );
}

// ================================
// 继电器图标 + 胶囊开关 同步显示
// ================================
void MainWindow::updateRelayUI()
{
    // 加热
    if (heaterOn) {
        ui->lblFireIcon->setStyleSheet("");
        ui->lblFireIcon->setPixmap(QPixmap(":/images/fire.svg").scaled(54,54));
        ui->chkHeatSwitch->setChecked(true);
    } else {
        ui->lblFireIcon->setStyleSheet("QLabel{ opacity: 0.4; }");
        ui->chkHeatSwitch->setChecked(false);
    }

    // 冷却 + 风扇旋转
    if (coolerOn) {
        ui->lblFanIcon->setStyleSheet("");
        ui->lblFanIcon->setPixmap(QPixmap(":/images/fan.svg").scaled(54,54));
        ui->chkFanSwitch->setChecked(true);
        fanAnimation->start();
    } else {
        ui->lblFanIcon->setStyleSheet("QLabel{ opacity: 0.4; }");
        ui->chkFanSwitch->setChecked(false);
        fanAnimation->stop();
    }
}

// ================================
// 按钮：仅发送指令
// ================================
void MainWindow::on_btnTempMaxUp_clicked()
{
    if (serial->isOpen()) serial->write("1");
    DBHelper::addOperationLog("阈值调整", QString("温度上限调高至%1℃").arg(tempMax + 1), "成功");
}

void MainWindow::on_btnTempMaxDown_clicked()
{
    if (serial->isOpen()) serial->write("2");
    DBHelper::addOperationLog("阈值调整", QString("温度上限调低至%1℃").arg(tempMax - 1), "成功");
}

void MainWindow::on_btnTempMinUp_clicked()
{
    if (serial->isOpen()) serial->write("a");
    DBHelper::addOperationLog("阈值调整", QString("温度下限调高至%1℃").arg(tempMin + 1), "成功");
}

void MainWindow::on_btnTempMinDown_clicked()
{
    if (serial->isOpen()) serial->write("b");
    DBHelper::addOperationLog("阈值调整", QString("温度下限调低至%1℃").arg(tempMin - 1), "成功");
}

void MainWindow::on_switchAlarm_toggled(bool checked)
{
    if (!serial->isOpen()) {
        ui->switchAlarm->setChecked(!checked);
        return;
    }

    serial->write("e");

    if (checked) {
        ui->switchAlarm->setText("蜂鸣器警报：已开启");
        DBHelper::addOperationLog("警报", "开启蜂鸣器警报", "成功");
    } else {
        ui->switchAlarm->setText("蜂鸣器警报：已关闭");
        DBHelper::addOperationLog("警报", "关闭蜂鸣器警报", "成功");
    }
}

// 初始化历史数据表格
void MainWindow::initHistoryTable()
{
    ui->tableHistory->setColumnCount(4);
    ui->tableHistory->setHorizontalHeaderLabels({"序号", "温度(℃)", "湿度(%)", "时间"});
    ui->tableHistory->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不可编辑
}

// 添加历史数据，自动保留最新30条，超过自动删除旧数据
void MainWindow::addHistory(double temp, double humi)
{
    // 插入新行到最上面
    ui->tableHistory->insertRow(0);

    // 获取当前时间
    QString time = QDateTime::currentDateTime().toString("HH:mm:ss");

    // 设置单元格内容
    ui->tableHistory->setItem(0, 0, new QTableWidgetItem(QString::number(1)));
    ui->tableHistory->setItem(0, 1, new QTableWidgetItem(QString::number(temp, 'f', 1)));
    ui->tableHistory->setItem(0, 2, new QTableWidgetItem(QString::number(humi, 'f', 1)));
    ui->tableHistory->setItem(0, 3, new QTableWidgetItem(time));

    // 更新序号
    for(int i=0; i<ui->tableHistory->rowCount(); i++){
        ui->tableHistory->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
    }

    // 限制最多30条
    if(ui->tableHistory->rowCount() > 50){
        ui->tableHistory->removeRow(ui->tableHistory->rowCount()-1);
    }

    // 同步写入数据库
    DBHelper::addTempHistory(temp, humi);
}

void MainWindow::loadOperationLogs()
{
    ui->tableOperationLogs->setColumnCount(6);
    ui->tableOperationLogs->setHorizontalHeaderLabels({"序号", "设备名称", "操作类型", "操作结果", "操作人", "操作时间"});
    ui->tableOperationLogs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableOperationLogs->setRowCount(0);
    ui->tableOperationLogs->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableOperationLogs->setMouseTracking(true);
    ui->tableOperationLogs->show();

    static bool connected = false;
    if (!connected) {
        connect(ui->tableOperationLogs, &QTableWidget::entered, [=](const QModelIndex &index){
            QString text = index.data().toString();
            if (!text.isEmpty()) {
                QToolTip::showText(QCursor::pos(), text);
            }
        });
        connected = true;
    }

    QList<QMap<QString, QVariant>> logs;
    if (DBHelper::getOperationLogs(logs)) {
        int row = 0;
        for (const QMap<QString, QVariant>& log : logs) {
            ui->tableOperationLogs->insertRow(row);

            QString opName = log["operation"].toString();
            QString time = log["create_time"].toString();
            QString deviceName = log["device_name"].toString();
            QString result = log["result"].toString();
            QString operatorName = log["operator_name"].toString();

            QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(row + 1));
            QTableWidgetItem *item2 = new QTableWidgetItem(deviceName);
            QTableWidgetItem *item3 = new QTableWidgetItem(opName);
            QTableWidgetItem *item4 = new QTableWidgetItem(result);
            QTableWidgetItem *item5 = new QTableWidgetItem(operatorName);
            QTableWidgetItem *item6 = new QTableWidgetItem(time);

            item2->setTextAlignment(Qt::AlignCenter);
            item3->setTextAlignment(Qt::AlignCenter);
            item4->setTextAlignment(Qt::AlignCenter);
            item5->setTextAlignment(Qt::AlignCenter);
            item6->setTextAlignment(Qt::AlignCenter);

            ui->tableOperationLogs->setItem(row, 0, item1);
            ui->tableOperationLogs->setItem(row, 1, item2);
            ui->tableOperationLogs->setItem(row, 2, item3);
            ui->tableOperationLogs->setItem(row, 3, item4);
            ui->tableOperationLogs->setItem(row, 4, item5);
            ui->tableOperationLogs->setItem(row, 5, item6);
            row++;
        }
    }
}

void MainWindow::loadTempHistory()
{
    ui->tableTempHistory->setColumnCount(4);
    ui->tableTempHistory->setHorizontalHeaderLabels({"序号", "温度(℃)", "湿度(%)", "时间"});
    ui->tableTempHistory->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableTempHistory->setRowCount(0);
    ui->tableTempHistory->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableTempHistory->setMouseTracking(true);
    ui->tableTempHistory->show();

    static bool tempHistoryConnected = false;
    if (!tempHistoryConnected) {
        connect(ui->tableTempHistory, &QTableWidget::entered, [=](const QModelIndex &index){
            QString text = index.data().toString();
            if (!text.isEmpty()) {
                QToolTip::showText(QCursor::pos(), text);
            }
        });
        tempHistoryConnected = true;
    }

    QList<QMap<QString, QVariant>> records;
    if (DBHelper::getTempHistory(records)) {
        int row = 0;
        for (const QMap<QString, QVariant>& record : records) {
            ui->tableTempHistory->insertRow(row);

            double temp = record["temperature"].toDouble();
            double humi = record["humidity"].toDouble();
            QString time = record["create_time"].toString();

            QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(row + 1));
            QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(temp, 'f', 1));
            QTableWidgetItem *item3 = new QTableWidgetItem(QString::number(humi, 'f', 1));
            QTableWidgetItem *item4 = new QTableWidgetItem(time);

            item2->setTextAlignment(Qt::AlignCenter);
            item3->setTextAlignment(Qt::AlignCenter);
            item4->setTextAlignment(Qt::AlignCenter);

            ui->tableTempHistory->setItem(row, 0, item1);
            ui->tableTempHistory->setItem(row, 1, item2);
            ui->tableTempHistory->setItem(row, 2, item3);
            ui->tableTempHistory->setItem(row, 3, item4);
            row++;
        }
    }
}

// ======================
// 模式切换功能
// ======================
void MainWindow::on_btnComfortMode_clicked()
{
    QMessageBox::information(this, "模式", "✅ 舒适模式已开启\n温度：26℃\n灯光：50%\n窗帘：打开\n电视：关闭");
    DBHelper::addOperationLog("模式切换", "切换至舒适模式", "成功");
}

void MainWindow::on_btnSleepMode_clicked()
{
    QMessageBox::information(this, "模式", "✅ 睡眠模式已开启\n温度：24℃\n灯光：关闭\n窗帘：关闭\n电视：关闭");
    DBHelper::addOperationLog("模式切换", "切换至睡眠模式", "成功");
}

void MainWindow::on_btnCinemaMode_clicked()
{
    QMessageBox::information(this, "模式", "✅ 观影模式已开启\n温度：25℃\n灯光：关闭\n窗帘：关闭\n电视：打开");
    DBHelper::addOperationLog("模式切换", "切换至观影模式", "成功");
}

void MainWindow::on_btnManualMode_clicked()
{
    QMessageBox::information(this, "模式", "✅ 手动模式已开启\n可自由控制灯光、窗帘、空调、电视");
    DBHelper::addOperationLog("模式切换", "切换至手动模式", "成功");
}

