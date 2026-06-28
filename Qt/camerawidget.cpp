#include "camerawidget.h"
#include "dbhelper.h"

CameraWidget::CameraWidget(QWidget *parent)
    : QWidget(parent), camera(nullptr), captureSession(nullptr), isCameraActive(false)
{
    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(20);

    // 标题
    QLabel *title = new QLabel("门口监控");
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #1E3A5F;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    // 摄像机选择下拉框
    QHBoxLayout *selectorLayout = new QHBoxLayout;
    QLabel *selectorLabel = new QLabel("选择摄像头:");
    selectorLabel->setStyleSheet("font-size: 14px; color: #4A6080;");
    cameraSelector = new QComboBox;
    cameraSelector->setMinimumWidth(200);

    // 获取可用的摄像头列表
    QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    if (cameras.isEmpty()) {
        cameraSelector->addItem("未检测到摄像头");
        cameraSelector->setEnabled(false);
    } else {
        for (const QCameraDevice &cam : cameras) {
            cameraSelector->addItem(cam.description());
        }
    }
    selectorLayout->addWidget(selectorLabel);
    selectorLayout->addWidget(cameraSelector);
    selectorLayout->addStretch();
    mainLayout->addLayout(selectorLayout);

    // 视频显示区域
    videoWidget = new QVideoWidget;
    videoWidget->setMinimumSize(420, 300);
    videoWidget->setStyleSheet(R"(
        QVideoWidget {
            background-color: #1E3A5F;
            border-radius: 16px;
            border: 1px solid rgba(200, 220, 255, 0.5);
        }
    )");
    mainLayout->addWidget(videoWidget);

    // 状态标签
    statusLabel = new QLabel("摄像头未启动");
    statusLabel->setStyleSheet("font-size: 14px; color: #8BA4C0; font-weight: 500;");
    statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statusLabel);

    // 控制按钮布局
    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setSpacing(15);

    btnStart = new QPushButton("启动摄像头");
    btnStop = new QPushButton("停止摄像头");
    btnStop->setEnabled(false);

    QString btnStyle = R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #4A90D9, stop:1 #357ABD);
            color: white;
            border-radius: 8px;
            padding: 8px 20px;
            font-weight: 600;
            border: none;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #5BA0E9, stop:1 #4A90D9);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #357ABD, stop:1 #2868A8);
        }
        QPushButton:disabled {
            background-color: #C5D5E8;
            color: #8BA4C0;
        }
    )";

    btnStart->setStyleSheet(btnStyle);
    btnStop->setStyleSheet(btnStyle);

    controlLayout->addWidget(btnStart);
    controlLayout->addWidget(btnStop);
    mainLayout->addLayout(controlLayout);

    // 操作按钮：开门 + 挂断 + 返回
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(15);

    btnOpenDoor = new QPushButton("开门");
    btnHangUp   = new QPushButton("挂断");
    btnBack     = new QPushButton("返回主页");

    btnOpenDoor->setStyleSheet(btnStyle);
    btnHangUp->setStyleSheet(btnStyle);
    btnBack->setStyleSheet(btnStyle);

    btnLayout->addWidget(btnOpenDoor);
    btnLayout->addWidget(btnHangUp);
    btnLayout->addStretch();
    btnLayout->addWidget(btnBack);

    mainLayout->addLayout(btnLayout);

    // 绑定信号
    connect(btnStart, &QPushButton::clicked, this, &CameraWidget::startCamera);
    connect(btnStop, &QPushButton::clicked, this, &CameraWidget::stopCamera);
    connect(cameraSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CameraWidget::onCameraChanged);
    connect(btnOpenDoor, &QPushButton::clicked, this, &CameraWidget::openDoor);
    connect(btnHangUp,   &QPushButton::clicked, this, &CameraWidget::hangUp);
    connect(btnBack,     &QPushButton::clicked, this, &CameraWidget::backToHome);
}

CameraWidget::~CameraWidget()
{
    if (camera) {
        camera->stop();
        delete camera;
    }
    if (captureSession) {
        delete captureSession;
    }
}

void CameraWidget::startCamera()
{
    QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    if (cameras.isEmpty()) {
        QMessageBox::warning(this, "警告", "未检测到可用摄像头！");
        return;
    }

    int index = cameraSelector->currentIndex();
    if (index < 0 || index >= cameras.size()) {
        QMessageBox::warning(this, "警告", "请选择有效的摄像头！");
        return;
    }

    // 创建摄像头会话
    if (!captureSession) {
        captureSession = new QMediaCaptureSession(this);
        captureSession->setVideoOutput(videoWidget);
    }

    // 设置摄像头
    if (camera) {
        camera->stop();
        delete camera;
    }

    camera = new QCamera(cameras[index], this);
    captureSession->setCamera(camera);

    // 启动摄像头
    camera->start();

    isCameraActive = true;
    statusLabel->setText("摄像头已启动 - 实时监控中");
    statusLabel->setStyleSheet("font-size: 14px; color: #357ABD; font-weight: 500;");

    btnStart->setEnabled(false);
    btnStop->setEnabled(true);

    DBHelper::addOperationLog("摄像头", "启动摄像头监控", "成功");
}

void CameraWidget::stopCamera()
{
    if (camera) {
        camera->stop();
    }

    isCameraActive = false;
    statusLabel->setText("摄像头已停止");
    statusLabel->setStyleSheet("font-size: 14px; color: #8BA4C0; font-weight: 500;");

    btnStart->setEnabled(true);
    btnStop->setEnabled(false);

    DBHelper::addOperationLog("摄像头", "停止摄像头监控", "成功");
}

void CameraWidget::onCameraChanged(int index)
{
    // 如果摄像头正在运行，切换到新选择的摄像头
    if (isCameraActive && index >= 0) {
        QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
        if (index < cameras.size()) {
            if (camera) {
                camera->stop();
                delete camera;
            }
            camera = new QCamera(cameras[index], this);
            captureSession->setCamera(camera);
            camera->start();
            DBHelper::addOperationLog("摄像头", "切换摄像头设备", "成功");
        }
    }
}

void CameraWidget::openDoor()
{

    QMessageBox::information(this, "提示", "门锁已打开！");
    DBHelper::addOperationLog("门锁", "远程开门", "成功");
}

void CameraWidget::hangUp()
{
    stopCamera();
    QMessageBox::information(this, "提示", "已挂断！");
}