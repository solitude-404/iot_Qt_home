#include "homepage.h"
#include "dbhelper.h"
#include <QMessageBox>
#include <QDialog>
#include <QSpinBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>

HomePage::HomePage(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(520, 600);
    QGridLayout *grid = new QGridLayout(this);
    grid->setSpacing(20);
    grid->setContentsMargins(25, 25, 25, 25);

    auto cardLight = createCard(":/images/icon_light_off.svg", "灯光控制", btnLight, iconLight, statusLight);
    auto cardAir = createCard(":/images/icon_aircon_off.svg", "空调控制", btnAir, iconAir, statusAir);
    auto cardCurtain = createCard(":/images/icon_curtain_close.svg", "窗帘控制", btnCurtain, iconCurtain, statusCurtain);
    auto cardDoor = createCard(":/images/icon_door_close.svg", "一键开门", btnDoor, iconDoor, statusDoor);
    auto cardCamera = createCard(":/images/camera.svg", "查看门口", btnCamera, iconCamera, statusCamera);
    auto cardTemp = createCard(":/images/temp.svg", "温湿度检测", btnTemp, iconTemp, statusTemp);

    grid->addWidget(cardLight, 0, 0);
    grid->addWidget(cardAir, 0, 1);
    grid->addWidget(cardCurtain, 0, 2);
    grid->addWidget(cardDoor, 1, 0);
    grid->addWidget(cardCamera, 1, 1);
    grid->addWidget(cardTemp, 1, 2);

    connect(btnLight, &QPushButton::clicked, this, &HomePage::toggleLight);
    connect(btnCurtain, &QPushButton::clicked, this, &HomePage::toggleCurtain);
    connect(btnAir, &QPushButton::clicked, this, &HomePage::toggleAir);
    connect(btnDoor, &QPushButton::clicked, this, &HomePage::toggleDoor);
    connect(btnCamera, &QPushButton::clicked, this, &HomePage::gotoCamera);
    connect(btnTemp, &QPushButton::clicked, this, &HomePage::gotoTemp);

    bool stat;
    if(DBHelper::getDeviceStatus("light", stat))
    {
        lightOn = stat;
        btnLight->setText(lightOn ? "已开启" : "已关闭");
        statusLight->setText(lightOn ? QString("已开启 %1%").arg(lightIntensity) : "已关闭");
        updateLightIcon();
    }
    if(DBHelper::getDeviceStatus("air", stat))
    {
        airOn = stat;
        btnAir->setText(airOn ? QString("已开启(%1°C)").arg(airTemp) : "已关闭");
        statusAir->setText(airOn ? QString("已开启 %1°C").arg(airTemp) : "已关闭");
        updateIcon(iconAir, ":/images/icon_aircon_on.svg", ":/images/icon_aircon_off.svg", airOn);
    }
    if(DBHelper::getDeviceStatus("curtain", stat))
    {
        curtainOn = stat;
        btnCurtain->setText(curtainOn ? "已开启" : "已关闭");
        statusCurtain->setText(curtainOn ? "已开启" : "已关闭");
        updateIcon(iconCurtain, ":/images/icon_curtain_open.svg", ":/images/icon_curtain_close.svg", curtainOn);
    }
    if(DBHelper::getDeviceStatus("door", stat))
    {
        doorOpen = stat;
        btnDoor->setText(doorOpen ? "门已开" : "关门");
        statusDoor->setText(doorOpen ? "已开启" : "已关闭");
        updateIcon(iconDoor, ":/images/icon_door_open.svg", ":/images/icon_door_close.svg", doorOpen);
    }
}

QFrame* HomePage::createCard(const QString& iconOff, const QString& title,
                             QPushButton*& btn, QLabel*& iconLabel, QLabel*& statusLabel)
{
    QFrame *frame = new QFrame;
    frame->setStyleSheet("background:white; border-radius:14px;");
    frame->setFixedSize(140, 170);

    QVBoxLayout *ly = new QVBoxLayout(frame);
    ly->setAlignment(Qt::AlignCenter);
    ly->setSpacing(5);

    iconLabel = new QLabel;
    iconLabel->setPixmap(QIcon(iconOff).pixmap(48,48));
    iconLabel->setAlignment(Qt::AlignCenter);

    QLabel *titleLab = new QLabel(title);
    titleLab->setAlignment(Qt::AlignCenter);

    statusLabel = new QLabel("已关闭");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("font-size: 12px; color: #8BA4C0;");

    btn = new QPushButton("开关");
    ly->addWidget(iconLabel);
    ly->addWidget(titleLab);
    ly->addWidget(statusLabel);
    ly->addWidget(btn);

    return frame;
}

void HomePage::updateIcon(QLabel* label, const QString& onPath, const QString& offPath, bool isOn)
{
    if(!label) return;
    label->setPixmap(QIcon(isOn ? onPath : offPath).pixmap(48,48));
}

void HomePage::updateLightIcon()
{
    bool allOff = true;
    for(int i=0; i<4; i++) {
        if(roomLightState[i]) { allOff = false; break; }
    }
    updateIcon(iconLight, ":/images/icon_light_on.svg", ":/images/icon_light_off.svg", !allOff);
}

void HomePage::toggleLight()
{
    QDialog dlg(this);
    dlg.setWindowTitle("灯光控制");
    dlg.setFixedSize(280, 300);

    QVBoxLayout *layout = new QVBoxLayout(&dlg);

    QComboBox *roomCombo = new QComboBox;
    roomCombo->addItems({"全屋", "客厅", "卧室", "厨房", "卫生间"});
    layout->addWidget(new QLabel("选择房间："));
    layout->addWidget(roomCombo);

    QPushButton *btnOn = new QPushButton("开启");
    QPushButton *btnOff = new QPushButton("关闭");
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(btnOn);
    btnLayout->addWidget(btnOff);
    layout->addLayout(btnLayout);

    QLabel *intensityLabel = new QLabel(QString("灯光强度：%1%").arg(lightIntensity));
    intensityLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(intensityLabel);

    QSlider *intensitySlider = new QSlider(Qt::Horizontal);
    intensitySlider->setRange(10, 100);
    intensitySlider->setValue(lightIntensity);
    intensitySlider->setTickInterval(10);
    intensitySlider->setTickPosition(QSlider::TicksBelow);
    layout->addWidget(intensitySlider);

    connect(intensitySlider, &QSlider::valueChanged, [=](int value) {
        intensityLabel->setText(QString("灯光强度：%1%").arg(value));
    });

    QString opText;
    int clickedBtn = -1;

    connect(btnOn, &QPushButton::clicked, [&](){ clickedBtn = 1; dlg.accept(); });
    connect(btnOff, &QPushButton::clicked, [&](){ clickedBtn = 0; dlg.accept(); });

    if(dlg.exec() == QDialog::Accepted && clickedBtn >= 0)
    {
        QString room = roomCombo->currentText();
        bool isOn = (clickedBtn == 1);

        if(isOn) {
            lightIntensity = intensitySlider->value();
        }

        if(room == "全屋")
        {
            lightOn = isOn;
            for(int i=0; i<4; i++) {
                roomLightState[i] = isOn;
            }
            opText = isOn ? QString("开启全屋灯光(%1%)").arg(lightIntensity) : "关闭全屋灯光";
        } else {
            int idx = roomCombo->currentIndex() - 1;
            roomLightState[idx] = isOn;
            opText = isOn ? QString("开启%1灯光(%2%)").arg(room).arg(lightIntensity) : QString("关闭%1灯光").arg(room);

            bool allOff = true;
            for(int i=0; i<4; i++) {
                if(roomLightState[i]) { allOff = false; break; }
            }
            lightOn = !allOff;
        }

        btnLight->setText(lightOn ? "已开启" : "已关闭");
        statusLight->setText(lightOn ? QString("已开启 %1%").arg(lightIntensity) : "已关闭");
        updateLightIcon();
        DBHelper::updateDeviceStatus("light", lightOn);
        DBHelper::addOperationLog("灯光", opText, "成功");
        QMessageBox::information(this, "提示", opText);
    }
}

void HomePage::toggleCurtain()
{
    curtainOn = !curtainOn;
    btnCurtain->setText(curtainOn ? "已开启" : "已关闭");
    statusCurtain->setText(curtainOn ? "已开启" : "已关闭");
    updateIcon(iconCurtain, ":/images/icon_curtain_open.svg", ":/images/icon_curtain_close.svg", curtainOn);
    DBHelper::updateDeviceStatus("curtain", curtainOn);
    DBHelper::addOperationLog("窗帘", curtainOn ? "开启" : "关闭", "成功");
    QMessageBox::information(this, "提示", curtainOn ? "窗帘已开启" : "窗帘已关闭");
}

void HomePage::toggleAir()
{
    QDialog dlg(this);
    dlg.setWindowTitle("空调控制");
    dlg.setFixedSize(280, 220);

    QVBoxLayout *layout = new QVBoxLayout(&dlg);

    QComboBox *modeCombo = new QComboBox;
    modeCombo->addItems({"开启空调", "关闭空调", "调节温度"});
    layout->addWidget(new QLabel("操作类型："));
    layout->addWidget(modeCombo);

    QSpinBox *tempSpin = new QSpinBox;
    tempSpin->setRange(16, 30);
    tempSpin->setValue(airTemp);
    tempSpin->setSuffix("℃");
    layout->addWidget(new QLabel("目标温度："));
    layout->addWidget(tempSpin);

    QPushButton *btnConfirm = new QPushButton("确认");
    layout->addWidget(btnConfirm);

    connect(btnConfirm, &QPushButton::clicked, &dlg, &QDialog::accept);

    if(dlg.exec() == QDialog::Accepted)
    {
        QString mode = modeCombo->currentText();
        QString opText;

        if(mode == "开启空调")
        {
            airOn = true;
            airTemp = tempSpin->value();
            opText = QString("开启空调(%1°C)").arg(airTemp);
        }
        else if(mode == "关闭空调")
        {
            airOn = false;
            opText = "关闭空调";
        }
        else
        {
            airTemp = tempSpin->value();
            opText = QString("空调温度调至%1°C").arg(airTemp);
        }

        if(mode != "调节温度")
        {
            updateIcon(iconAir, ":/images/icon_aircon_on.svg", ":/images/icon_aircon_off.svg", airOn);
            DBHelper::updateDeviceStatus("air", airOn);
        }

        btnAir->setText(airOn ? QString("已开启(%1°C)").arg(airTemp) : "已关闭");
        statusAir->setText(airOn ? QString("已开启 %1°C").arg(airTemp) : "已关闭");
        DBHelper::addOperationLog("空调", opText, "成功");
        QMessageBox::information(this, "提示", opText);
    }
}

void HomePage::openDoorOnly()
{
    if (!doorOpen) {
        doorOpen = true;
        btnDoor->setText("门已开");
        statusDoor->setText("已开启");
        updateIcon(iconDoor, ":/images/icon_door_open.svg", ":/images/icon_door_close.svg", true);
        DBHelper::updateDeviceStatus("door", true);
        DBHelper::addOperationLog("门", "远程开门", "成功");
        QMessageBox::information(this, "提示", "门已打开");
    }
}

void HomePage::toggleDoor()
{
    doorOpen = !doorOpen;
    btnDoor->setText(doorOpen ? "门已开" : "关门");
    statusDoor->setText(doorOpen ? "已开启" : "已关闭");
    updateIcon(iconDoor, ":/images/icon_door_open.svg", ":/images/icon_door_close.svg", doorOpen);
    DBHelper::updateDeviceStatus("door", doorOpen);
    DBHelper::addOperationLog("门", doorOpen ? "开门" : "关门", "成功");
    QMessageBox::information(this, "提示", doorOpen ? "门已打开" : "门已关闭");
}

void HomePage::setAirTemp(int temp)
{
    if (!airOn) {
        airOn = true;
        updateIcon(iconAir, ":/images/icon_aircon_on.svg", ":/images/icon_aircon_off.svg", true);
        DBHelper::updateDeviceStatus("air", true);
    }
    airTemp = temp;
    btnAir->setText(QString("已开启(%1°C)").arg(airTemp));
    statusAir->setText(QString("已开启 %1°C").arg(airTemp));
    DBHelper::updateDeviceStatus("air", true);
    DBHelper::addOperationLog("空调", QString("空调温度调至%1°C").arg(airTemp), "成功");
}

void HomePage::setLightIntensity(int intensity)
{
    if (intensity == 0) {
        lightOn = false;
        for(int i=0; i<4; i++) {
            roomLightState[i] = false;
        }
        lightIntensity = 100;
        btnLight->setText("已关闭");
        statusLight->setText("已关闭");
        updateLightIcon();
        DBHelper::updateDeviceStatus("light", false);
    } else {
        if (!lightOn) {
            lightOn = true;
            for(int i=0; i<4; i++) {
                roomLightState[i] = true;
            }
            updateLightIcon();
            DBHelper::updateDeviceStatus("light", true);
        }
        lightIntensity = intensity;
        btnLight->setText("已开启");
        statusLight->setText(QString("已开启 %1%").arg(lightIntensity));
    }
}

void HomePage::setCurtain(bool isOpen)
{
    curtainOn = isOpen;
    btnCurtain->setText(curtainOn ? "已开启" : "已关闭");
    statusCurtain->setText(curtainOn ? "已开启" : "已关闭");
    updateIcon(iconCurtain, ":/images/icon_curtain_open.svg", ":/images/icon_curtain_close.svg", curtainOn);
    DBHelper::updateDeviceStatus("curtain", curtainOn);
    DBHelper::addOperationLog("窗帘", curtainOn ? "开启" : "关闭", "成功");
}

void HomePage::gotoCamera()
{
    emit goToCameraPage();
}

void HomePage::gotoTemp()
{
    emit goToTempPage();
}