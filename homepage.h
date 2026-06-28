#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QFrame>
#include <QIcon>
#include <QMessageBox>

class HomePage : public QWidget
{
    Q_OBJECT
public:
    explicit HomePage(QWidget *parent = nullptr);

signals:
    void goToTempPage();
    void goToCameraPage();

private slots:
    void toggleLight();
    void toggleAir();
    void toggleCurtain();
    void toggleDoor();

    void gotoCamera();
    void gotoTemp();

private:
    QFrame* createCard(const QString& iconOff, const QString& title, QPushButton*& btn, QLabel*& iconLabel);
    void updateIcon(QLabel* label, const QString& onPath, const QString& offPath, bool isOn);

    // 每个图标独立！！！不共用
    QPushButton *btnLight, *btnAir, *btnCurtain, *btnDoor, *btnCamera, *btnTemp;
    QLabel *iconLight, *iconAir, *iconCurtain, *iconDoor, *iconCamera, *iconTemp;

    bool lightOn = false;
    bool airOn = false;
    bool curtainOn = false;
    bool doorOpen = false;
};

#endif // HOMEPAGE_H