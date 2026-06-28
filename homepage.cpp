#include "homepage.h"
#include <QMessageBox>

HomePage::HomePage(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(520, 600);
    QGridLayout *grid = new QGridLayout(this);
    grid->setSpacing(20);
    grid->setContentsMargins(25, 25, 25, 25);

    // 每个卡片 独立图标！！！不再共用 iconLight
    auto cardLight   = createCard(":/images/icon_light_off.svg", "灯光控制", btnLight, iconLight);
    auto cardAir     = createCard(":/images/icon_aircon_off.svg", "空调控制", btnAir, iconAir);
    auto cardCurtain = createCard(":/images/icon_curtain_close.svg", "窗帘控制", btnCurtain, iconCurtain);
    auto cardDoor    = createCard(":/images/icon_door_close.svg", "一键开门", btnDoor, iconDoor);
    auto cardCamera  = createCard(":/images/camera.svg", "查看门口", btnCamera, iconCamera);
    auto cardTemp    = createCard(":/images/temp.svg", "温湿度检测", btnTemp, iconTemp);

    grid->addWidget(cardLight,   0, 0);
    grid->addWidget(cardAir,     0, 1);
    grid->addWidget(cardCurtain, 0, 2);
    grid->addWidget(cardDoor,    1, 0);
    grid->addWidget(cardCamera,  1, 1);
    grid->addWidget(cardTemp,    1, 2);

    connect(btnLight, &QPushButton::clicked, this, &HomePage::toggleLight);
    connect(btnCurtain, &QPushButton::clicked, this, &HomePage::toggleCurtain);
    connect(btnAir, &QPushButton::clicked, this, &HomePage::toggleAir);
    connect(btnDoor, &QPushButton::clicked, this, &HomePage::toggleDoor);

    connect(btnCamera, &QPushButton::clicked, this, &HomePage::gotoCamera);
    connect(btnTemp, &QPushButton::clicked, this, &HomePage::gotoTemp);
}

QFrame* HomePage::createCard(const QString& iconOff, const QString& title,
                             QPushButton*& btn, QLabel*& iconLabel)
{
    QFrame *frame = new QFrame;
    frame->setStyleSheet("background:white; border-radius:14px;");
    frame->setFixedSize(140, 170);

    QVBoxLayout *ly = new QVBoxLayout(frame);
    ly->setAlignment(Qt::AlignCenter);
    ly->setSpacing(10);

    iconLabel = new QLabel;
    iconLabel->setPixmap(QIcon(iconOff).pixmap(48,48));
    iconLabel->setAlignment(Qt::AlignCenter);

    QLabel *titleLab = new QLabel(title);
    titleLab->setAlignment(Qt::AlignCenter);

    btn = new QPushButton("开关");
    ly->addWidget(iconLabel);
    ly->addWidget(titleLab);
    ly->addWidget(btn);

    return frame;
}

void HomePage::updateIcon(QLabel* label, const QString& onPath, const QString& offPath, bool isOn)
{
    if(!label) return;
    label->setPixmap(QIcon(isOn ? onPath : offPath).pixmap(48,48));
}

// ====================== 纯开关，无房间，无菜单 ======================
void HomePage::toggleLight()
{
    lightOn = !lightOn;
    btnLight->setText(lightOn ? "已开启" : "已关闭");
    updateIcon(iconLight, ":/images/icon_light_on.svg", ":/images/icon_light_off.svg", lightOn);
    QMessageBox::information(this, "提示", lightOn ? "灯光已开启" : "灯光已关闭");

}

void HomePage::toggleCurtain()
{
    curtainOn = !curtainOn;
    btnCurtain->setText(curtainOn ? "已开启" : "已关闭");
    updateIcon(iconCurtain, ":/images/icon_curtain_open.svg", ":/images/icon_curtain_close.svg", curtainOn);
    QMessageBox::information(this, "提示", curtainOn ? "窗帘已开启" : "窗帘已关闭");
}

void HomePage::toggleAir()
{
    airOn = !airOn;
    btnAir->setText(airOn ? "已开启" : "已关闭");
    updateIcon(iconAir, ":/images/icon_aircon_on.svg", ":/images/icon_aircon_off.svg", airOn);
    QMessageBox::information(this, "提示", airOn ? "空调已开启" : "空调已关闭");
}

void HomePage::toggleDoor()
{
    doorOpen = !doorOpen;
    btnDoor->setText(doorOpen ? "门已开" : "关门");
    updateIcon(iconDoor, ":/images/icon_door_open.svg", ":/images/icon_door_close.svg", doorOpen);
    QMessageBox::information(this, "提示", doorOpen ? "门已打开" : "门已关闭");
}

void HomePage::gotoCamera()
{
    emit goToCameraPage();
}

void HomePage::gotoTemp()
{
    emit goToTempPage();
}
