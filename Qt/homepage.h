#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QFrame>
#include <QIcon>
#include <QMessageBox>
#include <QComboBox>
#include <QDialog>
#include <QSlider>

class HomePage : public QWidget
{
    Q_OBJECT
public:
    explicit HomePage(QWidget *parent = nullptr);

signals:
    void goToTempPage();
    void goToCameraPage();

public slots:
    void openDoorOnly();
    void setAirTemp(int temp);
    void setLightIntensity(int intensity);
    void setCurtain(bool isOpen);

private slots:
    void toggleLight();
    void toggleAir();
    void toggleCurtain();
    void toggleDoor();

    void gotoCamera();
    void gotoTemp();

private:
    QFrame* createCard(const QString& iconOff, const QString& title, QPushButton*& btn, QLabel*& iconLabel, QLabel*& statusLabel);
    void updateIcon(QLabel* label, const QString& onPath, const QString& offPath, bool isOn);
    void updateLightIcon();

    QPushButton *btnLight, *btnAir, *btnCurtain, *btnDoor, *btnCamera, *btnTemp;
    QLabel *iconLight, *iconAir, *iconCurtain, *iconDoor, *iconCamera, *iconTemp;
    QLabel *statusLight, *statusAir, *statusCurtain, *statusDoor, *statusCamera, *statusTemp;

    bool lightOn = false;
    bool airOn = false;
    bool curtainOn = false;
    bool doorOpen = false;
    int airTemp = 26;
    int lightIntensity = 100;
    QString currentRoom = "全屋";
    bool roomLightState[4] = {false, false, false, false};
};

#endif // HOMEPAGE_H