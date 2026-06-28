#include "camerawidget.h"

CameraWidget::CameraWidget(QWidget *parent)
    : QWidget(parent)
{
    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setSpacing(25);

    // 标题
    QLabel *title = new QLabel("📷 门口监控");
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: #333;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    // ==========================
    // 监控画面（纯图片模拟）
    // ==========================
    imgMonitor = new QLabel;
    imgMonitor->setMinimumSize(420, 300);
    imgMonitor->setStyleSheet(R"(
        QLabel {
            background-color: #FFFFFF;
            border-radius: 16px;
            border: 1px solid #E6EBF2;
        }
    )");
    imgMonitor->setAlignment(Qt::AlignCenter);
    imgMonitor->setText("📷 实时监控中\n点击开门可解锁门锁");

    mainLayout->addWidget(imgMonitor);

    // ==========================
    // 按钮：开门 + 挂断
    // ==========================
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(15);

    btnOpenDoor = new QPushButton("✅ 开门");
    btnHangUp   = new QPushButton("❌ 挂断");
    btnBack     = new QPushButton("返回主页");

    QString btnStyle = R"(
        QPushButton {
            background-color: #4FACFE;
            color: white;
            border-radius: 6px;
            padding: 8px 16px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #00F2FE;
        }
        QPushButton:pressed {
            background-color: #3B9BEE;
        }
    )";

    btnOpenDoor->setStyleSheet(btnStyle);
    btnHangUp->setStyleSheet(btnStyle);
    btnBack->setStyleSheet(btnStyle);

    btnLayout->addWidget(btnOpenDoor);
    btnLayout->addWidget(btnHangUp);
    btnLayout->addStretch();
    btnLayout->addWidget(btnBack);

    mainLayout->addLayout(btnLayout);

    // 绑定
    connect(btnOpenDoor, &QPushButton::clicked, this, &CameraWidget::openDoor);
    connect(btnHangUp,   &QPushButton::clicked, this, &CameraWidget::hangUp);
    connect(btnBack,     &QPushButton::clicked, this, &CameraWidget::backToHome);
}

// 开门
void CameraWidget::openDoor()
{
    QMessageBox::information(this, "提示", "✅ 门锁已打开！");
}

// 挂断
void CameraWidget::hangUp()
{
    imgMonitor->setText("❌ 监控已关闭");
    QMessageBox::information(this, "提示", "❌ 已挂断！");
}